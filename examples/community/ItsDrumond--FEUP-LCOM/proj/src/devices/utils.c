#include <lcom/lcf.h>

#include <stdint.h>

#include "lcom/utils.h"

int count = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL){
    return 1;
  }
  *lsb = (uint8_t) val;
  return OK;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL){
    return 1;
  }
  *msb = (uint8_t) (val >> 8);
  return OK;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = *value;
  int flag = sys_inb(port, &val);
  *value = val;
  count++;
  return flag;
}

