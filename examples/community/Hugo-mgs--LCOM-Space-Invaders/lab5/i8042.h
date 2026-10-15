#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

// Interrupt constants
#define KBD_IRQ 1
#define MOUSE_IRQ 12

// Keyboard constants
#define BREAK_ESC 0x81
#define DELAY_US 20000

// KBC ports
#define KBD_OUT_BUF 0x60
#define KBD_IN_BUF 0x64
#define KBD_STAT_REG 0x64
#define KBD_CMD_REG 0x64
#define KBC_READ_CMD 0x20
#define KBC_RESET 0x60

// KBC commands
#define KBC_WRITE_CMD 0x60
#define KBC_READ_CMD_BYTE 0x20

// Mouse commands
#define MOUSE_AUX BIT(5)
#define MOUSE_WRITE_CMD 0xD4
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ENABLE_DATA_REPORTING 0xF4
#define MOUSE_DISABLE_DATA_REPORTING 0xF5
#define MOUSE_SET_STREAM_MODE 0xEA
#define MOUSE_SET_REMOTE_MODE 0xF0
#define MOUSE_READ_DATA 0xEB

// Error codes
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define FULL_IN_BUF BIT(1)
#define FULL_OUT_BUF BIT(0)

#endif
