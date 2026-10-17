#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 255;
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = 255 & (val >> 8);

  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value_32 = 0;

  if(sys_inb(port, &value_32))
  {
    return 1;
  }
  *value = (uint8_t) value_32;
  return 0;
}
