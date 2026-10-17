#include "utils.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val >>= 8;
  *msb = (uint8_t) val;
  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t val;
  if(sys_inb(port, &val)) return false;
  *value = (uint8_t) val;
  return true;
}
