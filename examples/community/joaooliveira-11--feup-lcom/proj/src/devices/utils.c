#include <lcom/lcf.h>
#include <stdint.h>

uint32_t counter_sysinb = 0;

int(util_get_LSB)(uint16_t value, uint8_t * lsb_byte){
  if(lsb_byte== NULL) return 1;
  *lsb_byte = value & 0x00FF;
  return 0;
} 

int(util_get_MSB)(uint16_t value, uint8_t *msb_byte){
  if(msb_byte == NULL) return 1;
  *msb_byte = (value >> 8) & 0x00FF;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *st){
  if(st == NULL) return 1;
  uint32_t value;
  if(sys_inb(port, &value) != 0) return 1;
  counter_sysinb++;
  *st = value & 0xFF;
  return 0;
}
