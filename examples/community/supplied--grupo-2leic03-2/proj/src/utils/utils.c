#include <lcom/lcf.h>
#include <stdint.h>

#include "utils.h"

int util_sys_inb(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }

  uint32_t temp;

  if (sys_inb(port, &temp) != OK) {
    return 1;
  }

  *value = (uint8_t) temp;

  return 0;
}

int clamp_int(int value, int min, int max) {
  if (value < min) {
    return min;
  }

  if (value > max) {
    return max;
  }

  return value;
}

