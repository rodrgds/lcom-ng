#ifndef VIDEO_UTILS
#define VIDEO_UTILS

#include <lcom/lcf.h>

// Interrupts
#define VIDEO_CARD_INTERRUPT 		0x10

// VBE Functions
#define SET_VBE_MODE_AH 			0x4F
#define SET_VBE_MODE_AL 			0x02

// Misc
#define LINEAR_FRAME_BUFFER_BIT		1 << 14

int enterMode(uint16_t mode);
int pageFlip(uint16_t yOffset);

#endif
