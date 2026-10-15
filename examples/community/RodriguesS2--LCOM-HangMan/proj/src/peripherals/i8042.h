#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

// KBC tem 2 registros na porta 0x64 e 0x60
#define STAT_REG 0x64
#define KBC_WRITING 0x64
#define OUT_BUF 0x60
#define WRITE_CMD 0x60
#define READ_CMD 0X20

//erros
#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OUT_BUF_FULL BIT(0)
#define IN_BUFF_FULL BIT(1)
#define AUX BIT(5) //bit 5 do status register, que indica se o dado lido é do teclado ou do mouse

//scancodes
#define ESC_BREAKCODE 0x81
#define MSB_SCANCODE 0xE0  // Byte mais significativo do scancode de dois bytes

#define KEY_UP_ARROW 0x48
#define KEY_DOWN_ARROW 0x50
#define KEY_ENTER 0x1C
#define KEY_ENTER_BREAK 0x9C

//mouse
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define STATUS_BYTE BIT(3)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)
#define DISABLE_DR 0xF5
#define ENABLE_DR 0xF4
#define WRITE_BYTE_TO_MOUSE 0xD4
#define SET_STREAM_MODE 0xEA
#define READ_DATA 0xEB

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define MAX_MOUSE_TRIES 5


#endif




