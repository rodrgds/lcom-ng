/**
 * @file utils.c
 * @brief Auxiliary function for the drivers programming.
 * 
 */

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief From a 2-byte value gets the least significant byte.
 * 
 * @param val 2-byte value
 * @param lsb contains the LSB from val
 * @return 0 if sucess,1 otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;

  *lsb = 0xFF & val;
  return 0;
}
/**
 * @brief From a 2-byte value gets the most significant byte.
 * 
 * @param val 2-byte value
 * @param msb contains the MSB from val
 * @return 0 if sucess,1 otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  
  *msb = val >> 8;
  return 0;
}
/**
 * @brief Allows the use of the function sys_inb but for values with 1 byte.
 * 
 * @param port number of the port where the value is coming from 
 * @param value container that will receive the value retrieved from the port
 * @return status, 0 if sucess, otherwise failure
 */
int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t temp;

  int status = sys_inb(port, &temp);
  *value = 0xFF & temp;
  return status;
}
