#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 *
 */

#define IRQ1 1
#define IRQ12 12
#define MAKE_BREAK 0x80



/*PORTS*/
#define OUT_BUFF   0x60
#define ARG_REG    0x60
#define STATUS_REG 0x64
#define CMD_REG    0x64



#define KBC        0x64




#define ESC_KEY    0x81
#define TWO_BYTES  0xE0
#define RCMD       0x20
#define WCMD       0x60
#define DELAY_US   20000

/*Errors*/
#define PARITY       BIT(7)
#define TIMEOUT      BIT(6)
#define IBUFFER_FULL BIT(1)
#define OBUFFER_FULL  BIT(0)


/*MOUSE COMMANDS*/
#define READ_CB         0x20
#define WRITE_CB        0x60
#define DISABLE_MOUSE   0xA7
#define ENABLE_MOUSE    0xA8
#define WRITE_BYTE_MOUSE 0xD4 
#define ENABLE_DATA     0xF4
#define DISABLE_DATA    0xF5



/*MOUSE ERRORS*/

#define ACK   0xFA
#define NACK  0xFE
#define ERROR 0xFC


/*KEYBOARD SHORTCUTS*/

#define A_KEY 0x1E
#define D_KEY 0x20


#endif /* _LCOM_I8042_H */
