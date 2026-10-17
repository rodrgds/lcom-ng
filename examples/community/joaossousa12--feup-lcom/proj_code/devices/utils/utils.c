/**
 * @file utils
 * 
 * @brief This file provides utility functions for handling bits and I/O operations that will be helpful on other devices/controllers.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023
*/

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Extracts the least significant bits (LSB) from a 16-bit value.
 * 
 * @param val The 16-bit value from which to extract the LSB.
 * 
 * @param lsb Pointer to an 8-bit variable where the LSB will be stored.
 * 
 * @return 0 on success, 1 if the pointer is invalid.
*/

// LSB -> Less Significant Bits
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // o apontador deve ser válido
  *lsb = 0xFF & val;         // preserva os 8 bits menos significativos
  return 0;
}

/**
 * @brief Extracts the most significant bits (MSB) from a 16-bit value.
 * 
 * @param val The 16-bit value from which to extract the MSB.
 * 
 * @param msb Pointer to an 8-bit variable where the MSB will be stored.
 * 
 * @return 0 on success, 1 if the pointer is invalid.
*/

// MSB -> Most Significant Bits
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

/**
 * @brief Reads the input from a specified port and converts it to an 8-bit value.
 * 
 * @param port The port from which to read the input.
 * 
 * @param value Pointer to an 8-bit variable where the input value will be stored.
 * 
 * @return 0 on success, non-zero if the pointer is invalid or there was an error reading the input.
*/

// Transform 32 bit output in 8 bit output. Error prevention.
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;    // o apontador deve ser válido
  uint32_t val;                   // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val);  // val ficará com o valor lido de port
  *value = 0xFF & val;            // value ficará apenas com os primeiros 8 bits do resultado
  return ret;
}
