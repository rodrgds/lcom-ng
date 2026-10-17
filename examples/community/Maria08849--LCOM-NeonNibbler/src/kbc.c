#include "kbc.h"

/** @defgroup Keyboard
 * @{
 *
 * Functions related to the use of the keyboard in the project
 */

int count = 0;
int keyboardIRQ = IRQ_KEYBOARD;
uint16_t data;
uint8_t status;

/**
 * @brief Retrieves keyboard data from the keyboard controller.
 *
 * @return 0 on success, non-zero otherwise.
 */
int(get_keyboard_data)() {
  for (int i = 0; i < 3; i++) {
    aux_sys_inb(KBC_STATUS_REG, &status);
    if (status & FULL_OUT_BUFFER) {
      if (status & (TIMEOUT_ERROR | PARITY_ERROR))
        continue;
      else {
        aux_sys_inb_16(KBC_OUT_CMD, &data);
        break;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

/**
 * @brief Keyboard interrupt handler.
 */
void(kbc_ih)() {
  get_keyboard_data();
}

/**
 * @brief Reads the status register of the keyboard controller.
 *
 * @param status Pointer to the variable where the status will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(read_KBC_status)(uint8_t *status) {
  aux_sys_inb(KBC_STATUS_REG, status);
  return 0;
}

/**
 * @brief Reads the output buffer of the keyboard controller.
 *
 * @param data Pointer to the variable where the data will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(read_KBC_output)(uint16_t *data) {
  aux_sys_inb_16(KBC_OUT_CMD, data);
  return 0;
}

/**
 * @brief Writes a byte to the keyboard controller.
 *
 * @param port The port to write the byte to.
 * @param bt The byte to write.
 * @return 0 on success, non-zero otherwise.
 */
int(write_KBC)(uint8_t port, uint8_t bt) {
  uint8_t st;
  read_KBC_status(&st);
  if ((st & FULL_IN_BUFFER) == 0) {
    sys_outb(port, bt);
  }
  return 0;
}

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to the variable where the bit number of the keyboard IRQ will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(keyboard_subscribe_interrupts)(uint8_t *bit_no) {
  *bit_no = keyboardIRQ;
  sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboardIRQ);
  return 0;
}

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int(keyboard_unsubscribe_interrupts)() {
  sys_irqrmpolicy(&keyboardIRQ);
  return 0;
}

/**
 * @brief Processes and prints keyboard data.
 *
 * @param data The keyboard data to process and print.
 * @return 0 on success, non-zero otherwise.
 */
int(process_and_print_keyboard_data)(uint16_t data) {
  /* equals 0 if it's make code, otherwise 1 */
  int make;
  /* number of bytes in data */
  int no_of_bytes;
  /* most significant byte in data */
  uint8_t msB_data;
  /* first element is the first byte of data (if data has two bytes, this is 0xE0) and second element is second byte of data (if data has one byte, this is not set) */
  uint8_t bytes[2];

  msB_data = data >> 8;
  no_of_bytes = (msB_data == TWO_BYTES) ? 2 : 1;

  make = (data & MAKE_CODE) ? 0 : 1;

  if (no_of_bytes == 2) {
    bytes[0] = TWO_BYTES;
    bytes[1] = (uint8_t) data;
  }
  else
    bytes[0] = (uint8_t) data;

  kbd_print_scancode(make, no_of_bytes, bytes);
  return 0;
}

/**
 * @brief Restores the keyboard to its default state.
 *
 * @param cmd The command to write to the keyboard controller.
 * @return 0 on success, non-zero otherwise.
 */
int(keyboard_restore)(uint8_t cmd) {
  cmd &= ~BIT(5) & ~BIT(4);
  // cmd &= (BIT(7) | BIT(6) | BIT(3) | BIT(2));
  cmd |= BIT(0) | BIT(1);
  if (write_KBC(KBC_STATUS_REG, KBC_WRITE_CMD) != 0)
    return 1;
  if (write_KBC(KBC_OUT_CMD, cmd) != 0)
    return 1;
  return 0;
}
