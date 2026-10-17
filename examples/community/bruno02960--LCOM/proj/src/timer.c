#include "timer.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "macros.h"

unsigned int hook_id_timer = 0x00;

int timer_subscribe_int(void) {

	int ret = hook_id_timer;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) == OK)
		if (sys_irqenable(&hook_id_timer) == OK)
			return ret;

	return -1;
}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_id_timer) == OK)
		if (sys_irqrmpolicy(&hook_id_timer) == OK)
			return 0;

	return 1;
}
