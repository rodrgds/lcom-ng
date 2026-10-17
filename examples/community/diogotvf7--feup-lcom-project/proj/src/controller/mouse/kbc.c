#include "kbc.h"

int(read_mouse_status)(uint8_t *st) {
  if (util_sys_inb(KBC_STAT_REG, st) != OK) {
    printf("Error: Unable to read status!\n");
    return EXIT_FAILURE;
  }
  if (*st & PARITY_ERROR) {
    printf("Error: Parity error!\n");
    return EXIT_FAILURE;
  }
  if (*st & TIMEOUT_ERROR) {
    printf("Error: Timeout error!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(read_output_buffer)(uint8_t *out) {
  int attempts_left = ATTEMPTS;
  uint8_t status;
  while (attempts_left--) {
    if (read_mouse_status(&status) != OK) return EXIT_FAILURE;
    if (status & OBF) {
      if (util_sys_inb(KBC_OUT_BUF, out) != OK) {
        tickdelay(micros_to_ticks(DELAY_US));
        continue;
      }
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return EXIT_FAILURE;
}

int(mouse_write)(int port, uint8_t cmd) {
  int attempts_left = ATTEMPTS;
  uint8_t status;

  while (attempts_left--) {
    if (read_mouse_status(&status) != OK)
      return EXIT_FAILURE;
    if ((status & IBF) == OK) {
      if (sys_outb(port, cmd) != OK)
        return EXIT_FAILURE;
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return EXIT_FAILURE;
}
