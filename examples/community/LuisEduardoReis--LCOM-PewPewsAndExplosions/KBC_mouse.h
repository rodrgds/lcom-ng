#ifndef __KBC_MOUSE_H_
#define __KBC_MOUSE_H_

#define MOUSE_IRQ 12
#define MOUSE_INT_BIT 3

#define KBC_IOPORT 0x60
#define KBC_STATPORT 0x64

#define KBC_IBF 0x02
#define KBC_OBF 0x01

#define ACK 0xFA
#define RESEND 0xFE
#define ERROR 0xFC

static int MOUSE_HOOK_ID;

static unsigned char packet[3];
static unsigned short packet_count = 0;
static int packet_sync = 0;

typedef struct {
	char xdelta, ydelta;
	int lbutton, mbutton, rbutton;
	int xoverflow, yoverflow;
} mousePacket;


int mouse_subscribe_int();
int mouse_unsubscribe_int();
int mouse_int_handler(mousePacket* out);


int test_asynch(unsigned short duration);

int mouse_recieve_time(unsigned short duration);

int test_config(void);

int mouse_get_config();


#endif
