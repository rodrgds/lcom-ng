#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/*VALUES TO PUT THE GRAPHIC MODE*/
#define VBE_AH          0x4F

//values for AL
#define VBE_SET_MODE    0x02
#define VBE_MODE_INFO   0x01
#define VBE_CTRL_INFO   0x00
#define VBE_DISPLAY_CTRL 0x07

//values for bx
#define VBE_BX_OR       BIT(14)
#define VBE_SET_DURING_VRETRACE 0x0080

#define INTNO           0x10

#endif /* _LCOM_VBE_H */
