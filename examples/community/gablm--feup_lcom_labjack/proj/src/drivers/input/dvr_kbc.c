#include "input.h"

int kbc_read_status(uint8_t *output) {
  if (output == NULL)
    return 1;

  return util_sys_inb(STAT_REG, output);
}

int kbc_read_output(uint8_t port, uint8_t *output, bool mouse) {
  uint8_t status;
  int attemps = KBC_MAX_TRIES;

  while (attemps > 0) {
    if (kbc_read_status(&status))
      return 1;

    if (status & FULL_OUTPUT) {
      if (util_sys_inb(OUT_BUF, output))
        return 1;

      if (!mouse && (status & MOUSE_BIT))
        return 1;

      if (mouse && !(status & MOUSE_BIT))
        return 1;

      return (status & PARITY_BIT || status & TIMEOUT_BIT);
    }

    attemps--;
    tickdelay(micros_to_ticks(20000));
  }

  return 1;
}

int kbc_write_input(uint8_t port, uint8_t input) {
  uint8_t status;
  int attemps = KBC_MAX_TRIES;

  while (attemps > 0) {
    if (kbc_read_status(&status))
      return 1;

    if (!(status & FULL_INPUT))
      return sys_outb(port, input);

    attemps--;
    tickdelay(micros_to_ticks(20000));
  }

  return 1;
}

int kbc_write(uint8_t intent, bool mouse) {
  uint8_t command;
  uint8_t attemps = KBC_MAX_TRIES;
  uint8_t response;

  if (mouse) {
    while (attemps) {
      if (kbc_write_input(STAT_REG, WRITE_CMD_MOUSE))
        return 1;

      if (kbc_write_input(OUT_BUF, intent))
        return 1;

      tickdelay(micros_to_ticks(DELAY_US));

      if (util_sys_inb(OUT_BUF, &response))
        return 1;

      if (response == 0xFA)
        return 0;

      attemps--;
    }
  }
  else {
    if (kbc_write_input(STAT_REG, READ_CMD))
      return 1;

    if (kbc_read_output(OUT_BUF, &command, mouse))
      return 1;

    command |= intent;

    if (kbc_write_input(STAT_REG, WRITE_CMD))
      return 1;

    return kbc_write_input(OUT_BUF, command);
  }
  return 1;
}
