/// @file utils.c
/// @brief Utility functions for LSB/MSB manipulation and system input operations.

#include <lcom/lcf.h>
#include <stdint.h>

#ifdef LAB3
uint32_t syscall_counter = 0; ///< Counter for sys_inb calls in LAB3
#endif

/**
 * @brief Gets the least significant byte of a 16-bit value.
 * 
 * @param val The 16-bit value.
 * @param lsb Pointer to store the least significant byte.
 * @return 0 on success, 1 on error (e.g., null pointer).
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) {
    return 1;
  }
  
  *lsb = (uint8_t)(val & 0xFF);
    
  return 0;
}

/**
 * @brief Gets the most significant byte of a 16-bit value.
 * 
 * @param val The 16-bit value.
 * @param msb Pointer to store the most significant byte.
 * @return 0 on success, 1 on error (e.g., null pointer).
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) {
    return 1;
  }
    
  *msb = (uint8_t)(val >> 8);
    
  return 0;
}

/**
 * @brief Reads a byte from the specified I/O port.
 * 
 * @param port The I/O port to read from.
 * @param value Pointer to store the read byte.
 * @return 0 on success, 1 on error.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }

  uint32_t temp;
  if (sys_inb(port, &temp) != OK) {
    return 1;
  }

  *value = (uint8_t)(0xFF & temp);

  #ifdef LAB3
  syscall_counter++;
  #endif

  return 0;
}

