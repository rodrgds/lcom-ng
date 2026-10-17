#include <lcom/lcf.h>
#include <stdint.h>


/**
 * @brief Gets the 8 least significant bits of a 16 bit value
 * 
 * @param val The 16 bit value
 * @param lsb The 8 least significant bits
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & 0xFF;
  return 0;
 

}


/**
 * @brief Gets the 8 most significant bits of a 16 bit value
 * 
 * @param val The 16 bit value
 * @param lsb The 8 most significant bits
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (val >> 8) & 0xFF;
  return 0;
}

/**
 * @brief Reads a byte from a certain port, with help from sys_inb()
 * 
 * @param port The port to read from
 * @param value The read byte
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t val;
  if (sys_inb(port, &val) != 0) return 1;
  *value = 0xFF & val;

  return 0;
}


