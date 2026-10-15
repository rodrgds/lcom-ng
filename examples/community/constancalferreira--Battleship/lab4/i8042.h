#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>


#define DELAY_US 20000

#define KBD_IRQ 1               // Keyboard IRQ line 
#define MOUSE_IRQ 12            // Mouse IRQ line



//-> I/O ports 

// status register
#define STATUS_REG 0x64         //read KBC state

// ouput buffer
#define OUT_BUF 0x60            //read scancodes from keyboard OR read return values from KBC commands

// input buffer 
#define KBC_ARGS 0x60           //write arguments of KBC commands
#define KBC_CMD_REG 0x64        //write commands to the KBC (see below)




//-> KBC commands

// general
#define KBC_READ_CMD 0x20       //read command byte  -> must be written to KBC_CMD_REG before reading and then we read value from OUT_BUF 
#define KBC_WRITE_CMD 0x60      //write command byte -> must be written to KBC_CMD_REG before writing and new value written to KBC_ARGS (?)
#define KBC_CHECK 0xAA          //check KBC (self-test)

// keyboard
#define KBC_CHECK_KBD 0xAB      //check keyboard interface
#define KBC_DISABLE_KBD 0xAD    //disable kbd interface
#define KBC_ENABLE_KBD 0xAE     //enable kbd interface

// mouse
#define KBC_CHECK_MOUSE 0xA9    //check mouse interface
#define KBC_ENABLE_MOUSE 0xA8   //enable mouse interface
#define KBC_DISABLE_MOUSE 0xA7  //disable mouse interface
#define KBC_WRITE_MOUSE 0xD4    //write byte to mouse,  commands the KBC to forward its argument to the mouse without any interpretation 




//-> command byte bit masks
#define INT BIT(0)              //enable interrupt on OBF, from keyboard
#define INT2 BIT(1)             //enable interrupt on OBF, from mouse
#define DIS BIT(4)              //disable keyboard
#define DIS2 BIT(5)             //disable mouse



//-> status register bit masks
#define PARITY BIT(7)           //parity error
#define TIMEOUT BIT(6)          //timeout error
#define AUX BIT(5)              //mouse data
#define INH BIT(4)              //inhibit flag
#define A2 BIT(3)               //A2 input line
#define SYS BIT(2)              //system flag
#define IBF BIT(1)              //input buffer full
#define OBF BIT(0)              //output buffer full






// KEYBOARD 

//-> constants
#define ESC_MAKE 0x01           //make code for ESC
#define ESC_BREAK 0x81          //break code for ESC

#define IS_BREAK BIT(7)         //bit mask to check if a scancode is a breakcode





// MOUSE

//-> commands  (slide 11)
#define MOUSE_RESET 0xFF        //reset
#define MOUSE_RESEND 0xFE       //resend - for serial communications erros
#define MOUSE_SET_DEFAULT 0xF6  //set default values
#define MOUSE_DISABLE 0xF5      //disable data reporting - in stream mode, should be sent before sending any other cmd!!!
#define MOUSE_ENABLE 0xF4       //enable data reporting - in stream mode only!!!
#define MOUSE_SET_SAMPLE 0xF3   //set sample rate - sets state sampling rate
#define MOUSE_SET_REMOTE 0xF0   //set remote mode - send data only when requested
#define MOUSE_READ_DATA 0xEB    //read data - send data packet request
#define MOUSE_SET_STREAM 0xEA   //set stream mode - send data on events
#define MOUSE_STATUS_REQ 0xE9   //get mouse config (3 bytes)
#define MOUSE_SET_RES 0xE8      //set resolution
#define MOUSE_SET_SCALE_2 0xE7  //set scaling 2:1 - accelaration mode
#define MOUSE_SET_SCALE 0xE6    //set scaling 1:1 - linear mode





//-> constants (slide 4)
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define IS_FIRST_BYTE BIT(3)
#define MOUSE_MSB_X_DELTA BIT(4)
#define MOUSE_MSB_Y_DELTA BIT(5)
#define MOUSE_X_OVERFLOW BIT(6)
#define MOUSE_Y_OVERFLOW BIT(7)


//-> signals (slide 12)
#define MOUSE_ACK 0xFA          //signals a mouse cmd had no issues
#define MOUSE_NACK 0xFE         //signals invalid byte
#define MOUSE_ERROR 0xFC        //signals 2nd consecutive invalid byte




#endif /* _LCOM_I8042_H */
