/**
 * @file utils.c
 * @brief Utilities for driver functions
 */

#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Retrieves the least significant byte (LSB) of a 16-bit value.
 *
 * @param val The 16-bit value from which to extract the LSB.
 * @param lsb Pointer to a variable where the LSB will be stored.
 * @return 0 upon success, 1 if lsb is NULL.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if( lsb==NULL) return 1;
  *lsb= val & 0xFF;

  return 0;
}

/**
 * @brief Retrieves the most significant byte (MSB) of a 16-bit value.
 *
 * @param val The 16-bit value from which to extract the MSB.
 * @param msb Pointer to a variable where the MSB will be stored.
 * @return 0 upon success, 1 if msb is NULL.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if( msb==NULL) return 1;
  val=val >>8;
  *msb=(uint8_t)val;
  return 0;

}

/**
 * @brief Reads a byte from a specified port.
 *
 * @param port The I/O port from which to read the byte.
 * @param value Pointer to a variable where the read value will be stored.
 * @return 0 upon success, 1 if value is NULL.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if( value==NULL) return 1;
  uint32_t temp;
  int res=sys_inb(port,&temp);
  util_get_LSB(temp, value);
  return res;
}
