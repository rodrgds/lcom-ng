#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"

int hook_id = 0;
int counter = 0;

Timer* initTimer() {
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->counter = 0;
	timer->ticks = 0;

	return timer;

}

void timerHandler(Timer* timer) {
	timer->ticks++;
	if (timer->ticks % 60 == 0)
		timer->counter++;

}

void stopTimer(Timer* timer) {
	free(timer);
}

void resetTimer(Timer* timer) {
	timer->ticks = 0;
	timer->counter = 0;
}

void printErrors(char * msg) {
	if (DEBUG)
		printf("%s", msg);
}

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	unsigned long timerfreqdiv;
	unsigned char timerconfig = 0, newtimerconfig = 0;
	unsigned char lsb, msb;

	if (freq <= 0) {
		printErrors("Frequency can't be zero or under\n");
		return FREQ_NEG_ERROR;
	}

	if (freq > TIMER_FREQ) {
		printErrors("Frequency bigger than TIMER_FREQ\n");
		return FREQ_OVERFLOW_ERROR;
	}

	timerfreqdiv = TIMER_FREQ / freq;
	lsb = timerfreqdiv & 0xFF;
	msb = (timerfreqdiv >> 8) & 0xFF;
	if (timer_get_conf(0, &timerconfig) != Ok)
		return TIMER_GET_CONF_ERROR;

	newtimerconfig = timerconfig & 0x0F;
	newtimerconfig = newtimerconfig | TIMER_LSB_MSB;
	newtimerconfig = newtimerconfig | TIMER_SEL0;
	if (sys_outb(TIMER_CTRL, newtimerconfig) != Ok)
		return SYS_OUT_ERROR;

	if (sys_outb(TIMER_0, lsb) != Ok)
		return SYS_OUT_ERROR;
	if (sys_outb(TIMER_0, msb) != Ok)
		return SYS_OUT_ERROR;

	return Ok;
}

int timer_subscribe_int(void) {

	int temp_hook_id = hook_id;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != Ok)
		return IRQ_SET_ERROR;

	if (sys_irqenable(&hook_id) != Ok)
		return IRQ_ENAB_ERROR;

	return BIT(temp_hook_id);
}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) != Ok)
		return IRQ_DISAB_ERROR;
	if (sys_irqrmpolicy(&hook_id) != Ok)
		return IRQ_REM_ERROR;

	return Ok;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	unsigned long stout;
	port_t timerport;
	unsigned long timerselect;

	switch (timer) {
	case 0: //timer 0
		timerport = TIMER_0;
		timerselect = TIMER_RB_SEL(0);
		break;
	case 1: //timer1
		timerport = TIMER_1;
		timerselect = TIMER_RB_SEL(1);
		break;
	case 2: //timer2
		timerport = TIMER_2;
		timerselect = TIMER_RB_SEL(2);
		break;
	default:
		return TIMER_CONF_ERROR;
	}

	stout = TIMER_RB_CMD | timerselect | TIMER_RB_COUNT_;
	if (sys_outb(TIMER_CTRL, stout) != Ok) {
		return SYS_OUT_ERROR;

	}

	if (sys_inb(timerport, (unsigned long int *) st) != Ok)
		return SYS_IN_ERROR;

	return Ok;
}


