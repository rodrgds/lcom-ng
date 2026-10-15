#include <lcom/lcf.h>

#include "controller/kbc/kbc.h"

int(kbc_ready)(uint8_t st) {
  // checks if the parity err or timeour err bits are set to 1
  return !((st) & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR));
}

int(kbc_read_st)(uint8_t *st) {
  if (st == NULL) {
    fprintf(stderr, "kbc_read_st: st pointer cannot be null.");
    return 1;
  }
  if (util_sys_inb(KBC_ST, st) != 0) { // reads the value stored at the status register
    fprintf(stderr, "kbc_read_st: failed to read the value stored at the status register.");
    return 1;
  }

  return 0;
}

int(kbc_read_data)(uint8_t *data, bool expect_mouse_data) {
  if (data == NULL) {
    fprintf(stderr, "kbc_read_data: data pointer cannot be null.");
    return 1;
  }

  uint8_t st = 0, attempts = KBC_MAX_ATTEMPTS;

  while (attempts > 0) {
    if (kbc_read_st(&st) != 0) { // reads the KBC status
      fprintf(stderr, "kbc_read_data: failed to read the kbc status.");
      return 1;
    }

    // checks if the "output buffer full" bit is set to 1
    if (st & KBC_FULL_OBF) {
      // effectivelly reads the value stored at the output buffer
      // checks for erros in the KBC
      if (!kbc_ready(st)) {
        fprintf(stderr, "kbc_read_data: KBC not ready.");
        return 1;
      }
      // when not expecting mouse data, bit 5 must NOT be set
      if (!expect_mouse_data && (st & KBC_AUX)) {
        fprintf(stderr, "kbc_read_data: expected keyboard data, but received data from the mouse.");
        return 1;
      }
      if (util_sys_inb(KBC_OUT, data) != 0) {
        fprintf(stderr, "kbc_read_data: failed to read the kbc buffer.");
        return 1;
      }
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }

  fprintf(stderr, "kbc_read_data: failed to read the kbc data.");
  return 1;
}

int(kbc_write_cmd)(int port, uint8_t cmd) {
  uint8_t st = 0, attempts = KBC_MAX_ATTEMPTS;

  /* both the KBC and the keyboard may take some time to respond to a command
   * thus this "driver" should not expect to get a response immediately after issuing a command
   * we need to give enough-time for the KBC to respond, retry a few times on time-out, and finally give up */
  while (attempts > 0) {
    if (kbc_read_st(&st) != 0) { // reads the kbc status
      fprintf(stderr, "kbc_write_cmd: failed to read the kbc status.");
      return 1;
    }

    if (!kbc_ready(st)) {
      fprintf(stderr, "kbc_write_cmd: KBC not ready.");
      return 1;
    }

    // checks if the input buffer is NOT alredy full
    if (!(st & KBC_FULL_IBF)) {
      // effectivelly sends the command byte to the specified port
      if (sys_outb(port, cmd) != 0) {
        fprintf(stderr, "kbc_write_cmd: failed to read the kbc port.");
        return 1;
      }

      return 0;
    }

    // waits (the cpu is way faster than the kbc)
    tickdelay(micros_to_ticks(DELAY_US));
    attempts--; // decreases the timeout counter
  }

  fprintf(stderr, "kbc_write_cmd: failed to write the kbc command.");
  return 1;
}
