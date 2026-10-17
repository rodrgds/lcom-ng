#ifndef _KBD_H
#define _KBD_H

/** @defgroup KBD KBD
 * @{
 */

/*
@brief subscribes the keybord interrupts

@param bit_no returns the bit of the keybord irq

@return 0 if everything is ok
*/

int kbd_subscribe_int(uint8_t *bit_no);

/*
@brief unsubscribes the keybord interrupts

@return 0 if everything is ok
*/

int kbd_unsubscribe_int();

/*
@brief handles the keybord interrupts
*/

void kbd_ih();

/*
@brief checks if its a makecode

@return true if its a makecode, false if it isn't
*/

bool is_make();

/*
@brief checks if its a two byte scancode

@return true if it is two byte, false if it isn't
*/

bool is_twoByte();

/*
@brief polls the scancodes

@return 0 if everything is ok
*/

int kbd_poll();

/*
@brief writes to the status register

@param cmd variable to write to the status register

@return 0 if everything is ok
*/

int kbd_cmd(uint8_t cmd);

/*
@brief reads the out buffer

@param data returns what it read from the out buffer

@return 0 if everything is ok
*/

int read_OB(uint32_t * data);

#define ESC_MAKE	   	      0x01
#define ESC_BREAK	        	0x81
#define A_BREAK             0x9E
#define A_MAKE              0x1E
#define D_BREAK             0xA0
#define D_MAKE              0x20
#define W_BREAK             0x91
#define W_MAKE              0x11
#define S_BREAK             0x9F
#define S_MAKE              0x1F
#define ARROW_RIGHT_BREAK   0xCD
#define ARROW_RIGHT_MAKE    0x4D
#define ARROW_LEFT_BREAK    0xCB
#define ARROW_LEFT_MAKE     0x4B
#define ARROW_UP_BREAK      0xC8
#define ARROW_UP_MAKE       0x48
#define ARROW_DOWN_BREAK    0xD0
#define ARROW_DOWN_MAKE     0x50
#define TWO_BYTE	   	      0xE0

#define KBD_IRQ 	   	1
#define OUT_BUF		   	0x60
#define STATUS_REG		0x64

#define OBF			    0x1
#define IBF           	BIT(1)
#define KBD_READ_CMD  	0x20
#define KBD_WRITE_CMD 	0x60
#define TOPARR_ERROR  	BIT(7) | BIT(6)

#define DELAY_US	   	20000
#endif
