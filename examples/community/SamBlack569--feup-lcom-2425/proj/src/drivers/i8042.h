#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KBC_IRQ       1       // keyboard's IRQ number is 1
#define MOUSE_IRQ     12      // mouse's IRQ number is 12

/*CONSTANTS*/
#define ESC_BREAK     0x81    // breakcode for ESC
#define SC_TWO_BYTE   0xE0    // value to check if the next byte is a two byte scancode
#define KBC_ENABLE    BIT(0)
#define OK            0       // return value for success

/*BIT MASKS*/
#define BREAK_MASK    BIT(7)  // mask to check if the scancode is a breakcode
#define PARITY        BIT(7)  // mask to check if there is a parity error
#define TIME_OUT      BIT(6)  // mask to check if there is a timeout error
#define AUX           BIT(5)  // mask to check if there is auxiliary data

/*I/O PORTS*/
#define OUT_BUF       0x60    // output buffer
#define STATUS_REG    0x64    // status register
#define KBC_READ      0x20    // KBC read command
#define KBC_WRITE     0x60    // KBC write command
      
/*ERRORS*/
#define PARITY_ERR    1       // return value for parity error
#define TIME_OUT_ERR  2       // return value for timeout error


#define AUX_DATA      4       // return value for auxiliary data

#define DELAY_US      20000  

/*MAKECODE AND BREAKCODE*/
#define A_KEY_PRESSED        0x1E    // A Key Makecode
#define A_KEY_RELEASED       0x9E    // A Key Breakcode
#define D_KEY_PRESSED        0x20    // D Key Makecode
#define D_KEY_RELEASED       0xA0    // D Key Breakcode
#define M_KEY_PRESSED        0x32    // M Key Makecode
#define M_KEY_RELEASED       0xB2    // M Key Breakcode
#define LEFT_ARROW_PRESSED   0x4B    // Left Arrow Makecode
#define LEFT_ARROW_RELEASED  0xCB    // Left Arrow Breakcode
#define RIGHT_ARROW_PRESSED  0x4D    // Right Arrow Makecode
#define RIGHT_ARROW_RELEASED 0xCD    // Right Arrow Breakcode

#endif
