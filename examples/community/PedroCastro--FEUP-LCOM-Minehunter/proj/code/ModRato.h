#ifndef __MODRATO_H
#define __MODRATO_H

#define BIT(n) (0x01<<(n))

#define STATUS_PORT 		0x64
#define OUT_BUF 			0x60
#define IN_BUF 				0x60
#define DELAY_US 			20000
#define TWO_BYTE_CODE 		0xE0
#define BREAK_CODE_ESC 		0x81
#define STAT_REG 			0x64
#define KBC_CMD_REG 		0x64
#define KBC_CMD_MOUSE		0xD4
#define ENABLE_PACKETS	 	0xF4
#define DSM 				0xF5
#define ENABLE_MOUSE		0xA8
#define STATUS_REQUEST		0XE9
#define ACK					0xFA
#define PAR_ERR         	BIT(7)
#define TO_ERR              BIT(6)
#define IBF                 BIT(1)
#define OBF                 BIT(0)
#define RESEND              0xFE
#define ERROR               0xFC
#define MOUSE_IRQ 			12
#define MOUSE_HOOK_ID		2

#define MOUSE_MULTIPLIER	1


typedef struct{
	int x,y;

	unsigned long packets[3];

    int leftButtonDown;

    int rightButtonDown;

    int leftButtonReleased;

    int rightButtonReleased;

}Rato;

static Rato* rato = 0;

///@brief checks if there is already a mouse, if yes return its addr , if not creates a new Rato
///return pointer to mouse
Rato* getRato();

///@brief creates a new Rato struct
///@return pointer to the new Rato struct
Rato* newRato();

///@brief enables the interrupts of the mouse
///@return Returns bit order in interrupt mask; negative value on failure
int mouse_subscribe_int();

///@brief disables the interrupts of the mouse
///@return Return 0 upon success and non-zero otherwise
int mouse_unsubscribe_int();

///@brief deals with mouse interrupts
///@return 0 upon success, non-zero upon failure
int mouse_int_handler();

///@brief updates the mouse status
void updateMouse();

///@brief confirms if the first packets really is the first packet
///@param mouse_byte
///@return 1 upon success , 0 upon failure
int first_byte(unsigned long mouse_byte);

///@brief prints mouse according to color
///@param cor
void drawRato(int cor);

///@brief disables mouse's stream mode and initializes mouse's data packets
///@return success
int enable_packets();


#endif
