/**
 * @file utils.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = 0xFF & val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val >> 8;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  int ret = sys_inb(port, &val);
  *value = 0xFF & val;
  return ret;
}
