#include "KBC.h"

int (read_KBC_status)(uint8_t *status) {
  if(util_sys_inb(KBC_STATUS_REG, status)) return 1;
  return 0;
}

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse) {
  uint8_t st;
  int attemps = MAX_ATTEMPTS;
  while(attemps != 0) {
    if (read_KBC_status(&st)) return 1;
    if (st & OUTPOUT_BUF_FULL) {
      if (util_sys_inb(port, output)) return 1;
      if (st & PARITY_ERROR) {
        printf("PARITY ERROR \n");
        return 1;
      }
      if (st & TIMEOUT_ERROR) {
        printf("TIMEOUT ERROR \n");
        return 1;
      }
      if (mouse & !(st & BIT(5))) return 1;
      if (!mouse & (st & BIT(5))) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
    attemps--;
  }
  return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {
  uint8_t st;
  int attemps = MAX_ATTEMPTS;
  while (attemps != 0) {
    if (read_KBC_status(&st)) return 1;
    if ((st & INPUT_BUF_FULL) == 0) {
      if (sys_outb(port, commandByte)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
    attemps--;
  }

  return 1;
}
