#include "bitwise.h"
#include <stdarg.h>

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) {
	msk &= ~(1 << pos);
	return msk;
}

uint8_t set(uint8_t msk, int pos) {
	msk |= (1 << pos);
	return msk;
}

bool is_set(uint8_t msk, int pos) {
	return ((msk >> pos) & 1) == 1;
}

uint8_t lsb(uint16_t wide_msk) {
	return wide_msk & 0xFF;
}

uint8_t msb(uint16_t wide_msk) {
	return (wide_msk & 0xFF00) >> 8;
}

uint8_t mask(int pos, ...) {
	uint8_t msk = 0;
	va_list list;

	va_start(list, pos);

	while (pos != MSK_END) {
		msk |= (1 << pos);
		pos = va_arg(list, int);
	}

	return msk;
}
