#include "i8042.h"
#include "kbc.h"

int (kbc_read_status)(uint8_t *status) {
  return util_sys_inb(KBC_STAT_REG, status);
}

int (kbc_read_output)(int port, uint8_t *output, bool is_mouse) {
  uint8_t status;

  for (int i = 0; i < ATTEMPTS; i++) {
    if (kbc_read_status(&status)) {
      printf("Error reading kbc status\n");
      return 1;
    }

    // If the output buffer is full we can read data
    if (status & KBC_STAT_OBF) {
      if (util_sys_inb(port, output)) {
        printf("Error reading kbc output\n");
        return 1;
      }

      if (status & KBC_STAT_PARITY) {
        printf("Parity error\n");
        return 1;
      }

      if (status & KBC_STAT_TIMEOUT) {
        printf("Timeout error\n");
        return 1;
      }

      if (is_mouse && !(status & KBC_STAT_AUX)) {
        printf("Mouse output not found\n");
        return 1;
      }

      if (!is_mouse && (status & KBC_STAT_AUX)) {
        printf("Keyboard output not found\n");
        return 1;
      }

      return 0;
    }

    // tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int (kbc_write_command)(int port, uint8_t command) {
  uint8_t status;

  for (int i = 0; i < ATTEMPTS; i++) {
    if (kbc_read_status(&status)) {
      printf("Error reading kbc status\n");
      return 1;
    }

    if ((status & KBC_STAT_IBF)) {
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }

    if (sys_outb(port, command)) {
      printf("Error writing kbc command\n");
      return 1;
    }

    return 0;
  }

  return 1;
}
