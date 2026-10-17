#include "timer.h"


int hook_id = TIMER0_IRQ;

int timer_subscribe_int(void ) {

	int hook_id_2 = hook_id;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)
		if (sys_irqenable(&hook_id) == OK)
			return BIT(hook_id_2);
	return -1;
}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_id)== OK)
		if (sys_irqrmpolicy(&hook_id) == OK)
			return 0;
	return -1;
}


