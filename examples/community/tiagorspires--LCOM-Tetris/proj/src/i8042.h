#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_LINE_KEYBOARD 1
#define IRQ_LINE_MOUSE 12

#define OUT_BUF_FULL BIT(0)
#define IN_BUF_FULL BIT(1)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)
#define ERROR (TIMEOUT_ERROR|PARITY_ERROR)

#define DATA_PORT 0X60
#define STATUS_PORT 0X64
#define READ_COMMAND_BYTE_ALERT 0X20
#define WRITE_COMMAND_BYTE_ALERT 0X60
#define WRITE_COMMAND 0X60
#define READ_COMMAND_ADRESS 0X64


#define DELAY_US 20000

#define ESC_BREAK_CODE 0x81
#define A_BREAK_CODE 0x9E
#define S_BREAK_CODE 0x9F
#define D_BREAK_CODE 0xA0
#define Q_BREAK_CODE 0x90
#define W_BREAK_CODE 0x91
#define E_BREAK_CODE 0x92
#define SPACE_BREAK_CODE 0xB9


#define LEFT_BUTTON     BIT(0)
#define RIGHT_BUTTON    BIT(1)
#define MIDDLE_BUTTON   BIT(2)
#define FIRST_BYTE      BIT(3)
#define X_SIGN          BIT(4)
#define Y_SIGN          BIT(5)
#define X_OVFL          BIT(6)
#define Y_OVFL          BIT(7)

#define SIGN_EXTENSION      0xFF00
#define BYTE_TO_MOUSE       0xD4
#define DIS_DATA_REPORTING  0xF5

#define ACK 0xFA
#define NACK 0xFE
#define ERROR_REP 0xFC


#define TWO_BYTES 0XE0

#define BREAK_CODE BIT(7)

#define MOUSE BIT(5)

#define KEYBOARD_INTERRUPT_ENABLE BIT(0)

#endif
