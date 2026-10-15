#include "utils.h"

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;

  *lsb = val & 0xFF;
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;

  *msb = (val & 0xFF00) >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
    return 1;

  uint32_t res;
  int retv = sys_inb(port, &res);

  *value = (uint8_t) res;
  return retv;
}
