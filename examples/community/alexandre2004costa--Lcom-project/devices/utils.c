#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup utils utils
 * @{
 */



int sys_inb_counter = 0;

/**
 * @brief Gets the least significant byte out of a 2 byte value.
 * @param val 2 byte value.
 * @param lsb Reference where lsb will be stored.
 * @return 0 in case of success.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & 0xff;
  return 0;
}

/**
 * @brief Gets the most significant byte out of a 2 byte value.
 * @param val 2 byte value.
 * @param msb Reference where msb will be stored.
 * @return 0 in case of success.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

/**
 * @brief Reads content through a designated port, and converts the 32 bits read value into an 8 bit one.
 * @param port Port through which value is read.
 * @param value Reference that will store the value.
 * @return 0 in case of success.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL || port < 0) return 1;
  uint32_t local;
  if (sys_inb(port,&local) != 0) return 1;
  sys_inb_counter += 1;
  *value = local & 0xff; 
  return 0;
}

/**@}*/
