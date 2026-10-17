#include <lcom/lcf.h>

#include <stdint.h>

#define mask 0x00FF

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) {
    return 1;
  }
  *lsb = val & mask;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) {
    return 1;
  }
  val >>= 8;
  *msb = val & mask;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }
  uint32_t temp;
  int ret = sys_inb(port, &temp);
  *value = (uint8_t) temp;
  return ret;
}
