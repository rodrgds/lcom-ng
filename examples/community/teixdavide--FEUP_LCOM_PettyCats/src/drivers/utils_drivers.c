#include <lcom/lcf.h>

#include <stdint.h>

#include "utils_drivers.h"


int (util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb == NULL) return 1;

  *lsb = val;

  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  if (value == NULL) return 1;
  
  uint32_t read_value;

  sys_inb(port, &read_value);

  *value = read_value;

  return 0;
}

uint8_t (bcd_to_binary)(uint8_t bcd) {
  uint8_t tens = (bcd >> 4) & 0x0F;  // high nibble
  uint8_t ones = bcd & 0x0F;         // low nibble
  
  return tens * 10 + ones;
}

int getRandomInt(int lower_bound, int upper_bound) {
  static int seed_initialized = 0;
  if (!seed_initialized) {
    srand(time(NULL));
    seed_initialized = 1;
  }

  return lower_bound + rand() % (upper_bound - lower_bound + 1);
}

float getRandomFloat(float lower_bound, float upper_bound) {
  static int seed_initialized = 0;
  if (!seed_initialized) {
    srand(time(NULL));
    seed_initialized = 1;
  }

  float random = ((float)rand()) / RAND_MAX;
  return lower_bound + random * (upper_bound - lower_bound);
}

