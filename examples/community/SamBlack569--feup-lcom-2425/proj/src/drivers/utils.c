#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  //add case where val is not valid?

  *lsb = val & 0x00FF;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    //add case where val is not valid?

  uint8_t temp_val = val >> 8;
  *msb = temp_val & 0x00FF;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t temp_val;
  sys_inb(port, &temp_val);
  *value = 0x00FF & temp_val;

  return 0;
}
