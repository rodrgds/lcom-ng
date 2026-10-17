#include <lcom/lcf.h>

#include <stdint.h>

/** @defgroup utils utils
 * @{
 *
 * Auxiliar functions for the program.
 */


/**
 * @brief Gets the least significant byte (LSB) from a 16-bit value.
 * This function gets the LSB from a 16-bit value and stores it in the given pointer.
 * @param val The 16-bit value to get the LSB.
 * @param lsb Pointer to store.
 * @return 0 if success and 1 if the pointer is NULL.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL){
    return 1;
  }
  *lsb = val & 0xFF;
  return 0; 
}

/**
 * @brief Gets the most significant byte (MSB) from a 16-bit value.
 * This function gets the MSB from a 16-bit value and stores it in the given pointer.
 * @param val The 16-bit value to get the MSB.
 * @param msb Pointer to store.
 * @return 0 if success and 1 if the pointer is NULL.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL){
    return 1;
  }
  *msb = val >> 8;
  return 0;
}

/**
 * @brief Reads a byte from the given I/O port.
 * This function reads a byte from the given I/O port and stores it in the pointer ('*value').
 * @param port The I/O port to read.
 * @param value Pointer to store.
 * @return 0 if success and 1 if the pointer is NULL or if reading from the port fails.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL){
    return 1;
  }
  uint32_t temp;
  if (sys_inb(port, &temp) != 0){
    return 1;
  }
  
  *value = temp & 0xFF;
  return 0;
}
