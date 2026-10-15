/**
 * @file utils.c
 * 
 * @brief Provides functions to fetch data from the devices' controllers
 * and to process the data if desired.
*/
#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Gets the least significant byte of a 16-bit number
 * 
 * @param val 16-bit number
 * @param lsb Address of memory of the 8-bit integer where the least significant byte will be stored
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; 
  *lsb = 0xFF & val;        
  return 0;
}

/**
 * @brief Gets the most significant byte of a 16-bit number
 * 
 * @param val 16-bit number
 * @param msb Address of memory of the 8-bit integer where the most significant byte will be stored
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

/**
 * @brief Reads data from a device's controller
 * 
 * This function reads data from a device's controller and stores it in a
 * 8-bit integer. It has to use a 32-bit integer to read the data, because´
 * the sys_inb() function only reads 32-bit integers. After reading the data,
 * it stores the data in the 8-bit integer.
 * 
 * @param port Address of the port to read from
 * @param value Address of memory of the 8-bit integer where the data will be stored
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;   
  uint32_t val;                  
  int ret = sys_inb(port, &val);  
  *value = 0xFF & val;            
  return ret;
}
