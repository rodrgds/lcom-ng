#include <lcom/lcf.h>
#include <stdint.h>

uint32_t sys_calls_KBC = 0;

/*
@brief Function that reads the least significant byte of a 16-bit variable
@param val 16-bit variable
@param lsb pointer to the variable where the least significant byte will be stored
@return 0 upon success, 1 upon failure
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = 0xFF & val;
  return 0;
}
/*
@brief Function that reads the most significant byte of a 16-bit variable
@param val 16-bit variable
@param msb pointer to the variable where the most significant byte will be stored
@return 0 upon success, 1 upon failure
*/
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (val >> 8) & 0xFF;
  return 0;
}

/*
@brief Function that reads the LSB from a 32 bit variable
@param port port to read from
@param value pointer to the variable where the byte will be stored
@return 0 upon success, 1 upon failure
*/
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t value32;

  #ifdef LAB3
  sys_calls_KBC++;
  #endif

  if (sys_inb(port, &value32) == OK){
    *value = 0xFF & value32;
    return 0;
  }
  return 1;
}
