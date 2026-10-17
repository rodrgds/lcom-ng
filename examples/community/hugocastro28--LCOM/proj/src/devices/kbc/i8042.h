#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define DELAY_US 20000
#define INT_PER_SEC 60

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

// kbc ports
#define OUT_BUF_REG 0x60
#define ST_REG 0x64
#define KBC_CMD_REG 0x64

// kbc commands
#define READ_CMD 0x20
#define WRITE_CMD 0x60
#define WRITE_MOUSE_BYTE 0xD4

// mouse commands
#define ENABLE_DATA 0xF4
#define DISABLE_DATA 0xF5
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

// status flags
#define OBF BIT(0)
#define IBF BIT(1)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

// value of the most significant byte if the scanecode has 2 bytes
#define SCAN_MSB 0xE0

// make codes
#define ESC_MCODE 0x01
#define ARROW_UP_MCODE 0x48
#define ARROW_DOWN_MCODE 0x50
#define ARROW_RIGHT_MCODE 0x4D
#define ARROW_LEFT_MCODE 0x4B
#define SPACEBAR_MCODE 0x39

// break codes
#define ESC_BCODE 0x81
#define ARROW_UP_BCODE 0xC8
#define ARROW_DOWN_BCODE 0xD0
#define ARROW_RIGHT_BCODE 0xCD
#define ARROW_LEFT_BCODE 0xCB
#define SPACEBAR_BCODE 0xB9

// get value of packet's byte 1 bits
#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define MSB_X BIT(4)
#define MSB_Y BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#endif
