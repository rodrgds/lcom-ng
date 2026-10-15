#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_KEYBOARD  1
#define IRQ_MOUSE     12

#define ESC           0x81
#define CHK_MAKE      BIT(7) //check if it's a make code
#define DELAY         20000 //20 ms delay, as specified in the doxygen
#define ENABLE_INT    BIT(0) //Enable interruption on Output buffer, from keyboard
#define MAX_ATTEMPS 10 //specified in the doxygen


//KBC
#define KBC_ST_REG    0x64 //address to read KBC state
#define KBC_CMD_REG   0x64 //address to write command to KBC
#define KBC_OUT_BUF   0x60 //address to receive scancode
#define KBC_WRITE_CMD 0x60
#define KBC_READ_CMD  0x20

//importante bits for interpreting KBC status
#define INVALID_DATA    BIT(7)
#define TIMEOUT_ERROR   BIT(6)
#define MOUSE_DATA      BIT(5) //for lab4
#define KEYBOARD_INH    BIT(4)
//#define A2_IN_LINE   BIT(3)
//#define SYS          BIT(2)
#define INPUT_BUF_FULL  BIT(1) //don't write commands or arguments
#define OUTPUT_BUF_FULL BIT(0)

#endif
