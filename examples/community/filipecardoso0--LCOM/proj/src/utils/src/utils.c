#include "../include/utils.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0xFF;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val & 0xFF00) >> 8;
  
  return 0;
}

int (util_sys_inb)(int port, uint8_t* value) {

  uint32_t read;
  if (sys_inb(port, &read)) return 1;
  *value = (uint8_t) read;
  return 0;
}
