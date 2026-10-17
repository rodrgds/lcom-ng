#ifndef GRAPHICS_MACROS_H
#define GRAPHICS_MACROS_H

#define SET_VBE_MODE 0X4F02
#define RETURN_VBE_MODE 0x4F01
#define RETURN_VBE_CONTROLLER_INFO 0x4F00

#define BIOS_VIDEO_SERVICES 0x10

#define VALID_FUNC 0x4F
#define SUCCESS_FUNC 0x00

#define MODE_INDEXED 0x105
#define MODE_DIRECT_15 0x110
#define MODE_DIRECT_24 0x115
#define MODE_DIRECT_16 0x11A
#define MODE_DIRECT_32 0x14C

#define GET_RED(n)            (0xFF & ((n) >> 16))
#define GET_GREEN(n)          (0xFF & ((n) >>  8))
#define GET_BLUE(n)           (0xFF & ((n)      ))
#define SET_RED(n)            (((n) & 0xFF) << 16)
#define SET_GREEN(n)          (((n) & 0xFF) <<  8)
#define SET_BLUE(n)           (((n) & 0xFF)      )
#define SET_COLOR(r,g,b)      (SET_RED(r) | SET_GREEN(g) | SET_BLUE(b))

#define BLACK 0x000000

#endif
