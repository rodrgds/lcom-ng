#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int counter;
int irq_set;
static int timer_hook_id = 0;
int timer0_id;

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char conf, tmp = 0x00, MSB, LSB;
	unsigned long div;
	timer_get_conf(timer,&conf);
	tmp = conf & BIT(0);
	tmp |= (TIMER_LSB_MSB | TIMER_SQR_WAVE);
	if (timer != 0)
	{
		if (timer = 1)
			tmp |= TIMER_SEL1;
		else if (timer = 2)
			tmp |= TIMER_SEL2;
		else return 1;
	}
	div = TIMER_FREQ/freq;
	LSB = div;
	MSB = div >> 8;
	if (sys_outb(TIMER_CTRL, tmp) != OK)
		return 1;
	if (sys_outb(TIMER_0+timer, LSB) != OK)
		return 1;
	if (sys_outb(TIMER_0+timer, MSB) != OK)
		return 1;
	return 0;
}

int timer_subscribe_int() {
	timer_hook_id = 0;
	timer0_id= BIT(timer_hook_id);
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
		return 1;
	if (sys_irqenable(&timer_hook_id) != OK)
		return 1;
	return timer0_id;
}

int timer_unsubscribe_int() {
	if (sys_irqrmpolicy(&timer_hook_id) != OK)
		return 1;
	if (sys_irqdisable(&timer_hook_id) != OK)
		return 1;
	return 0;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned char tempbyte;
	tempbyte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
	if (sys_outb(TIMER_CTRL, tempbyte) != OK)
		return 1;
	unsigned long aux;
	if (sys_inb(TIMER_0+timer,&aux) != OK)
		return 1;
	*st = aux;
	return 0;
}
