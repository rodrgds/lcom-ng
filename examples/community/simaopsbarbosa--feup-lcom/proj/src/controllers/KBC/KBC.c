#include "KBC.h"

int KBC_read_status(uint8_t *status) {
  return util_sys_inb(KBC_IN_CMD, status);
}

int KBC_read_output(uint8_t port, uint8_t *output, uint8_t mouse) {
  uint8_t statusRegister;
  uint8_t attempts = ATTEMPTS;

  while (attempts) {
    // read the status register
    if (KBC_read_status(&statusRegister) != 0) {
      printf("[ERROR] statusRegister not available");
      return 1;
    }

    // check if output buffer is full
    if ((statusRegister & BIT(0)) != 0) {
      // read the output buffer
      if (util_sys_inb(port, output) != 0) {
        printf("[ERROR] could not read output\n");
        return 1;
      }

      // check for parity error
      if ((statusRegister & BIT(7)) != 0) {
        printf("[ERROR] parity error\n");
        return 1;
      }
      // check for timeout error
      if ((statusRegister & BIT(6)) != 0) {
        printf("[ERROR] timeout error\n");
        return 1;
      }
      // validate if output is from mouse when expected
      if (mouse && !(statusRegister & BIT(5))) {
        printf("[ERROR] mouse output not found\n");
        return 1;
      }
      // validate if output is from keyboard when expected
      if (!mouse && (statusRegister & BIT(5))) {
        printf("[ERROR] keyboard output not found\n");
        return 1;
      }
      return 0;
    }

    // wait and retry
    tickdelay(micros_to_ticks(WAIT_KBC));
    attempts--;
  }

  return 1;
}

int KBC_write_command(uint8_t port, uint8_t commandByte) {
  uint8_t statusRegister;
  uint8_t attempts = ATTEMPTS;

  while (attempts) {
    // read the status register
    if (KBC_read_status(&statusRegister) != 0) {
      printf("[ERROR] status not available");
      return 1;
    }

    // check if input buffer is empty
    if ((statusRegister & BIT(1)) == 0) {
      // write the command byte
      if (sys_outb(port, commandByte) != 0) {
        printf("[ERROR] could not write commandByte\n");
        return 1;
      }
      return 0;
    }

    // wait and retry
    tickdelay(micros_to_ticks(WAIT_KBC));
    attempts--;
  }

  return 1;
}
