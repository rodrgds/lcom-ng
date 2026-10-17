#ifndef __I8042_H
#define __I8042_H
#include "vbe.h"


/*
 * KEYBOARD MACROS
 */

#define DELAY_US    20000

#define KBC_IRQ 1 /**< @brief Keyboard IRQ line */
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60
#define KBC_DATA_REG 0x60
#define KBC_STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_HOOK_BIT 1
#define BREAK_ESC_CODE 0x81
#define MAKE_ESC_CODE 0x01

//STATUS REGISTER
#define KBC_OBF BIT(0)
#define KBC_IBF BIT(1)
#define AUX BIT(5)

//C@KBD values
#define RESEND 0xFE
#define ERROR 0xFC
//==========================================================================================
/*
 * MOUSE MACROS
 */

#define MOUSE_IRQ 12

#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_DISABLE_STREAM_MODE 0xF5
#define WRITE_BYTE_TO_MOUSE 0xD4
#define MOUSE_SEND_PACKETS 0xF4
#define MOUSE_STATUS_REQUEST 0xE9
#define ENABLE_MOUSE 0xA8
#define MC 0xD4
#define COMMAND_PORT 0x60
#define OBF 0x01
#define MOUSE_DATA	0x20
#define DISABLE_STREAM	0xF5
#define SET_STREAM	0xEA
#define ESDP		0xF4
#define IBF			0x02
#define NACK		0xFE

/* MOUSE PACKETS */

#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)

/* MOUSE STATUS */

#define RIGHT_STATUS_BUTTON BIT(0)
#define MIDDLE_STATUS_BUTTON BIT(1)
#define LEFT_STATUS_BUTTON BIT(2)
#define SCALLING BIT(4)
#define SEND_PACKETS_ENABLED BIT(5)
#define REMOTE BIT(6)


#endif
