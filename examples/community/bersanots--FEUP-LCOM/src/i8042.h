#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the Keyboard, Mouse and Video card.
 */

#define DELAY_US 20000
#define KBD_IRQ 1
#define MOUSE_IRQ 12

#define OK 0

//Status Register
#define STAT_REG_OBF BIT(0)
#define STAT_REG_IBF BIT(1)
#define STAT_REG_SYS BIT(2)
#define STAT_REG_A2 BIT(3)

#define STAT_REG_INH BIT(4)
#define STAT_REG_AUX BIT(5)
#define STAT_REG_TIMEOUT BIT(6)
#define STAT_REG_PARITY BIT(7)

//KBC commands
#define KBC_READ_CMD 0X20
#define KBC_WRITE_CMD 0x60
#define KBC_CHECK 0xAA
#define KBC_CHECK_INTERFACE 0xAB
#define KBC_DISABLE_KBD 0xAD
#define KBC_ENABLE_KBD 0xAE

#define STAT_REG 0X64
#define OUT_BUF 0X60
#define IN_BUF 0X60
#define KBC_CMD_REG 0x64
#define KBC_CMD_ARG 0x60
#define ESC_BREAKCODE 0x81
#define ENTER_BREAKCODE 0x9C
#define TWO_BYTE_SCANCODE 0xE0

#define W_MAKECODE 0x11
#define W_BREAKCODE 0x91
#define A_MAKECODE 0x1E
#define A_BREAKCODE 0x9E
#define S_MAKECODE 0x1F
#define S_BREAKCODE 0x9F
#define D_MAKECODE 0x20
#define D_BREAKCODE 0xA0
#define ARROW_LEFT_MAKECODE 0x4B
#define ARROW_LEFT_BREAKCODE 0xCB
#define ARROW_RIGHT_MAKECODE 0x4D
#define ARROW_RIGHT_BREAKCODE 0xCD
#define ARROW_UP_MAKECODE 0x48
#define ARROW_UP_BREAKCODE 0xC8
#define ARROW_DOWN_MAKECODE 0x50
#define ARROW_DOWN_BREAKCODE 0xD0

//mouse commands
#define LBP 0x01
#define RBP 0x02
#define MBP 0x04
#define BIT_3 0x08
#define DELTA_X 0x10
#define DELTA_Y 0x20
#define X_OVERFLOW 0x40
#define Y_OVERFLOW 0x80

#define MOUSE_READ_CMD 0x20
#define MOUSE_WRITE_CMD 0x60
#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8
#define CHECK_MOUSE_INTERFACE 0xA9
#define WRITE_BYTE_TO_MOUSE 0xD4

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define SET_DEFAULT 0xF6

#define RESET 0xFF
#define SET_REMOTE_MODE 0xF0
#define SET_STREAM_MODE 0xEA
#define READ_DATA 0xEB

//video commands
#define INT_VIDEO 0x10
#define VBE_CALL 0x4F
#define SET_VBE_MODE 0x02
#define RETURN_VBE_MODE_INFO 0x01
#define LINEAR_MODEL 1 << 14
#define RETURN_VBE_CONTROLLER_INFO 0x00

#define GRAPHICS_MODE_1 0x105 //1024x768 - indexed - 8 bits per pixel
#define GRAPHICS_MODE_2 0x110 //640x480 - direct color - 15 bits per pixel ((1:)5:5:5)
#define GRAPHICS_MODE_3 0x115 //800x600 - direct color - 24 bits per pixel (8:8:8)
#define GRAPHICS_MODE_4 0x11A //1280x1024 - direct color - 16 bits per pixel (5:6:5)
#define GRAPHICS_MODE_5 0x14C //1152x864 - direct color - 32 bits per pixel ((8:)8:8:8)

#define VBE_FUNCTION_CALL_FAIL 0x01
#define VBE_FUNCTION_NOT_SUPPORTED 0x02
#define VBE_FUNCTION_INVALID 0x03

#define MODEINFOBLOCK_SIZE 256
#define VBEINFOBLOCK_SIZE 512 //for VBE 2.0

/**@}*/

#endif /* _LCOM_I8042_H */
