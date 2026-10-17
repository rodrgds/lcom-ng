#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US 20000

/* Ports*/
#define KBD_IRQ1         1    /* keyboard interrupt line */
#define IRQ12_MOUSE     12
#define MOUSE_OUT_BUF   0x60
#define MOUSE_ARGS      0x60 
#define MOUSE_ST_REG    0x64
#define MOUSE_CMD_REG   0x64

/* PS/2 Mouse Byte 1 */
#define LB              BIT(0)
#define RB              BIT(1)
#define MB              BIT(2)
#define MSB_X           BIT(4)
#define MSB_Y           BIT(5)
#define X_OVFL          BIT(6)
#define Y_OVFL          BIT(7)

/* PS/2 Mouse-Related KBC Commands */
#define CMD_READ        0x20
#define CMD_WRITE       0x60
#define MOUSE_DISABLE   0xA7 
#define MOUSE_ENABLE    0xA8
#define MOUSE_INTF_CHK  0xA9  
#define WRITE_TO_MOUSE  0xD4   /* Commands the KBC to forward it's argument to the mouse without any interpretation */

/* Status Register */
#define PARITY          BIT(7) /* Parity error - invalid data */
#define TIMEOUT         BIT(6) /* Timeout error - invalid data */
#define KBC_ERR_SIG     BIT(6) | BIT(7)
#define AUX             BIT(5) /* Mouse data */
#define INH             BIT(4) /* Inhibit flag: 0 if keyboard is inhibited */
#define A2              BIT(3) /* A2 input line: irrelevant for LCOM */
#define SYS             BIT(2) /* System flag: irrelevant for LCOM */
#define IBF             BIT(1) /* Input buffer full - don’t write commands or arguments */
#define OBF             BIT(0) /* Output buffer full - data available for reading */
#define IS_BREAKCODE    BIT(7) /* bit mask to check if a scancode is a breakcode */

/* KBC Errors*/

#define PARITY_ERR  1
#define TIMEOUT_ERR 2
#define IBF_ERR 3
#define AUX_ERR 4

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

/* Commands passed as arguments of command 0xD4(aka "WRITE_TO_MOUSE") */
#define RESET           0xFF /* Mouse reset */
#define RESEND          0xFE /* For serial communications errors */
#define SET_DEFAULT     0xF6 /* Set default values */
#define DT_REP_DISABLE  0xF5 /* In stream mode, should be sent before any other command */
#define DT_REP_ENABLE   0xF4 /* In stream mode only */
#define SET_SMP_RATE    0xF3 /* Sets state sampling rate  */
#define SET_RMT_MODE    0xF0 /* Send data on request only */
#define READ_DATA       0xEB /* Send data packet request */
#define SET_STM_MODE    0xEA /* Send data on events */
#define STATUS_REQUEST  0xE9 /* Get mouse configuration (3 bytes) */
#define SET_RES         0xE8      
#define SET_SCALING_2_1 0xE7 /* Acceleration mode */
#define SET_SCALING_1_1 0xE6 /* Linear mode */

/* PS/2 Mouse Stats Commands */
#define ACK             0xFA /* if everything OK */	
#define NACK            0xFE /* if invalid byte */	
#define ERROR           0xFC /* second consecutive invalid byte */	

#define KBC_KC_ESC 		0x81

/* Read and Write Port*/
#define R_W_PORT        0x64 


#endif
