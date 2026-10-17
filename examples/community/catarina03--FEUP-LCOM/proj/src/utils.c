#include <lcom/lcf.h>

#include <stdint.h>

int count =0;


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  val=val<<8;
  val=val>>8;
  *lsb=(uint8_t)val;
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val=val>>8;
  *msb=(uint8_t)val;

  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t tmp;
  int result;
  result=sys_inb(port,&tmp);
  *value=(uint8_t)tmp;
  #ifdef LAB3
  count++;
  #endif
  return result;
	
}
