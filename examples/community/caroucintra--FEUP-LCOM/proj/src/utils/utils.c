#include <lcom/lcf.h>

#include <stdint.h>

uint32_t cnt = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t mask = 0x00ff;
  val &= mask;
  *lsb = (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t mask = 0xff00;
  val &= mask;
  val = val >> 8;
  *msb = (uint8_t)val;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  if (sys_inb(port, &temp)) return 1;
  *value = (uint8_t) temp;
  #ifdef LAB3
    cnt++;
  #endif
  return 0;
}
