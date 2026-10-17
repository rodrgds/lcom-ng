#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // Verifying input validity!
  
  *lsb = (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1; // Verifying input validity!

  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1; // Verifying input validity!

  uint32_t read_value;
  int status = sys_inb(port, &read_value);

  if (status != 0) return 1;
  *value = (uint8_t)read_value;

  return 0;
}
