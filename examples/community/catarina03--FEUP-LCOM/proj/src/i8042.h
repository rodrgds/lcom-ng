#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

#define KBD_IRQ 1

//KBC registers
#define STATUS_REGISTER 0x64
#define OUT_BUF 0X60
#define IN_BUF 0X64
#define KBC_CMD_ARGS 0x60


//KBC commands
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

//KBC command byte bits
#define ENABLE_KBD_OBF BIT(0)

//important key scancodes
#define ERRO 0xff
#define ESC_BRK_CODE 0x81
#define SECOND_BYTE_CODE 0xe0
#define BREAK_CODE BIT(7)
#define DOWN_ARROW 0x50
#define UP_ARROW 0x48
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d
#define SPACEBAR 0x39
#define ALT 0x38
#define DOWN_ARROW_BRK 0xd0
#define UP_ARROW_BRK 0xc8
#define LEFT_ARROW_BRK 0xcb
#define RIGHT_ARROW_BRK 0xcd
#define SPACEBAR_BRK 0xb9
#define ALT_BRK 0xb8

//status register bits
#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define MOUSE_ACTIVE BIT(5)
#define IBF BIT(1)
#define OBF BIT(0)


#define DELAY 20000



//mouse macros

//command values for the mouse
#define ENABLE_MOUSE         0xA8  //command to enable the mouse
#define DISABLE_MOUSE         0xA7  //command to disable the mouse
#define WRITE_BYTE_MOUSE  0xD4  //command used to issue a byte directly to the mouse (passed as arg)


//commands passed as arguments of command 0xD4 (WRITE_BYTE_MOUSE)
#define ENB_DATA_REPORT     0xF4  //enable data reporting (stream mode only)
#define DIS_DATA_REPORT    0xF5  //disable data reporting (stream mode only)
#define SET_STRM_MODE     0xEA  //sets stream mode
#define SET_RMT_MODE      0xF0  //sets remote mode
#define READ_DATA         0xEB  //send data packet request
#define RESEND            0xFE  //resends a byte (used when the response is a NACK)




#define MOUSE_FIRST_BYTE BIT(3) //to check if a byte read is the first one 


//Mouse packet byte 1 bits
#define Y_OVF BIT(7)
#define X_OVF BIT(6)
#define PRESSED_MB BIT(2)
#define PRESSED_RB BIT(1)
#define PRESSED_LB BIT(0)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)



//Acknoledgment byte
#define ACK 0xFA   // Acknowledged command or argument 
#define NACK 0xFE  // Invalid byte 
#define ERROR 0xFC // consecutive invalid byte



#endif
