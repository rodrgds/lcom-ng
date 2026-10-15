#include <lcom/lcf.h>

#include <stdint.h>

int cnt = 0;


/**
 * @brief Retrieves the least significant byte (LSB) from a 16-bit value.
 * @details This function extracts the LSB from a given 16-bit value and stores it in the provided 
 * pointer.
 * @param val The 16-bit value from which to extract the LSB.
 * @param lsb Pointer to store the extracted LSB.
 * @return 0 on success.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t temp = 0;

  temp = (val & 0x00FF);

  *lsb = (uint8_t) temp;

  return 0;
}


/**
 * @brief Retrieves the most significant byte (MSB) from a 16-bit value.
 * @details This function extracts the MSB from a given 16-bit value and stores it in the provided 
 * pointer.
 * @param val The 16-bit value from which to extract the MSB.
 * @param msb Pointer to store the extracted MSB.
 * @return 0 on success.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t temp = 0;

  temp = (val & 0xFF00);

  temp = temp >> 8;

  *msb = (uint8_t) temp;

  return 0;
}


/**
 * @brief Reads a byte from a specified port.
 * @details This function reads a byte from the specified I/O port and stores it in the provided 
 * pointer. It also increments a global counter when a specific macro is defined.
 * @param port The I/O port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, 1 otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t variable = 0;
  
  if(sys_inb(port, &variable) == 1) {
    return 1;
  }
  #ifdef LAB3
  cnt++;
  #endif


  *value = (uint8_t) variable;


  return 0;
}
