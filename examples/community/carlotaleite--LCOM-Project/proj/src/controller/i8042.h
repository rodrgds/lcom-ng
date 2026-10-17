#pragma once

#define KBD_IRQ 1
#define MOUSE_IRQ 12

#define ESC_BREAK_CODE 0x81
#define UP_ARROW_MAKE_CODE 0x48
#define LEFT_ARROW_MAKE_CODE 0x4B
#define DOWN_ARROW_MAKE_CODE 0x50
#define RIGHT_ARROW_MAKE_CODE 0x4D
#define W_MAKE_CODE 0x11
#define A_MAKE_CODE 0x1E
#define S_MAKE_CODE 0x1F
#define D_MAKE_CODE 0x20

#define TWO_BYTE_CODE 0xE0
#define DELAY_US 20000
#define MAKE_CODE BIT(7)

// KBC registers

#define KBC_STATUS_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_IN_BUF 0x60 // Supposedly not used in LCOM but needed for passing arguments to KBC commands
#define KBC_OUT_BUF 0x60

// KBC status register

#define KBC_STAT_PARITY_ERR BIT(7)
#define KBC_STAT_TIMEOUT_ERR BIT(6)
#define KBC_STAT_AUX_MOUSE BIT(5)
#define KBC_STAT_IN_BUFF_FULL BIT(1)
#define KBC_STAT_OUT_BUFF_FULL BIT(0)

// KBC commands

#define KBD_READ_CMD 0x20
#define KBD_WRITE_CMD 0x60

// KBC commands arguments

#define KBC_KB_INTERRUPT_ENABLE BIT(0)
#define KBC_MOUSE_INTERRUPT_ENABLE BIT(1)
#define KBC_DISABLE_KB_INTERFACE BIT(4)
#define KBC_DISABLE_MOUSE_INTERFACE BIT(5)

// Mouse commands

#define KBC_CMD_WRITE_BYTE_TO_MOUSE 0xD4

#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_MSB_X BIT(4)
#define MOUSE_MSB_Y BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4
