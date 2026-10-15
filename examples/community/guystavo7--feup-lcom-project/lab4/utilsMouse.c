#include "utilsMouse.h"

int sys_inb_8b(uint32_t addr, uint8_t* output) {
	uint32_t byte;
	if (sys_inb(addr, &byte)) return -1;
	*output = byte;
	return 0;
}
