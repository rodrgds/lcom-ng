#include "auxfuncs.h"

/** @defgroup Auxiliary_Functions
 * @{
 *
 * Auxiliary functions to the project
 */

extern int count;

/**
 * @brief Function that retrieves the least significant byte (LSB) from a 16-bit value.
 *
 * @param val The 16-bit value from which the LSB should be obtained.
 * @param lsb Pointer to the variable where the LSB will be stored.
 * @return 0 on success, nonzero otherwise.
 */
int(aux_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) (val & 0xFF);
  return 0;
}

/**
 * @brief Function that retrieves the most significant byte (MSB) from a 16-bit value.
 *
 * @param val The 16-bit value from which the MSB should be obtained.
 * @param msb Pointer to the variable where the MSB will be stored.
 * @return 0 on success, nonzero otherwise.
 */
int(aux_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) ((val & 0xFF00) >> 8);
  return 0;
}

/**
 * @brief Function that performs an input from a given port and retrieves the value.
 *
 * @param port The port from which the input should be performed.
 * @param value Pointer to the variable where the retrieved value will be stored.
 * @return 0 on success, nonzero otherwise.
 */
int(aux_sys_inb)(int port, uint8_t *value) {
  uint32_t u32 = 0x00;
  sys_inb(port, &u32);
#ifdef LAB3
  count++;
#endif
  *value = (uint8_t) u32;
  return 0;
}

/**
 * @brief Function that performs a 16-bit input from a given port and retrieves the value.
 *
 * @param port The port from which the input should be performed.
 * @param value Pointer to the variable where the retrieved value will be stored.
 * @return 0 on success, nonzero otherwise.
 */
int(aux_sys_inb_16)(int port, uint16_t *value) {
  uint32_t u32 = 0x00;
  sys_inb(port, &u32);
#ifdef LAB3
  count++;
#endif
  *value = (uint16_t) u32;
  return 0;
}
