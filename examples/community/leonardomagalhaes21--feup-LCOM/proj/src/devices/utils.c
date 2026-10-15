/**
 * @file timer.c
 * @brief Contains functions for handling timer operations and interactions.
 */

#include <lcom/lcf.h>

#include <stdint.h>



/**
 * @brief Extracts the Least Significant Byte (LSB) from a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param lsb Pointer to the variable that will store the LSB.
 * @return 0 on success, 1 on failure.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb == NULL) return 1;

  *lsb = val & 0x00FF;
  return 0;   
}


/**
 * @brief Extracts the Most Significant Byte (MSB) from a 16-bit value.
 *
 * @param val The 16-bit value.
 * @param msb Pointer to the variable that will store the MSB.
 * @return 0 on success, 1 on failure.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb == NULL) return 1;

  *msb = (val & 0xFF00) >> 8;
  return 0;
}


/**
 * @brief Reads a byte from the specified port.
 *
 * @param port The port to read from.
 * @param value Pointer to the variable that will store the read value.
 * @return 0 on success, 1 on failure.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;
  uint32_t read;
  if(sys_inb(port, &read))
    return 1;
  *value = (read & 0x000000FF);
  return 0;
}
