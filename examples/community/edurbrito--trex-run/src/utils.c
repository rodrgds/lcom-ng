#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include "i8254.h"

#ifdef LAB3
uint32_t sys_inb_counter;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  // Example : val = 1234 which is 0x04D2 ---> lsb = 0xD2 or 210 in decimal
  // 0000 0100 1101 0010 --> (8-bit) 1101 0010
  
  uint16_t mask = 0x00FF;
  uint8_t result = (uint8_t) (val & mask);
  *lsb = result;

  return OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  // Example : val = 1234 which is 0x04D2 ---> msb = 0x04 
  // 0000 0100 1101 0010 --> (8-bit) 0000 0100

  uint8_t result = (uint8_t) (val >> 8);
  *msb = result;
  
  return OK;
}

int (util_sys_inb)(int port, uint8_t *value) {
  
  uint32_t var;
  
  #ifdef LAB3
  sys_inb_counter++;
  #endif  
  if(sys_inb(port,&var) == OK){
    *value = (uint8_t) (var & 0xFF);
    return OK;
  }

  return NOT_OK;
}
