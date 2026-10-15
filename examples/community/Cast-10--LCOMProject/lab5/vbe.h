#ifndef VBE_H
#define VBE_H

#include <lcom/lcf.h>



#define BIOS_VIDEO_INT 0x10

#define VBE_FUNCTION_AH 0x4F

#define VBE_GET_CTRL_INFO_AL 0x00
#define VBE_GET_MODE_INFO_AL 0x01
#define VBE_SET_MODE_AL 0x02

#define BIOS_SET_VIDEO_MODE 0x00
#define BIOS_TEXT_MODE 0x03

#define VBE_SET_DISPLAY_START_AL 0x07
#define VBE_SET_DISPLAY_START_NOW 0x00
#define VBE_SET_DISPLAY_START_VERTICAL 0x80

#define LINEAR_FRAMEBUFFER BIT(14)

/* VBE return values */
#define VBE_SUPPORTED 0x4F
#define VBE_SUCCESS 0x00

/* Screen Size */
#define MODE105_WIDTH 1024
#define MODE105_HEIGHT 768


#endif
