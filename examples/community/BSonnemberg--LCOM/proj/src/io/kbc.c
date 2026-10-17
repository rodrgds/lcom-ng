#include <lcom/lcf.h>

#include "kbc.h"

int kbc_get_status(uint8_t *st) {
  if (st == NULL) {
    return 1;
  }
  if (util_sys_inb(KBC_ST_REG, st) != 0) {
    return 2;
  }
  return 0;
}

int kbc_read_outbuf(uint8_t *buf) {
  int tries = 5;
  while (tries != 0) {
    // get status
    uint8_t st;
    if (kbc_get_status(&st) != 0) {
      return 2;
    }
    // wait until obf
    if (st & KBC_ST_OBF) {
      if (util_sys_inb(KBC_OUTBUF, buf) != 0) {
        return 2;
      }
      // check for errors
      if (st & KBC_ST_ERR) {
        return 3;
      }
      return 0;
    }
    tickdelay(DELAY_TICKS);
    tries--;
  }
  return TIMEOUT;
}

int kbc_write(uint8_t port, uint8_t byte) {
  int tries = 5;
  while (tries != 0) {
    uint8_t st;
    if (kbc_get_status(&st) != 0) {
      return 2;
    }
    // wait until inbuf empty
    if ((st & KBC_ST_IBF) == 0) {
      return sys_outb(port, byte);
    }
    tickdelay(DELAY_TICKS);
    tries--;
  }
  return TIMEOUT;
}
