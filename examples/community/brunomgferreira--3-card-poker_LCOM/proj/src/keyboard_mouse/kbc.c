#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"

int(kbc_write_command)(uint8_t port, uint8_t cmd) {

  int tries = TRIES;
  uint8_t stat;

  while (tries > 0) {
    if (util_sys_inb(KBC_STAT_REG, &stat))
      return 1;
    /* loop while 8042 input buffer is not empty */
    if ((stat & IBF) == 0) {
      if (sys_outb(port, cmd))
        return 1;
      return 0;
    }
    tries--;
    tickdelay(micros_to_ticks(DELAY)); // e.g. tickdelay()
  }
  return 1;
}

int(kbc_read_output)(uint8_t port, uint8_t *cmd, uint8_t mouse) {
  int tries = TRIES;
  uint8_t stat;

  while (tries > 0) {
    if (util_sys_inb(KBC_STAT_REG, &stat))
      return 1;
    if (stat & OBF) {

      if (util_sys_inb(port, cmd))
        return 1;

      if (stat & (PARITY_ERR | TIMEOUT_ERR))
        return 1;

      if((mouse && !(stat & AUX)) || (!mouse && (stat & AUX)))
        return 1;

      return 0;
    }
    tries--;
    tickdelay(micros_to_ticks(DELAY));
  }
  return 1;
}
