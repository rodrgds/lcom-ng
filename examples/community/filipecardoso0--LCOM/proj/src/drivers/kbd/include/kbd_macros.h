#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/*Register Macros*/
#define OBF         0x60    /*Output buffer port register*/
#define IBF         0x60    /*Input buffer port register*/
#define STATREG     0x64    /*Status register port register*/
#define CMDREG      0x64
#define KBD_IRQ     0x01    /*KBD IRQ line*/
/* #define IRQ_REENABLE 0x1	   reenable IRQ line after interrupt */

/*Scancode Macros*/
#define TWO_BYTES   0XE0    /*Signals the scancode is two bytes long*/

/*Command Macros*/
#define R_CMDBYTE   0x20    /*Returns Command Byte*/
#define W_CMDBYTE   0X60    /*Writes Command Byte*/
#define KBC_CHECK   0xAA    /*Checks if KBC is working properly*/
#define KBD_CHECK   0xAB    /*Checks KBD interface*/
#define KBD_OFF     0xAD    /*Disables KBD interface*/
#define KBD_ON      0xAE    /*Enables KBD interface*/

/*KB Macros*/
#define KBC_OK      0x55    /*KBC is working properly*/
#define KBC_ERR     0xFC    /*KBC encountered an error*/
#define KBD_OK      0x00    /*KBD is working properly*/

/*Status Register Macros*/
#define PAR_ERROR   BIT(7)  /*Parity Error*/
#define TIMEOUT     BIT(6)  /*Timeout Error*/
#define AUX         BIT(5)  /*Checks if Mous is being interrupted*/
#define IBF_FULL    BIT(1)  /*Input Buffer is full (can't write)*/
#define OBF_FULL    BIT(0)  /*Output Buffer is full (can read)*/

/*Byte Macros*/
#define _LSB 0x00FF         /*Least Significant Byte*/
#define _MSB 0xFF00         /*Most Significant Byte*/

/* CommandByte macros*/
#define INT         BIT(0) /*enable interrupt on OBF, from keyboard*/

/*Tick*/

#define DELAY_US 20000

#endif /* _LCOM_I8042_H */
