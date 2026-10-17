/** 
 * @file vbe.h
 * @brief This file defines several useful macros to use with the vbe
 */


#ifndef _VBE_H_
#define _VBE_H_

#include <lcom/lcf.h>

#define VIDEO_INT_INSTRUCTION 0x10 ///< Software interrupt instruction for the video services

/* VBE functions */
#define VBE_FUNCTION 0x4F ///< Value to be set on register AH when calling a VBE function
#define VBE_CONTROLLER_INFO 0x00 ///< VBE function to get the controller information, set on AL
#define VBE_MODE_INFO 0x01 ///< VBE function to get the mode information, set on AL
#define VBE_SET_MODE 0x02 ///< VBE function to set the mode, set on AL
#define VBE_GET_MODE 0x03 ///< VBE function to get the current mode, set on AL
#define VBE_FUNCTION_SUPPORTED 0x4F ///< Output on register AL

/* Set mode function */
#define LINEAR_FRAME_BUFFER BIT(14) ///< Sets the VRAM as a sucession of lines in memory

/* Color models */
#define DIRECT_COLOR 0x06 ///< Colors are represented as R:G:B, size varies

#endif /*_VBE_H */
