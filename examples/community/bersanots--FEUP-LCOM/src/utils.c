#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <stdint.h>

int counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val & 0xff);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t aux;
  aux = val & 0xff00;
  *msb = (uint8_t)(aux >> 8);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t byte;
  sys_inb(port, &byte);
  *value = (uint8_t) byte;
  counter++;
  return 0;
}
