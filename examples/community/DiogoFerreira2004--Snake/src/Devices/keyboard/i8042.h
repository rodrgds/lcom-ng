#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BREAK_ESC 0x81
#define TWO_BYTES_PREFIX 0xE0
#define DELAY_US 20000
#define KBD_IRQ 1
#define OUTPUT_BUFFER_PORT 0x60
#define STATUS_REGISTER_PORT 0x64
#define KBC_COMMAND_REG 0x64        
#define KBC_READ_COMMAND_BYTE 0x20 
#define KBC_WRITE_COMMAND_BYTE 0x60 
#define MAKE_CODE_CHECK BIT(7)
#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)
#define KBC_ESC_BREAK_CODE BREAK_ESC
#define KBC_ST_IBF BIT(1) 
#define INPUT_BUFFER_PORT 0x60 
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_ENTER 0x1C
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20

#endif
