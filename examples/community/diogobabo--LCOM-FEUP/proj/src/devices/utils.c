#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t temp = 0x00FF;
  temp = temp & val;
  *lsb = (uint8_t) temp;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t temp = 0xFF00;
  temp = temp & val;
  temp = temp >> 8;
  *msb = (uint8_t) temp;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t val = 0;

  int in;

  in = sys_inb(port,&val);

  *value = (uint8_t)val;
  return in;
}
