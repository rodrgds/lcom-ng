#include "kbc.h"
#include "i8042.h"
#include <lcom/lcf.h>

int (kbc_read_status)(uint8_t *status) {
  if (util_sys_inb(KBC_STATUS_REG, status) != 0) return 1;
  return 0;
}

int (kbc_read_data)(uint8_t port, uint8_t *data, uint8_t mouse) {
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while (attempts > 0) {
    if (kbc_read_status(&status) != 0) return 1;

    if ((status & KBC_OBF) != 0) {
      if (util_sys_inb(port, data) != 0) return 1;
      if ((status & KBC_PAR_ERR) != 0) return 1;
      if ((status & KBC_TO_ERR) != 0) return 1;
      if (mouse && !(status & KBC_AUX)) return 1;
      if (!mouse && (status & KBC_AUX)) return 1;
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }

  return 1;
}

int (kbc_read_data_ih)(uint8_t *data, uint8_t mouse) {
  if (data == NULL) return 1;
  uint8_t status;
  if (kbc_read_status(&status) != 0) return 1;

  if ((status & KBC_OBF) == 0) return 1;

  if (util_sys_inb(KBC_OUT_BUF, data) != 0) return 1;
  if ((status & KBC_PAR_ERR) != 0) return 1;
  if ((status & KBC_TO_ERR) != 0) return 1;
  if (mouse && !(status & KBC_AUX)) return 1;
  if (!mouse && (status & KBC_AUX)) return 1;

  return 0;
}


int (kbc_write_command)(uint8_t port, uint8_t command_byte) {
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while (attempts > 0) {
    if (kbc_read_status(&status) != 0) return 1;

    if ((status & KBC_IBF) == 0) {
      if (sys_outb(port, command_byte) != 0) return 1;
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts--;
  }

  return 1;
}
