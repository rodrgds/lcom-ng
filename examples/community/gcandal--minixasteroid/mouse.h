#ifndef _TEST5_H_
#define _TEST5_H_

#define STAT_REG			0x64
#define KBC_CMD_REG 		0x64
#define IN_BUF 				0x60 //or 0x64
#define OUT_BUF				0x60
#define ACK					0xFA
#define RESEND				0xFE
#define ERROR				0xFC
#define OKK					0xAA
#define READ_ID				0x00
#define MOUSE_IRQ			12
#define WAIT_TIME			500000

//KBC Commands
#define READ_COMMAND_BYTE	0x20 // returns Command Byte
#define WRITE_COMMAND_BYTE	0x60 // write Command Byte
#define DISABLE_MOUSE		0xA7
#define ENABLE_MOUSE		0xA8
#define RESET_MOUSE			0xFF
#define WRITE_BYTE			0xD4 //commands the KBC to forward its argument to the mouse
#define DISABLE_STREAM		0xF5
#define ENABLE_DATA			0xF4
#define SET_SAMPLE_RATE		0xF3
#define SET_RESOLUTION		0xE8
#define SET_SCAILING		0xE7
#define ENABLE_STREAM		0xEA

//Mouse commands (as arguments of 0xDA)
#define MOUSE_RESET			0xFF
#define MOUSE_RESEND		0xFE

//Communication bits
#define BIT(n) 				(0x01<<(n))
#define LB					BIT(0)
#define RB					BIT(1)
#define MB					BIT(2)
#define X					BIT(4)
#define Y					BIT(5)
#define X_OVF				BIT(6)
#define Y_OVF				BIT(7)

static unsigned char packet[3];
static unsigned short count=0;
static unsigned long stat;
static int mousehook=3;

int mouse_unsubscribe_int();
int mouse_subscribe_int();
int mouse_int_handler(int* x, int* y, int* posx_inicial, int* posy_inicial, const int H_RES, const int V_RES);


#endif
