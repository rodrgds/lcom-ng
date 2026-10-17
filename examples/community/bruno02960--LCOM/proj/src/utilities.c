#include "utilities.h"

unsigned long convertBCDtoBinary(unsigned long number) {
  unsigned long dec;

  dec = ((number & 0xF0) >> 4) * 10 + (number & 0x0F);

  return dec;
}

char twos_complement(char packet) {
  packet = -(~packet + 1);

  return packet;
}
