#include <lcom/lcf.h>

#include <stdint.h>

uint32_t sysinb_count = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val & 0x00FF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val & 0xFF00) >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  sysinb_count++;
  if (sys_inb(port, &temp) != OK) {
    return 1;
  }
  *value = (uint8_t) temp;
  return 0;
}
