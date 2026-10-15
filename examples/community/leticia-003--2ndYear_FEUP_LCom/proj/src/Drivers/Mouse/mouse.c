/**
 * @file mouse.c
 * @brief Mouse driver implementation for handling mouse interrupts and communication with the i8042 controller.
 *
 * This file provides functions for subscribing/unsubscribing mouse interrupts,
 * handling mouse interrupts, sending commands to the mouse, parsing mouse packets,
 * and updating gesture state for gesture recognition.
 */

#include "mouse.h"
#include "i8042.h"

int    hook_id_mouse = MOUSE_IRQ;     
uint8_t pkt_bytes[3];                  
uint8_t pkt_index = 0;                 
struct packet pkt;                     

/**
 * @brief Issues a command to the specified KBC port.
 *
 * Tries up to ATTEMPTS times to write the command if the input buffer is not full.
 *
 * @param port KBC port to write to.
 * @param cmd Command byte to write.
 * @return 0 on success, 1 on failure.
 */
static int (kbc_issue_cmd)(uint8_t port, uint8_t cmd) {
  for (int at = 0; at < ATTEMPTS; ++at) {
    uint8_t status;
    if (util_sys_inb(STAT_REG, &status) != OK) return 1;
    if ((status & IBF) == 0) {
      if (sys_outb(port, cmd) != OK) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

/**
 * @brief Sends a command to the mouse.
 *
 * Sends 0xD4 to the KBC command register so the next byte goes to the mouse,
 * then writes the actual command and waits for an ACK.
 *
 * @param cmd Mouse command byte to send.
 * @return 0 on success, 1 on failure.
 */
static int (mouse_write_cmd)(uint8_t cmd) {
  if (kbc_issue_cmd(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE)) return 1;
  if (kbc_issue_cmd(IN_BUF, cmd)) return 1;

  for (int at = 0; at < ATTEMPTS; ++at) {
    uint8_t status;
    if (util_sys_inb(STAT_REG, &status) != OK) return 1;
    if (status & OBF) {
      uint8_t resp;
      if (util_sys_inb(OUT_BUF, &resp) != OK) return 1;
      if      (resp == ACK)  return 0;   
      else if (resp == NACK) continue;   
      else                   return 1;   
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

/**
 * @brief Checks if the given byte is a sync byte.
 *
 * A sync byte is a byte with bit 3 set to 1.
 *
 * @param b Byte to check.
 * @return true if sync byte, false otherwise.
 */
static bool (is_sync_byte)(uint8_t b) {
  return b & BIT(3);
}

/**
 * @brief Subscribes mouse interrupts (IRQ 12).
 *
 * Sets the IRQ policy for mouse interrupts and enables exclusive access.
 *
 * @param bit_no Pointer to variable where the hook id bit number will be stored.
 * @return 0 on success, 1 on failure.
 */
int mouse_subscribe_int(uint8_t *bit_no)
{
    hook_id_mouse = MOUSE_IRQ;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != OK) return 1;
    if (sys_irqenable(&hook_id_mouse) != OK) return 1;
    *bit_no = MOUSE_IRQ;
    return 0;
}

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * Removes the IRQ policy for mouse interrupts.
 *
 * @return 0 on success, 1 on failure.
 */
int mouse_unsubscribe_int(void){
  sys_irqdisable(&hook_id_mouse);
  return sys_irqrmpolicy(&hook_id_mouse);
}

/**
 * @brief Enables mouse data reporting (stream mode).
 *
 * Sends the enable data reporting command (0xF4) to the mouse.
 *
 * @return 0 on success, 1 on failure.
 */
int mouse_send_cmd_enable(void)  { return mouse_write_cmd(MOUSE_ENABLE_DR);  }

/**
 * @brief Disables mouse data reporting (stream mode).
 *
 * Sends the disable data reporting command (0xF5) to the mouse.
 *
 * @return 0 on success, 1 on failure.
 */
int mouse_send_cmd_disable(void) { return mouse_write_cmd(MOUSE_DISABLE_DR); }

/**
 * @brief Mouse interrupt handler.
 *
 * Reads a byte from the mouse, checks for sync on the first byte,
 * and stores bytes until a full packet is ready.
 */
void (mouse_ih)(void) {
    uint8_t status, byte;

    if (util_sys_inb(STAT_REG, &status) != OK)
        return;

    if (!(status & OBF) || !(status & AUX) || (status & (PAR_ERR | TO_ERR)))
        return;

    if (util_sys_inb(OUT_BUF, &byte) != OK)
        return;

    if (pkt_index == 0 && !is_sync_byte(byte))
        return;

    pkt_bytes[pkt_index++] = byte;

    if (pkt_index == 3) {
        pkt_index = 0; 
    }
}

/**
 * @brief Parses a mouse packet from 3 bytes.
 *
 * Decodes the 3 bytes received from the mouse into a packet structure.
 *
 * @param pp Pointer to the packet structure to fill.
 * @param bytes Array of 3 bytes from the mouse.
 * @return 0 on success, 1 on failure.
 */
int (mouse_parse_packet)(struct packet *pp, uint8_t bytes[3]) {
  if (!pp) return 1;

  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];

  pp->lb   = bytes[0] & BIT(0); 
  pp->rb   = bytes[0] & BIT(1); 
  pp->mb   = bytes[0] & BIT(2); 
  pp->x_ov = bytes[0] & BIT(6); 
  pp->y_ov = bytes[0] & BIT(7); 
  
  int16_t dx = bytes[1];
  if (bytes[0] & BIT(4)) dx |= 0xFF00;
  pp->delta_x = dx;

  int16_t dy = bytes[2];
  if (bytes[0] & BIT(5)) dy |= 0xFF00;
  pp->delta_y = dy;

  return 0;
}

/**
 * @brief Updates the gesture state machine based on the mouse packet.
 *
 * Handles gesture recognition logic for a specific gesture (e.g., V-shape).
 *
 * @param state Current gesture state.
 * @param pkt Pointer to the latest mouse packet.
 * @param dx_total Pointer to accumulated x movement.
 * @param dy_total Pointer to accumulated y movement.
 * @param x_len Minimum x/y movement required for a gesture segment.
 * @param tolerance Maximum allowed deviation for gesture recognition.
 * @return Updated gesture state.
 */
gesture_state_t (update_gesture_state)(gesture_state_t state, struct packet *pkt, int16_t *dx_total, int16_t *dy_total, uint8_t x_len, uint8_t tolerance) {

    int16_t dx = pkt->delta_x;
    int16_t dy = pkt->delta_y; 
    bool lb = pkt->lb, rb = pkt->rb, mb = pkt->mb; 
    switch (state) {

        case INIT:
        if (lb && !rb && !mb) {
            *dx_total = 0; *dy_total = 0;
            return DRAW_UP;
        }
        break;

        case DRAW_UP:
        if (!lb) {
            if (abs(*dx_total) >= x_len && abs(*dy_total) >= x_len && abs(*dy_total) > abs(*dx_total))
            return VERTEX;
            else return INIT;
        } else if (rb || mb || dx < -tolerance || dy < -tolerance) {
            return INIT;
        } else {
            *dx_total += dx;
            *dy_total += dy;
        }
        break;

        case VERTEX:
        if (rb && !lb && !mb) {
            *dx_total = 0; *dy_total = 0;
            return DRAW_DOWN;
        } else if (abs(dx) > tolerance || abs(dy) > tolerance || lb || mb) {
            return INIT;
        }
        break;

        case DRAW_DOWN:
        if (!rb) {
            if (abs(*dx_total) >= x_len && abs(*dy_total) >= x_len && abs(*dy_total) > abs(*dx_total))
            return COMPLETE;
            else return INIT;
        } else if (lb || mb || dx < -tolerance || dy > tolerance) {
            return INIT;
        } else {
            *dx_total += dx;
            *dy_total += dy;
        }
        break;

        default:
        break;
    }

    return state;
}

