#include "kbc.h"



int(kbc_read_output)(uint8_t *scancode, uint8_t aux_mouse_bit) {
  uint8_t status;
  int attempts = 10;

  while (attempts > 0) {

    // Read status
    if (util_sys_inb(KBC_STATUS_REG, &status))
      return 1;

    // Check if output buffer is full
    if (status & KBC_STAT_OUT_BUFF_FULL) {

      // Read scancode
      if (util_sys_inb(KBC_OUT_BUF, scancode))
        return 1;

      // Check if scancode is valid
      if ((status & (KBC_STAT_PARITY_ERR | KBC_STAT_TIMEOUT_ERR)))
        return 1;

      if (aux_mouse_bit && !(status & KBC_STAT_AUX_MOUSE)) {
        printf("aux_mouse_bit: %d\n", aux_mouse_bit);
        return 1;
      }

      if (!aux_mouse_bit && (status & KBC_STAT_AUX_MOUSE)) {
        printf("aux_mouse_bit: %d\n", aux_mouse_bit);
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }
  return 1;
}

int(kbc_write_command)(uint8_t port, uint8_t commandByte) {
  uint8_t status;
  int attemps = 10;

  while (attemps > 0) {
    if (util_sys_inb(KBC_STATUS_REG, &status)) {
      return 1;
    }

    /* loop while 8042 input buffer is not empty */
    if ((status & KBC_STAT_IN_BUFF_FULL) == 0) {
      sys_outb(port, commandByte); /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
    attemps--;
  }

  return 1;
}
