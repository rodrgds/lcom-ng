#include <lcom/lcf.h>
#include <stdint.h>

// Extracts the Least Significant Byte (LSB) from a 16-bit value.
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL) return 1; // 1 = error
  *lsb = 0xFF & val;
  return 0;
}

// Extracts the Most Significant Byte (MSB) from a 16-bit value.
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL) return 1; // 1 = error
  *msb = (0xFF00 & val) >> 8;
  return 0;
}


// if we use the address of a uint8_t variable, 
// the program may terminate abnormally, because it 
// accesses a memory position at an invalid address 
// (thus triggering a segmentation violation signal)
// for that reason, we use util_sys_inb
int (util_sys_inb)(int port, uint8_t *value) {
  if (value==NULL) return 1;   // 1 = error
  uint32_t val;
  int ret = sys_inb(port, &val);
  if(ret !=0) return ret; // error
  *value = 0xFF & val;
  return ret;
}
