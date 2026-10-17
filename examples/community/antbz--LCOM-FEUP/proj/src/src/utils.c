#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
int cnt = 0; // Variable for counting sys_inb calls
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val&0xff);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val>>8);
  return 0;
}

#ifdef LAB3
void sys_inb_cnt() {
  cnt++;
}
#endif

int (util_sys_inb)(int port, uint8_t *value) {
  u32_t newvalue;
  int success = sys_inb(port, &newvalue);
  *value = newvalue;
  #ifdef LAB3
  sys_inb_cnt();
  #endif
  return success;
}
