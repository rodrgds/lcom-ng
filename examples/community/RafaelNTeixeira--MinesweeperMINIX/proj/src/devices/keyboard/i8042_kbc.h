#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US 20000

/* Ports*/
#define KBD_IRQ1         1    /* keyboard interrupt line */
#define KBC_OUT_BUF   0x60
#define KBC_ARGS      0x60 
#define KBC_ST_REG    0x64
#define KBC_CMD_REG   0x64


/* Status Register */

#define PARITY      BIT(7) /* Parity error - invalid data */
#define TIMEOUT     BIT(6) /* Timeout error - invalid data */
#define KBC_ERR_SIG BIT(6) | BIT(7)
#define AUX         BIT(5) /* Mouse data */
#define INH         BIT(4) /* Inhibit flag: 0 if keyboard is inhibited */
#define A2          BIT(3) /* A2 input line: irrelevant for LCOM */
#define SYS         BIT(2) /* System flag: irrelevant for LCOM */
#define IBF         BIT(1) /* Input buffer full - don’t write commands or arguments */
#define OBF         BIT(0) /* Output buffer full - data available for reading */
#define IS_BREAKCODE BIT(7) /* bit mask to check if a scancode is a breakcode */

/* KBC Errors*/

#define PARITY_ERR  1
#define TIMEOUT_ERR 2
#define IBF_ERR 3
#define AUX_ERR 4

/* Keys for the Game */

#define W_MAKE 0x11
#define A_MAKE 0x1E
#define S_MAKE 0x1F
#define D_MAKE 0x20
#define ENTER_MAKE 0x1C
#define F_MAKE 0x21
#define R_MAKE 0x13

/* Constants */

#define ESC_MAKE        0x01
#define ESC_BREAK       0x81
#define TWO_BYTE_SCAN   0xE0
#define MAKE_BREAK_DIFF BIT(7) // Bit that usually differs from the break code (key released) to the make code


/* KBC-related commands */

#define RCB          0x20 /* Read Command Back - reads command byte */
#define WCB          0x60 /* Write Command Back - writes command byte */
#define CHECK_KBC    0xAA /* Check KBC (Self-Test) - returns 0x55 if OK, returns 0xFC if error */
#define CHECK_KBI    0xAB /* Check KBD Interface - returns 0 if OK */
#define DIS_KBD_INTF 0xAD /* Disable KBD Interface */
#define ENA_KBD_INTF 0xAE /* Enable KBD Interface */


/** KBC Command Byte */

#define DIS2 BIT(5) // disable mouse interface
#define DIS  BIT(4) // disable keyboard interface
#define INT2 BIT(1) // enable interrupt on OBF, from mouse
#define INT  BIT(0) // enable interrupt on OBF, from keyboard

#endif /* _LCOM_I8042_H */
