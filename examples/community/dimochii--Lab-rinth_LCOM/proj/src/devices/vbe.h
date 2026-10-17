#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
 
#define BIOS_VIDEOCARD      0x10

#define VBE_1024x768        0x105
#define VBE_640x480         0x110
#define VBE_800x600         0x115
#define VBE_1280x1024       0x11A
#define VBE_1152x864        0x14C

#define LINEAR_BUFFER       BIT(14)

#define SET_VBE_MODE        0x4F02
#define VBE_MODE_INFO       0x4F01
#define VBE_CTRL_INFO       0x4F00
#define BIOS_SET_VIDEOMODE  0x00

#define DIRECT_COLOR        0x06
#define INDEXED_COLOR       0x105

#endif

