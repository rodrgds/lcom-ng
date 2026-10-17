#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Function that fetches the least significant byte of a 16-bit value.
 * @param val Value to extract the least significant byte from.
 * @param lsb Variable to store the least significant byte.
 * @return 0 upon success, 1 otherwise.
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = (uint8_t) val;
  return 0;
}

/**
 * @brief Function that fetches the most significant byte of a 16-bit value.
 * @param val Value to extract the most significant byte from.
 * @param msb Variable to store the most significant byte.
 * @return 0 upon success, 1 otherwise.
*/
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (uint8_t) (val >> 8);
  return 0;
}

/**
 * @brief Function that fetches a 8-bit value instead of a 32-bit value, using sys_inb().
 * @param port Port to read from.
 * @param value Variable to store the value read.
 * @return 0 upon success, 1 otherwise.
*/
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t auxValue;
  if(sys_inb(port, &auxValue)) return 1;
  *value = (uint8_t) auxValue;
  return 0;
}
