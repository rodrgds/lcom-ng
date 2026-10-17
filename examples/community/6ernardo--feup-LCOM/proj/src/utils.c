#include <lcom/lcf.h>

#include <stdint.h>
int cnt=0;
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  
  *lsb=(uint8_t) (val& 0x00FF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  *msb=((val>> 8) & 0x00FF);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
 
  uint32_t p= 0;
  int err= sys_inb(port,&p);// p = valor que port está a guardar
  if(err!=0){
    return 1;
  }
  *value=(uint8_t) p;
  
  cnt++;
  
  return 0;
}
