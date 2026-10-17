#include <lcom/lcf.h>
#include <stdint.h>


/// @brief gets the least significant byte from 16 bit varibale
/// @param val  16 bit variabale
/// @param lsb least significant byte read
/// @return  0 in case of success, 1 otherwise
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; 
  *lsb = 0xFF & val;         
  return 0;
}

/// @brief  gets themost significant byte from 16 bit varibale
/// @param val 16 bit variabale
/// @param msb most significant byte read
/// @return 0 in case of success, 1 otherwise
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

/// @brief wrapper to the sys_inb system call, fetches an 8 bits varibale instead of a 32 bits one
/// @param port register to read from
/// @param value value read
/// @return 0 in case of success, 1 otherwise
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;   
  uint32_t val;                   
  int ret = sys_inb(port, &val);  
  *value = 0xFF & val;           
  return ret;
}
