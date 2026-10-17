#include <lcom/lcf.h>

#include <stdint.h>
#ifdef LAB3
uint32_t counter=0;
#endif
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0xFF;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >>8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  u_int32_t big ;
  if(sys_inb(port, &big)!=0){return 1;}
  *value =(uint8_t) big;
  return 0;
}

