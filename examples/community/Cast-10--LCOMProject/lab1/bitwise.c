#include "bitwise.h"

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
    return msk & ~(1 << pos);
}

uint8_t set(uint8_t msk, int pos) {
    return msk | 1 << pos;
}

bool is_set(uint8_t msk, int pos) {
    return (msk | 1 << pos) == msk;
}

uint8_t lsb(uint16_t wide_msk) {
    return (uint8_t)(wide_msk & 0xFF);
}

uint8_t msb(uint16_t wide_msk) {
    return (uint8_t)(wide_msk >> 8);
}

uint8_t mask(int pos, ...) {
    uint8_t result = 0;
    va_list arguments;

    va_start(arguments, pos);

    int cur = pos;
    while (cur != MSK_END)
    {
        result |= (uint8_t)(1 << cur);

        cur = va_arg(arguments, int);
    }

    va_end(arguments);
    return result;
}
