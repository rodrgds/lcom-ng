#ifndef VBE_H
#define VBE_H
#include <lcom/lcf.h>
#define VBE_MODE 0x115
#define VBE_INTERRUPT 0x10
#define VBE_AH 0x4F
#define VBE_SET_MODE 0x02
#define VBE_SET_LINEAR_MODEL BIT(14)
#define VBE_SET_MODE_FUNC 0x4F02
#endif //VBE_H
