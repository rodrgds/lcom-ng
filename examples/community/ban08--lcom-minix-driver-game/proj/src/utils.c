#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"

extern uint8_t scan_code;

/**
 * @brief Retrieves the least significant byte (LSB) of a 16-bit value.
 * 
 * @param val The 16-bit value
 * @param lsb Pointer to store the LSB
 * @return 0 on success, 1 on failure (e.g., null pointer)
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & 0xFF;
  return 0;
}

/**
 * @brief Retrieves the most significant byte (MSB) of a 16-bit value.
 * 
 * @param val The 16-bit value
 * @param msb Pointer to store the MSB
 * @return 0 on success, 1 on failure (e.g., null pointer)
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

/**
 * @brief Reads a byte from an I/O port using sys_inb and returns only the least significant byte.
 * 
 * @param port The I/O port number
 * @param value Pointer to store the read byte
 * @return 0 on success, non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;

  uint32_t v;
  int r = sys_inb(port, &v);
  *value = v & 0xFF; // Mask out only the LSB of the returned value
  return r;
}

/**
 * @brief Waits for the ESC break code to be pressed on the keyboard.
 * 
 * Subscribes to keyboard interrupts, reads scan codes until ESC break code is detected,
 * then unsubscribes.
 * 
 * @return 0 on success, 1 on failure
 */
int (was_ESC_pressed)() {
  int status;
  uint8_t kb_irq_mask;
  message msg;

  if (keyboard_subscribe_interruptions(&kb_irq_mask) != 0)
    return 1;

  while (scan_code != BREAK_CODE_ESCAPE) {
    if (driver_receive(ANY, &msg, &status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
        if (msg.m_notify.interrupts & kb_irq_mask) {
          kbc_ih();  // Keyboard interrupt handler
        }
      }
    }
  }

  if (keyboard_unsubscribe_interruptions() != 0)
    return 1;

  return 0;
}
