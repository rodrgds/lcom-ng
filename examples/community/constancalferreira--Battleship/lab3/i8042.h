#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>


#define DELAY_US 20000

#define KBD_IRQ 1     // Keyboard IRQ line 



//-> I/O ports (slide 17)

// status register
#define STATUS_REG 0x64      //read KBC state

// ouput buffer
#define OUT_BUF 0x60         //read scancodes from keyboard OR read return values from KBC commands

// input buffer 
#define KBC_ARGS 0x60        //write arguments of KBC commands
#define KBC_CMD_REG 0x64     //write commands to the KBC (see below)




//-> KBC commands
#define KBC_READ_CMD 0x20       //read command byte  -> must be written to KBC_CMD_REG before reading and then we read value from OUT_BUF 
#define KBC_WRITE_CMD 0x60      //write command byte -> must be written to KBC_CMD_REG before writing and new value written to KBC_ARGS (?)
#define KBC_CHECK 0xAA          //check KBC
#define KBC_CHECK_KBD 0xAB      //check KBC interface
#define KBC_DISABLE_KBD 0xAD    //disable KBC
#define KBC_ENABLE_KBD 0xAE     //enable KBC


//-> status register bit masks
#define PARITY BIT(7)       //parity error
#define TIMEOUT BIT(6)      //timeout error
#define AUX BIT(5)          //mouse data
#define INH BIT(4)          //inhibit flag
#define A2 BIT(3)           //A2 input line
#define SYS BIT(2)          //system flag
#define IBF BIT(1)          //input buffer full
#define OBF BIT(0)          //output buffer full



//-> constants
#define ESC_MAKE 0x01    //make code for ESC
#define ESC_BREAK 0x81   //break code for ESC


/* important!
a scancode can have 2 forms:
- makecode: code generated when we press a key
- breakcode: code generated when we release it

globaly, the breakcode is diff from the original makecode only on the MSB 
*/
#define IS_BREAK BIT(7)   //bit mask to check if a scancode is a breakcode



#endif /* _LCOM_I8042_H */
