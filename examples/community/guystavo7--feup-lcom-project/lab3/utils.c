#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  
  *lsb=(0x00FF&val);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  
  *msb=(0xFF00&val)>>8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  u32_t lol;
  sys_inb(port,&lol);//?
  *value=(u8_t)lol;
  return 0;
}
