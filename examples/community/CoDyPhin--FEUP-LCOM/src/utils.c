#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    *lsb = (val & 0x00FF); // selects the LSByte
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    *msb = ((val & 0xFF00)>>8); // selects the MSByte and shifts it to the LSB positions
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) { // =sys_inb but for parameters of type uint8_t
  uint32_t tmpval;
  if(sys_inb(port, &tmpval)) return 1;
  *value = tmpval;
  return 0;
}
