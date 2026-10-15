#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/* --- BIOS & VBE INTERRUPTS --- */
#define BIOS_VIDEO_SERV 0x10    // BIOS video services interrupt vector

/* --- AH REGISTERS (FUNCTIONS) --- */
#define VBE_CALL        0x4F    // VBE function call identifier
#define BIOS_SET_MODE   0x00    // Standard BIOS set-mode function

/* --- AL REGISTERS (SUB-FUNCTIONS) --- */
#define VBE_CTRL_INFO   0x00    // Returns controller information
#define VBE_MODE_INFO   0x01    // Returns information for a specific VBE mode
#define VBE_SET_MODE    0x02    // Sets the VBE video mode
#define TEXT_MODE       0x03    // Standard Minix text mode (80x25)
#define VBE_SET_START   0x07    // Set/Get Display Start (page flipping)

/* --- VBE 07h (SET DISPLAY START) FLAGS --- */
#define VBE_START_VSYNC BIT(7)  // BL=0x80: wait for vertical retrace before switching

/* --- CONFIGURATION BITS --- */
#define VBE_LINEAR_FB   BIT(14) // BX bit 14: enables the linear/flat frame buffer

/* --- VBE RETURN VALUES (IN AX) --- */
#define VBE_SUCCESS     0x004F  // Success: supported function and successful call

/* --- COMMON VBE MODES (LAB 5) --- */
#define MODE_105        0x105   // 1024x768, Indexed Color, 8 bits
#define MODE_110        0x110   // 640x480, Direct Color, 15 bits
#define MODE_115        0x115   // 800x600, Direct Color, 24 bits
#define MODE_11A        0x11A   // 1280x1024, Direct Color, 16 bits
#define MODE_14C        0x14C   // 1152x864, Direct Color, 32 bits

/* --- COLOR MODELS --- */
#define VBE_MDL_INDEXED 0x04    // Indexed Color Model
#define VBE_MDL_DIRECT  0x06    // Direct Color Model*/

#endif /* _LCOM_VBE_H_ */
