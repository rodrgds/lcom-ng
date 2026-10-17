#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"
// Variaveis globais
static int hook_id;

Timer* newTimer()
{
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->counter = 0;
	timer->ticked = 0;
	return timer;
}
void resetTimer(Timer* timer)
{
	timer->ticked = 0;
}
void deleteTimer(Timer* timer)
{
	free(timer);
}


int timer_set_square(unsigned long timer, unsigned long freq) {
	// configura o read-back command
	unsigned char read_back = TIMER_SQR_WAVE | TIMER_BIN | (TIMER_0 + timer)
			| TIMER_LSB_MSB;
	// calcula a frequencia a ser utilizada
	unsigned long new_freq = TIMER_FREQ / freq;
	// tem que se passar 1 byte de cada vez, o lsb, seguido do msb
	unsigned char lsb = new_freq & 0xFF;
	unsigned char msb = new_freq >> 8;
	if (new_freq <= TIMER_FREQ && new_freq >= TIMER_MIN_FREQ) {
		// informa-se o controlar e de seguida passa-se o lsb e o msb
		sys_outb(TIMER_CTRL, read_back);
		sys_outb(TIMER_0 + timer, lsb);
		sys_outb(TIMER_0 + timer, msb);
		return 0;
	} else {
		printf("ERROR: freq out of range \n");
		return 1;
	}
}

int timer_subscribe_int(void) {
	// atualiza hook_id, passando a ser 0
	hook_id = TIMER_HOOK_BIT;

	int bitmask = BIT(hook_id);
	/*The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so
	 that the generic interrupt handler will acknowledge the interrupt. */
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0
			|| sys_irqenable(&hook_id) != 0) {
		printf("\ntimer_subscribe_int() failed \n");
		return -1;
	} else {
		return bitmask;
	}
}

int timer_unsubscribe_int() {
	/*sys_irqrmpolicy(int *hook_id) Unsubscribes a previous interrupt notification.

	 sys_irqdisable(int *hook_id) Masks an interrupt line associated
	 with a previously subscribed interrupt notification.*/

	if (sys_irqdisable(&hook_id) != 0) {
		printf("\ntimer_unsubscribe_int() failed \n");
		return 1;
	}

	if (sys_irqrmpolicy(&hook_id) != 0) {
		printf("\ntimer_unsubscribe_int() failed \n");
		return 1;
	}

	return 0;
}

void timer_handler(Timer* timer) {
	timer->ticked = 1;
	timer->counter++;
}
