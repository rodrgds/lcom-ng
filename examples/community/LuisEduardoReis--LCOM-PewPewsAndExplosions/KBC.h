#ifndef __KBC_H_
#define __KBC_H_


#define KBD_IRQ 1
#define KBD_INT_BIT 2

#define KBD_IOPORT 0x60
#define KBD_STATPORT 0x64

#define KEY_ESC_DOWN 0x01
#define KEY_ESC_UP 0x81

#define KBD_DELAY 20
#define KBD_MAX_POOLS 20

#define KBC_IBF 0x02
#define KBC_OBF 0x01

#define ACK 0xFA
#define RESEND 0xFE
#define ERROR 0xFC



static int KBD_HOOK_ID;

int kbd_recieve(void);
int kbd_subscribe_int();
int kbd_unsubscribe_int();
int kbd_int_handler();


int kbd_wait_response();
int kbd_send_command(unsigned long command);
int kbd_send_command_arg(unsigned long command,unsigned long arg);

int kbd_set_leds(unsigned char leds);




#endif
