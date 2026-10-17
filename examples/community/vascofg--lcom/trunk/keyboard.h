#ifndef __TEST4_H
#define __TEST4_H
#include <minix/sysutil.h>

#define TIMER0_IRQ 0x00
#define KB_IRQ 0x01
#define SCROLL 0
#define NUM 1
#define CAPS 2
#define TIMER_DELAY 5
#define REPEAT 100

unsigned long kbc_subscribe_int(void );

unsigned char scancode;

int kbc_unsubscribe_int();
int test_scan(void);

int test_leds(unsigned short n, unsigned short *leds);

#endif __TEST4_H
