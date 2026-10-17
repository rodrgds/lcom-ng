#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
uint32_t sysCounter = 0;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0x00FF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) 
{
  #ifdef LAB3
  sysCounter++;
  #endif
  uint32_t new;
  if(sys_inb(port, &new)!= 0) return 1;
  *value = new & 0xFF;
  return 0;
}


