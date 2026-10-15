#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IRQ_KEYBOARD  1
#define IRQ_MOUSE     12

#define ESC           0x81
#define CHK_MAKE      BIT(7) //check if it's a make code
#define DELAY         20000 //20 ms delay, as specified in the doxygen
#define ENABLE_INT    BIT(0) //Enable interruption on Output buffer, from keyboard
#define MAX_ATTEMPS   10 //specified in the doxygen


//KBC
#define KBC_ST_REG    0x64 //address to read KBC state
#define KBC_CMD_REG   0x64 //address to write command to KBC
#define KBC_OUT_BUF   0x60 //address to receive scancode
#define KBC_WRITE_CMD 0x60
#define KBC_READ_CMD  0x20
#define MOUSE_WRITE   0xD4

//mouse
#define ENABLE_DATA_REPORTING   0xF4
#define DISABLE_DATA_REPORTING  0xF5
#define LEFT_MOUSE_BUTTON       BIT(0)
#define RIGHT_MOUSE_BUTTON      BIT(1)
#define MIDDLE_MOUSE_BUTTON     BIT(2)
#define BYTE_1_MOUSE            BIT(3) //the first byte of the mouse packet has the bit 3 == 1
#define X_DELTA_SIGN            BIT(4)
#define Y_DELTA_SIGN            BIT(5)
#define X_OVERFLOW              BIT(6)
#define Y_OVERFLOW              BIT(7)

//acknowledgement bytes to bytes written for mouse, that are read via 0x60 (kbc output buffer)
#define ACK                     0xFA //everything OK
#define NACK                    0xFE //invalid byte (something went wrong)

//importante bits for interpreting KBC status
#define INVALID_DATA    BIT(7)
#define TIMEOUT_ERROR   BIT(6)
#define MOUSE_DATA      BIT(5)
#define KEYBOARD_INH    BIT(4)
//#define A2_IN_LINE   BIT(3)
//#define SYS          BIT(2)
#define INPUT_BUF_FULL  BIT(1) //don't write commands or arguments
#define OUTPUT_BUF_FULL BIT(0)

#endif
