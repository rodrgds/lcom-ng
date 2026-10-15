#include "bitwise.h"
#include <stdarg.h>

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  msk &= ~bit;
  return msk;
}

uint8_t set(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  msk |= bit;
  return msk; 
}

bool is_set(uint8_t msk, int pos) { 
  uint8_t bit = 1 << pos;
  uint8_t  f = msk & bit;
  return f;
}

uint8_t lsb(uint16_t wide_msk) { 
  return (uint8_t) wide_msk;
}

uint8_t msb(uint16_t wide_msk) { 
  return (uint8_t) (wide_msk >> 8); 
}

uint8_t mask(int pos, ...) { 
  va_list ap;
  uint8_t msk = 0;

  va_start(ap, pos);

  int p = pos;
  while ( p != MSK_END)
  {
    msk = set(msk, p);
    p = va_arg(ap, int);
  }
  
  va_end(ap);
  return msk;
}
