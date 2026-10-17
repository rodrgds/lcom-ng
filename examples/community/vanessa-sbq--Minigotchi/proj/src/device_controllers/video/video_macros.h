#ifndef _LCOM_VIDEO_MACROS_H_
#define _LCOM_VIDEO_MACROS_H_

// Video BIOS Service
#define VIDEO_BIOS_SERVICE 0x10 

// Video functions
#define VBE_MODE_INFO   0x01
#define VBE_SET_MODE    0x02

#define VBE_CALL 0x4f // VBE call, function 02 - set VBE mode

// Graphics modes
#define VIDEO_LINEAR_FRAMEBUF_MODE BIT(14)

// Transparency color
#define TRANSPARENT 0x000001

#endif /* _LCOM_VIDEO_MACROS_H */
