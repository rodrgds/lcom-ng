#include "bitwise.h"
#include <stdarg.h>

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) {
  uint8_t res = 1U;
  res <<= pos;
  res = ~res;
  res &= msk;
  return res;
}

uint8_t set(uint8_t msk, int pos) {
  uint8_t res = 1U;
  res <<= pos;
  res |= msk;
  return res;
}

bool is_set(uint8_t msk, int pos) {
  uint8_t res = 1U;
  res <<= pos;
  res &= msk;
  res >>=pos;
  if(res&1) return true;
  return false;
}

uint8_t lsb(uint16_t wide_msk) {
  uint8_t res = 0;
  res |= wide_msk;
  return res;
}

uint8_t msb(uint16_t wide_msk) {
  uint16_t res = wide_msk;
  res >>= 8;
  return res & 0xFF;
}

uint8_t mask(int pos, ...) {
  uint8_t res = 0;

  if (pos == MSK_END) return res;

  res = set(res, pos);

  va_list arg;
  va_start(arg, pos);

  int current_pos;
  while ((current_pos = va_arg(arg, int)) != MSK_END) {
    res = set(res, current_pos);
  }

  va_end(arg);

  return res;
}
