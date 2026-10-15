#include <lcom/lcf.h>

#include <stdint.h>


/// @brief Gets the least significant byte of a 16 bit number
/// @param val Number to get the LSB from
/// @param lsb Output for LSB
/// @return Returns 0 on success and 1 otherwise
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb==NULL) return 1;
  *lsb=(uint8_t) val;
  return 0;
}

/// @brief Gets the most significant byte of a 16 bit number
/// @param val Number to get the MSB from
/// @param msb Output for MSB
/// @return Returns 0 on success and 1 otherwise
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb==NULL) return 1;
  val=val>>8;
  *msb=(uint8_t) val;
  return 0;
}

/// @brief Reads from a specified port
/// @param port Port to read from
/// @param value Value received from the port
/// @return Returns 0 on success and 1 otherwise 
int (util_sys_inb)(int port, uint8_t *value) {
  if (value==NULL) return 1;
  uint32_t tempValue;
  int ret = sys_inb(port,&tempValue);
  *value = (uint8_t) tempValue;
  if (ret!=0) return 1;
  return 0;
}
