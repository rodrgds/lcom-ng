#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Get the Least significant bits of a 16 bit value and store it in a 8 bit variable
 * @param val 16 bit value
 * @param lsb Address to store the 8 bit value
 * @return 0 if successful, 1 otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; 
  *lsb = val & 0xFF;
  return 0;
}

/**
 * @brief Get the Most significant bits of a 16 bit value and store it in a 8 bit variable
 * @param val 16 bit value
 * @param msb Address to store the 8 bit value
 * @return 0 if successful, 1 otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (val >> 8);
  return 0;
}

/**
 * @brief Transform 32 bit output in 8 bit output. Error prevention.
 * @param port Port to read
 * @param value Address to store the 8 bit value
 * @return 0 if successful, 1 otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t temp_val;
  int ret = sys_inb(port, &temp_val);
  *value =  0xFF & temp_val;
  return ret;
}
