#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "mouse.h"

/** @defgroup Mouse
 * @{
 *
 * Functions related to the use of the mouse in the project
 */

uint8_t st;
uint8_t data_m;
int hook_id = 12;
int byteCounter = 1;

/**
 * @brief Subscribes to mouse interrupts.
 *
 * @param bit_no Pointer to the variable where the bit number of the mouse IRQ will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(mouse_subscribe_interrupts)(uint8_t *bit_no) {
  *bit_no = hook_id;
  sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  return 0;
}

/**
 * @brief Unsubscribes from mouse interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int(mouse_unsubscribe_interrupts)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

/**
 * @brief Mouse interrupt handler.
 */
void(mouse_ih)() {
  get_mouse_data();
}

/**
 * @brief Retrieves mouse data from the keyboard controller.
 *
 * @return 0 on success, non-zero otherwise.
 */
int(get_mouse_data)() {
  for (int i = 0; i < 5; i++) {
    aux_sys_inb(KBC_STATUS_REG, &st);
    if ((st & FULL_OUT_BUFFER) && (st & MOUSE_DATA)) {
      if ((st & (TIMEOUT_ERROR | PARITY_ERROR)) != 0)
        continue;
      else {
        aux_sys_inb(KBC_OUT_CMD, &data_m);
        return 0;
      }
    }
    // tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

/**
 * @brief Processes mouse data and updates the packet.
 *
 * @param data The mouse data to process.
 * @param counter Pointer to the counter variable.
 * @param pack Pointer to the packet structure to update.
 * @return 0 on success, non-zero otherwise.
 */
int(process_mouse_data_t)(uint8_t data, int *counter, struct packet *pack) {
  if ((*counter == 1) && (data & BIT(3))) {
    pack->bytes[0] = data;
    pack->rb = (bool) (data & RB);
    pack->lb = (bool) (data & LB);
    pack->mb = (bool) (data & MB);
    pack->x_ov = (bool) (data & X_OVERFLOW);
    pack->y_ov = (bool) (data & Y_OVERFLOW);
    (*counter)++;
  }
  else if (*counter == 2) {
    pack->bytes[1] = data;
    pack->delta_x = (pack->bytes[0] & MSB_X_DELTA) ? (data | 0xFF00) : data;
    (*counter)++;
  }
  else if (*counter == 3) {
    pack->bytes[2] = data;
    pack->delta_y = (pack->bytes[0] & MSB_Y_DELTA) ? (data | 0xFF00) : data;
    *counter = 1;
    // mouse_print_packet(pack);
  }
  else {
    return 1;
  }
  return 0;
}
