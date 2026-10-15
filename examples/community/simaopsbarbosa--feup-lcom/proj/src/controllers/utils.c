#include <lcom/lcf.h>

#include <stdint.h>


/**
 * @brief Gets the LSB of a 16-bit value.
 * 
 * Extracts the LSB from a uint16_t value and stores it in the provided pointer.
 * 
 * @param val The 16-bit value from which to extract the LSB.
 * @param lsb Pointer to uint8_t where the LSB will be stored.
 * @return int Returns 0 on success.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)val;
  return 0;
}

/**
 * @brief Gets the MSB of a 16-bit value.
 * 
 * Extracts the MSB from a uint16_t value and stores it in the provided pointer.
 * 
 * @param val The 16-bit value from which to extract the MSB.
 * @param msb Pointer to uint8_t where the MSB will be stored.
 * @return int Returns 0 on success.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val >> 8);
  return 0;
}

/**
 * @brief Reads a byte from an I/O port using sys_inb.
 * 
 * Uses sys_inb to read a 32-bit value from the specified port, then converts it to uint8_t.
 * 
 * @param port The I/O port to read from.
 * @param value Pointer to uint8_t where the read byte will be stored.
 * @return int Returns the return code of sys_inb.
 */
int(util_sys_inb)(int port, uint8_t *value) {

  uint32_t aux;
  int ret = sys_inb(port, &aux);
  *value = (uint8_t)aux;

  return ret;
}
