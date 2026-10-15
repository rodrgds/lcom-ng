#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>


/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the KBC
 */

/* I/O port adresses*/
#define IRQ_KEYBOARD 1
#define IRQ_MOUSE    12

#define KBC_OUT_BUF   0x60    /*Out buffer (to read scancodes from the keyboard and return values)*/
#define KBC_ST_REG    0x64    /*Used to read the KBC state*/
#define KBC_CMD_REG   0x64    /*Used to send commands to the KBC*/

/* Status Register */

#define KBC_PARITY    BIT(7)  /*Parity error - invalid data*/
#define KBC_TIMEOUT   BIT(6)  /*Timeout error - invalid data*/
#define KBC_AUX       BIT(5)  /*Mouse data*/
#define KBC_INH       BIT(4)  /*Inhibit flag: 0 if keyboard is inhibited*/
#define KBC_A2        BIT(3)  /*A2 input line: irrelevant for LCOM*/
#define KBC_SYS       BIT(2)  /*System flag: irrelevant for LCOM*/
#define KBC_IBF       BIT(1)  /*Input buffer full dont write commands or arguments*/
#define KBC_OBF       BIT(0)  /*Output buffer full - data available for reading*/

/* KBC Commands */

#define KBC_RD_CM_B   0x20    /*Read Command Byte*/
#define KBC_WRT_CM_B  0x60    /*Write Command Byte*/
#define KBC_CHECK     0xAA    /*Check KBC (Self-test)*/

/*Keyboard specific commands*/
#define KDB_KB_INTER  0xAB    /*Check Keyboard Interface*/
#define KDB_DISABLE_INTER   0xAD  /*Disable KBD Interface*/
#define KDB_ENABLE_INTER    0xAE  /*Enable KBD Interface*/
#define KBD_ESC_BREAK   0x81     /*ESC breakcode*/

#define KBD_BREAKCODE   BIT(7)  /*Indentify the break code*/
#define KBD_MAKECODE    0x00    /*Identify the makecode*/
#define KBD_TWOBYTES    0xE0    /*Verify if the code uses two bytes*/
#define KBD_ENABLE      0X01    /*Verifies if the KBD is enable*/

/*Mouse specific commands*/
#define MOUSE_DISABLE   0xA7      /*Disables the mouse*/
#define MOUSE_ENABLE    0xA8      /*Enables the mouse*/
#define MOUSE_CHECK_INTER 0xA9
#define MOUSE_WRITE_BYTE  0xD4    /*Commands the KBC to Send its arguments to the mouse*/

/*Mouse packet*/
#define MOUSE_LB  BIT(0)
#define MOUSE_RB  BIT(1)
#define MOUSE_MB  BIT(2)
#define MOUSE_MSB_X BIT(4)
#define MOUSE_MSB_Y BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)

/*Mouse data report*/
#define MOUSE_DIS_DATA_REP  0xF5
#define MOUSE_EN_DATA_REP   0xF4
#define MOUSE_ACK           0xFA
#define MOUSE_NACK          0xFE
#define MOUSE_ERROR         0xFC
#define MOUSE_CONTROL   BIT(3)  /*Bit used to see if the value is a new packt or not*/

/*List of makecodes*/
#define ARROW_LEFT 0x4b
#define ARROW_RIGHT 0x4d
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define A_KEY 0X1E
#define W_KEY 0X11
#define S_KEY 0x1F
#define D_KEY 0X20
#define ENTER_KEY 0X1C

#define DELAY_US    20000





#endif
