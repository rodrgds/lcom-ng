#ifndef _TEST4_H_
#define _TEST4_H_

static int kbdhook=5;

#define STAT_REG 			0x64
#define OUT_BUF				0x60
#define KBD_IRQ				1
#define DELAY_US 			20000

//C@KBD responses
#define ESC_MAKE			0x01
#define ESC_BREAK			0x81
#define SPACE_BREAK			0x39
#define SPACE_MAKE			0xB9
#define A_BREAK				0x1E
#define A_MAKE				0x9E
#define D_BREAK				0x20
#define D_MAKE				0xA0

#define BIT(n) 				(0x01<<(n))

//Status Register
#define OBF					BIT(0) // =1: full; Outbuffer
#define PAR_ERR				BIT(7)
#define TO_ERR				BIT(6)

int kbd_unsubscribe_int();
int kbd_subscribe_int();
unsigned char kbd_readscancode();

#endif
