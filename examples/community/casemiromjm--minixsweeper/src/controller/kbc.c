#include "kbc.h"

int (read_KBC_state)(uint8_t* status) {

  if (status == NULL) return 1;

  if (util_sys_inb(KBC_STATUS_REG, status) != 0) return 1;

  return 0;
}

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse) {

  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while (attempts > 0) {

    if (read_KBC_state(&status) != 0) {
      printf("Error reading KBC state\n");
      return 1;
    }

    // can only read kb controller when the output buffer is full
    if ( (status & KBC_OB_FULL) != 0 ) {

      // by reading it first, we assure it will be discarded when needed
      if (util_sys_inb(port, output) != 0) return 1;
      
      // check parity error
      if ( (status & KBC_PARITY_ERROR) != 0) {
        return 1;
      }

      // check timeout error
      if ( (status & KBC_TIMEOUT_ERROR) != 0) {
        return 1;
      }

      // wants to check mouse output, but there's nothing
      if (mouse && !(status & KBC_AUX)) {
        return 1;
      }

      // wants to check keyboard output, but there's mouse data
      if (!mouse && (status & KBC_AUX)) {
        return 1;
      }

      // if no error occurs, it was successfully read
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }

  // failed to read in the determined time
  return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {

  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while (attempts > 0) {

    if (read_KBC_state(&status) != 0) {
      printf("Error reading the KBC state\n");
      return 1;
    }

    // can only write a command if input buffer is not full
    if ( (status & KBC_IN_BUF) == 0) {
      if (sys_outb(port, commandByte) != 0) {
        printf("Error writing KBC command byte\n");
        return 1;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }

  return 1;
}

