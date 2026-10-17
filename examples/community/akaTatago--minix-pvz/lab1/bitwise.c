#include <stdarg.h>
#include "bitwise.h"

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
  return msk & ~(0x1 << (pos));
}

uint8_t set(uint8_t msk, int pos) { 
  return msk | (0x1 << (pos));
}

bool is_set(uint8_t msk, int pos) { 
  return msk & set(0x00, pos);
}

uint8_t lsb(uint16_t wide_msk) {
  return wide_msk & (0x00FF);
}

uint8_t msb(uint16_t wide_msk) { 
  return (wide_msk >> 8) & (0x00FF);
}

uint8_t mask(int pos, ...) {
  uint8_t msk = 0x00;

  va_list args;
  va_start(args, pos);

  int current_pos = pos;
  while (current_pos != MSK_END) {
    msk |= (1 << current_pos);
    current_pos = va_arg(args, int); // VS Code complains about "expected an expression" here but it works so :P
  }

  va_end(args);
  return msk;
}
