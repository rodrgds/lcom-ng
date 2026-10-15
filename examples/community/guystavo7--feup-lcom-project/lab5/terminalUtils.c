#include "terminalUtils.h"
#include <string.h>

int enterMode(uint16_t mode) {
	reg86_t reg; 
	memset(&reg, 0, sizeof(reg));

	reg.intno = VIDEO_CARD_INTERRUPT;
	reg.ah = SET_VBE_MODE_AH;
	reg.al = SET_VBE_MODE_AL;

	reg.bx = mode | LINEAR_FRAME_BUFFER_BIT;

	if (sys_int86(&reg)) {
		printf("Error on setting mode\n");
		return -1;
	}

	return 0;
}

int pageFlip(__uint16_t yOffset) {
	reg86_t reg;
	memset(&reg, 0, sizeof(reg));

	reg.intno = VIDEO_CARD_INTERRUPT;
	reg.ax = 0x4F07;
	reg.bx = 0x0000;
	reg.cx = 0;
	reg.dx = yOffset;

	if (sys_int86(&reg)) {
		return -1;
	}

	return 0;
}
