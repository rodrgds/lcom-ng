#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"

uint32_t cnt = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  val = val << 8;
  val = val >> 8;
  *lsb = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val = val >> 8;
  *msb = (uint8_t) val;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = 0x00;
  if (sys_inb(port, &val) == 0) {
    *value = (uint8_t) val;
    return 0;
  }
  return 1;
}

uint16_t convert_2_complement(uint8_t number, uint8_t msb, struct packet * pacote){
  if (pacote->bytes[0] & msb){
    return number - 256;
  }
  return number;
}

