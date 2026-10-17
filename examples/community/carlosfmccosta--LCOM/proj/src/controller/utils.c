/**
 * @file utils.c
 * @brief Implementation of utility functions.
 *
 * This file contains the implementation of utility functions used throughout the project.
 */
#include <lcom/lcf.h>

#include <stdint.h>

int ctr = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // Places the least significant byte of val in lsb
  *lsb = (uint8_t) (val & 0x00FF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // Places the most significant byte of val in lsb
  *msb = (uint8_t) ((val >> 8) & 0x00FF);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
#ifdef LAB3
  ctr++;
#endif

  uint32_t res; // store result of system call

  // system call to read data from a port and handle error
  if (sys_inb(port, &res) != 0) {
    printf("Error read data from port: %d\n", port);
    return 1;
  }

  *value = (uint8_t) res;

  return 0;
}
