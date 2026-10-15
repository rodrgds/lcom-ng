#ifndef UTILS_MOUSE
#define UTILS_MOUSE

#include <lcom/lcf.h>

// IRQ line
#define MOUSE_IRQ_LINE 			0x0C

// Addresses
#define INPUT_OUTPUT_ADDR 		0x60
#define STATUS_CMD_ADDR   		0x64

// CMD
#define READ_CMD_BYTE			0x20
#define REQUEST_BYTE_CMD		0xD4
#define ENABLE_DATA_REP_CMD		0xF4
#define DISABLE_DATA_REP_CMD 	0xF5

// Acknowledgement byte
#define SUCCESS_IN_ACK 			0xFA
#define FAILURE_IN_ACK			0xFE
#define FAILURE_IN_ARGS			0xFC

// Status masks
#define PARITY_MSK        		0x80
#define TIMEOUT_MSK       		0x40
#define IBF_MSK		 		  	0x02
#define OBF_MSK 			  	0x01

// Packets masks
#define OVERFLOW_Y_MSK			0x80
#define OVERFLOW_X_MSK			0x40
#define Y_SIGN_MSK				0x20
#define X_SIGN_MSK				0x10
#define SYNC_MASK_BIT 			0x08
#define MIDDLE_BUTTON_MSK		0x04
#define RIGHT_BUTTON_MSK		0x02
#define LEFT_BUTTON_MSK			0x01

// MISC
#define MSB_MSK 				0xFF00
#define LSB_MSK 				0x00FF
#define DELAY 					20000
#define TRIES	 				1	
#define MOUSE_DELAY				200

int sys_inb_8b(uint32_t addr, uint8_t* output);

#endif
