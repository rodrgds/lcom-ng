#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

// BIOS calls
#define BIOS_VIDEO 0x10   // This instruction takes as an argument an 8-bit value that specifies the interrupt number

// Interrupt vector -> set intno
#define VIDEO_CARD 0x10
#define PC_CONFIG 0x11
#define MEM_CONFIG 0X12
#define KBD_CONFIG 0x16

// Invoking the VBE Functions
#define VBE_AH 0x4F             // VBE call, AH register
#define AH_FAILED 0x01          // Function call failed
#define AH_NOT_SUPPORTED 0x02   // Function is not supported in current HW configuration
#define AH_INVALID 0x03         // Function call invalid in current video mode

// VBE modes
#define TEXT_MODE 0x03
#define GRAPHICS_MODE 0x02

// VBE submodes
#define VBE_1024X768_MODE 0x105      // Model: indexed, Bits per pixel (RGB): 8
#define VBE_640x480_MODE 0x110       // Model: direct, Bits per pixel (RGB): 15 ((1:)5:5:5)
#define VBE_800x600_MODE 0x115       // Model: direct, Bits per pixel (RGB): 24 (8:8:8)
#define VBE_1280x1024_MODE 0x11A     // Model: direct, Bits per pixel (RGB): 16 (5:6:5)
#define VBE_1152x864_MODE 0x14C      // Model: direct, Bits per pixel (RGB): 32 ((8:)8:8:8)

// VBE functions -> set AL register, AX = VBE_AH | FUNCTION (0x00 or 0x01 or 0x02)
#define VBE_CTRL_INFO 0x00          // Need to pass as argument the address of the vbe_controller_info_t struct
#define VBE_GET_MODE_INFO 0x01      // Need to pass as arguments the mode passed on register CX and the address of the vbe_mode_info_t struct
#define VBE_SET_MODE 0x02           // Mode should be passed in register BX, which should have bit 14 set

// The mode must be passed in the BX register

// Activate linear frame buffer -> Bit 14 of the BX register hould be set
#define LINEAR_MODE BIT(14)

// Reset VBE mode -> Set the mode to 0x03
#define VBE_RESET_MODE 0x03     // Use BIOS call 0x10 and function 0x00

#endif

