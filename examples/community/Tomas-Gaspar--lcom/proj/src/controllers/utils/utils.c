#include <lcom/lcf.h>
#include "utils.h"

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;

  *lsb = (uint8_t)val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;

  *msb = (uint8_t)(val >> 8);

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value32;

  if (value == NULL || sys_inb(port, &value32)) return 1;

  *value = (uint8_t)value32;

  return 0;
}

int bcd_to_decimal(uint8_t bcd) {
  return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}
