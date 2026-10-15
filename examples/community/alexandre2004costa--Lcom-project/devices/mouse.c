#include "mouse.h"

/** @defgroup mouse mouse
  * @{
 */

#define DELAY_US 20000
int mouse_hook_id = 2; 
uint8_t scancode = 0;
int byteCount = 0;
struct packet pp;

/**
 * @brief Mouse interrupt handler.
 * Reads the mouse scancode from the output buffer.
 */
void (mouse_ih)() {
  read_KBC_output(OUT_KBC_CMD, &scancode, true);
}

/**
 * @brief Manages mouse packets.
 * Processes the bytes of a mouse packet and fills the packet structure.
 */
void (managePackages)() {
  byteCount++;
  if (byteCount == 1) {  
    pp.bytes[0] = scancode;
    pp.lb = scancode & LB_MOUSE_PACKET;
    pp.rb = scancode & RB_MOUSE_PACKET;
    pp.mb = scancode & MB_MOUSE_PACKET;
    pp.x_ov = scancode & X_OV_MOUSE_PACKET;
    pp.y_ov = scancode & Y_OV_MOUSE_PACKET;
  } else if (byteCount == 2) {
    pp.delta_x = scancode;
    pp.bytes[1] = scancode;
    if (pp.bytes[0] & MSB_X_DELTA) {
      pp.delta_x = 0xFF00 | pp.bytes[1];
    }
  } else {
    pp.delta_y = scancode;
    pp.bytes[2] = scancode;
    byteCount = 0;
    if (pp.bytes[0] & MSB_Y_DELTA) {
      pp.delta_y = 0xFF00 | pp.bytes[2];
    }
  }
}

/**
 * @brief Subscribes mouse interrupts.
 * @param bit_no Pointer to bit number to be set with the IRQ line.
 * @return 0 on success, 1 otherwise.
 */
int (mouse_subscribe_int) (uint8_t *bit_no) {
  if(bit_no == NULL) return 1;   
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

/**
 * @brief Unsubscribes mouse interrupts.
 * @return 0 on success, 1 otherwise.
 */
int (mouse_unsubscribe_int) () {
  return sys_irqrmpolicy(&mouse_hook_id);
}

/**
 * @brief Writes a command byte to the mouse.
 * @param commandByte Command byte to be written.
 * @return 0 on success, 1 otherwise.
 */
int (write_to_mouse)(uint8_t commandByte) {
  int k = 10;
  while (k > 0) {
    if (write_KBC_command(IN_KBC_CMD, MOUSE_REQUEST) != 0) return 1;
    if (write_KBC_command(WRITE_CMD, commandByte) != 0) return 1;
    uint8_t readBack;
    if (read_KBC_output(OUT_KBC_CMD, &readBack, true) != 0) return 1; 
    if (readBack == SUCESSS_BYTE) return 0;
    tickdelay(micros_to_ticks(DELAY_US));
    k--;
  }
  return 1;
}

/**@}*/
