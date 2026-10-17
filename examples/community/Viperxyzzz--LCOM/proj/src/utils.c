#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(!lsb)
    return 1;
  *lsb = (uint8_t)val;
  
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (!msb)
    return 1;
  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t newValue = 0;
  if (sys_inb(port,&newValue) == 0)
  {
    *value = (uint8_t)newValue;
    return 0;
  }
   return 1;
}
