#include "kbc_utils.h"

int (read_KBC_status)(uint8_t* status) {
  /* read the KBC status */
  if (util_sys_inb(KBC_STATUS_REG, status) != 0)
    return 1;
  return 0;
}

int (read_KBC_output)(uint8_t* output, uint8_t port, uint8_t mouse) {
  /* read the KBC output */
  uint8_t status;
  uint8_t att = MAX_ATT;

  while (att) {
    if (read_KBC_status(&status) != 0)
      return 1;

    /* output buf full so we can read from it */
    if ((status & OUTPUT_BUF_FULL) != 0) {
      if (util_sys_inb(KBC_OUT_BUF, output) != 0) {
        return 1;
      }

      if ((status & PARITY_ERROR) != 0) {
        return 1;
      }

      if ((status & TIMEOUT_ERROR) != 0) {
        return 1;
      }
      
      /* waiting for mouse output */
      if (mouse && !(status & MOUSE_DATA)) {
        return 1;
      }

      /* waiting for keyboard output */
      if (!mouse && (status & MOUSE_DATA)) {
        return 1;
      }

      return 0;
    }
    
    tickdelay(micros_to_ticks(WAIT_KBC));
    att--;
  }
  /* number of attempts exceeded*/
  return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t cmd) {
  uint8_t status;
  uint8_t att = MAX_ATT;

  while (att) {
    if (read_KBC_status(&status) != 0) {
      return 1;
    }

    /* input buf empty so we can write to it */
    if ((status & INPUT_BUF_FULL) == 0) {
      if (sys_outb(port, cmd) != 0) {
        return 1;
      }
      return 0;
    }
    
    tickdelay(micros_to_ticks(WAIT_KBC));
    att--;
  }

  return 1;
}

int (kbc_flush_input)() {
  uint8_t temp;
  while (read_KBC_output(&temp, KBC_OUT_BUF, 0) == 0) {
    /* flush the input buffer */
  }

  return 0;
}
