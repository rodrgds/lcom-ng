#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.h"


uint32_t cnt = 0;
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb= (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb= (uint8_t)(val>>8); 
  return 0;
}

int util_sys_inb(int port, uint8_t *val){
    uint32_t value;
    sys_inb(port, &value);
    #ifdef LAB3
    cnt++;
    #endif
    *val = (uint8_t)value;
    return 0;
}


