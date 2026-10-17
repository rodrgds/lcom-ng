#include <lcom/lcf.h>

#include <stdint.h>

/// @brief Gets the least significant bit of 16 bit val
/// @param val Initial number of 16 bits
/// @param lsb where to send the LSB
/// @return 0 if success, 1 if no success
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb==NULL) return 1;
  *lsb = (uint8_t) val;
  return 0;
}

/// @brief Gets the most significant bit of 16 bit val
/// @param val Initial number of 16 bits
/// @param msb where to send the MSB
/// @return 0 if success, 1 if no success
int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  if (msb==NULL) return 1;
  *msb = (uint8_t) (val>>8);
  return 0;
}

/// @brief Adapts the function sys_inb to read 8-bit values
/// @param port port to read
/// @param value value of 8 bits read
/// @return 0 if success, 1 if no success
int (util_sys_inb)(int port, uint8_t *value) { 
 uint32_t aux;
 if (sys_inb(port,&aux)) return 1;
 *value = (uint8_t) aux;
 return 0;
  
}
