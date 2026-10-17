#ifndef _MOUSE_H_
#define _MOUSE_H_

#define KBD_IRQ 			1
#define BC_ESC          0x81
#define DELAY_US     	20000
#define KBC_CMD_REG     0x64
#define IN_BUF			0x60
#define OUT_BUF 		0x60
#define TWO_BYTE   		0xE0
#define LED_CMD			0xED


#define IBF				(BIT(1)) //ou 0x02  ---input buffer full
#define OBF				(BIT(0)) //ou 0x01
#define PAR_ERR			(BIT(7)) //ou 0x80
#define TO_ERR 			(BIT(6)) //ou 0x40

#define ACK				0xFA
#define RESEND			0xFE
#define ERROR			0xFC

#define BREAK_CODE    (BIT(7))




#define MOUSE_IRQ 		12
#define WRITE_MOUSE		0xD4
#define STREAM_MODE		0xEA
#define DATA_MODE		0xF4
#define DIS_STREAM_MODE	0xF5
#define MOUSE_CONFIG	0xE9


#define XOV				BIT(6)
#define YOV 			BIT(7)
#define LB  			BIT(0)
#define MB 				BIT(2)
#define RB				BIT(1)
#define YSIGN			BIT(5)
#define XSIGN			BIT(4)

#define BYTE_3			BIT(3)
#endif

