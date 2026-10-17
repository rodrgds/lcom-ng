#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL)
    return 1;

  *lsb = val & 0xFF; // 0xFF = 00000 0000 1111 1111 , selects only the first 8 bits

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL)
    return 1;

  *msb = (val >> 8) & 0xFF; // shifts the value 8 bits to the right and selects only the first 8 bits

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  /* Input validation */
  if(value == NULL)
    return 1;

  uint32_t value32 = 0x00;
  int ret_value = sys_inb(port, &value32);

  /* O fabio sa retira os ultimos 8 bits a 1 (0xFF & value32) */ 
  *value = value32;

  return ret_value;
}
