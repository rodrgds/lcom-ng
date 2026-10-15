#ifndef _LCOM_VBE_MACROS_H_
#define _LCOM_VBE_MACROS_H_

#include <lcom/lcf.h>

#define VBE_INT               0x10    

/* Funções */
#define VBE_SET_MODE          0x4F02  
#define VBE_GET_MODE_INFO     0x4F01  
#define BIOS_SET_MODE         0x00    

/* Modos */
#define TEXT_MODE_80x25       0x03    
#define MODE_1024x768_256     0x105   

/* Controlo */
#define VBE_LINEAR_FRAME_BUFFER BIT(14) 





#endif

