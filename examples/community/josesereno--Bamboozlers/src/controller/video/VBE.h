#ifndef _VBE_H_
#define _VBE_H_

#include <lcom/lcf.h>

#define TRANSPARENT 0xFFFFFE
#define WHITE 0xFFFFFF
#define GREY 0x3B3838
#define LIGHT_GREY 0xAFABAB

#define VIDEO_SERVICES 0x10

#define VBE_SET_MODE 0x4F02

#define LINEAR_FRAME_BUF BIT(14)

#define TEXT_MODE 0x03
#define VBE_INDEXED_MODE 0X105
#define VBE_DIRECT_480p 0X110
#define VBE_DIRECT_600p 0X115
#define VBE_DIRECT_1024p 0X11A
#define VBE_DIRECT_864p 0X14C

#endif /* _VBE_H */
