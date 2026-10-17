#include <lcom/lcf.h>

#include "utils.h"

#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t mask = 0x00ff;
  val &= mask;
  *lsb = (uint8_t) val;
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t mask = 0xff00;
  val &= mask;
  val = val >> 8;
  *msb = (uint8_t) val;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t* value_32 = (uint32_t*) malloc(sizeof(uint32_t));
  if (sys_inb(port, value_32) != 0)
    return 1;

  uint32_t mask = 0x000000ff;
  *value = (uint8_t) (mask & *value_32);
  return 0;
}
