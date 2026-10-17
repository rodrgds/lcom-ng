#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Gets the Least Significant Byte (LSB) of a 16-bit value.
 *
 * Extracts the least significant byte (the lower 8 bits) 
 * from a given 16-bit value and stores it in the provided pointer.
 *
 * @param val The 16-bit value to extract the LSB from.
 * @param lsb Pointer to a variable where the LSB will be stored.
 * @return success, 1 if the pointer is NULL.
 */

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL) return 1;
  *lsb = (uint8_t) val;

  return 0;
}

/**
 * @brief Gets the Most Significant Byte (MSB) of a 16-bit value.
 *
 * Shifts the given 16-bit value 8 bits to the right,
 * isolating the most significant byte, and stores it in the provided pointer.
 *
 * @param val 16-bit value to extract the MSB from.
 * @param msb Pointer to a variable where the MSB will be stored.
 * @return 0 success, 1 if the pointer is NULL.
 */

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  if(msb == NULL) return 1;
  val = (val >> 8);
  *msb = (uint8_t) val;
  return 0;
}

/**
 * @brief Reads a byte from the specified I/O port.
 *
 * Wraps the `sys_inb` call, which reads 4 bytes from an I/O port,
 * and stores only the least significant byte (LSB) of that value into the provided pointer.
 *
 * @param port The I/O port to read from.
 * @param value Pointer to a variable where the read byte will be stored.
 * @return 0 success, non-zero failure (including if `value` is NULL).
 */

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  if ( value == NULL) return 1;
  uint32_t temp;
  int ret = sys_inb(port, &temp);
  *value = (uint8_t) temp;

  return ret;
}
