#include "func_aux.h"


int (aux_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val & 0xFF);
  return 0;
}

int (aux_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)((val & 0xFF00) >> 8);
  return 0;
}

int (aux_sys_inb)(int port, uint8_t *value) {
  uint32_t u32 = 0x00;
  if (sys_inb(port, &u32) != OK) return 1;
  *value = (uint8_t) (u32 & 0xFF);
  return 0;
}

