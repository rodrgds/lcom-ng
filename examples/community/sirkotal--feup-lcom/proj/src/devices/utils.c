#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t lsb_mask = 0x00FF;

  *lsb = val & lsb_mask;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t msb_mask = 0xFF00;

  *msb = (val & msb_mask) >> 8;

  return 0;
}

#ifdef LAB3
  extern uint32_t sys_count;
#endif

int (util_sys_inb)(int port, uint8_t *value) {
  
  #ifdef LAB3
    sys_count++;
  #endif
  
  uint32_t result;
  int status = sys_inb(port, &result);
  if (status == 0) *value = (uint8_t) result;
  return status;
}
