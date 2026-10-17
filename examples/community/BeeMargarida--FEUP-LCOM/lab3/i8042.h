#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n) (0x01 << (n))

#define KBD_IRQ 			1

#define BC_ESC          0x81

#define DELAY_US     	20000

#define KBC_CMD_REG     0x64

#define STAT_REG        0x64

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



#endif
