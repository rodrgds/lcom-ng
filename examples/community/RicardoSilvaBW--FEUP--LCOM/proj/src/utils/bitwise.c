#include "bitwise.h"
#include <stdarg.h>

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) {
    return msk & ~(1 << pos);
}

uint8_t set(uint8_t msk, int pos) {
    return msk | (1 << pos);
}

bool is_set(uint8_t msk, int pos) {
    return msk & (1 << pos);
}

uint8_t lsb(uint16_t wide_msk) {
    return wide_msk & 0xFF;
}

uint8_t msb(uint16_t wide_msk) {
    return (wide_msk >> 8) & 0xFF;
}

uint8_t mask(int pos, ...) {
    va_list positions; // All positions provided as function arguments.
    uint8_t res = 0;

    va_start(positions, pos); // Initializes the positions list from the stated first argument (pos) (I think?)
    int value = pos;

    while (value != MSK_END) {
        res = set(res, value); // Set value bit in the res number (to 1).
        value = va_arg(positions, int); // Read next int argument from the positions va_list, and stores it into value.
    }

    return res;
}
