#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"

int hook_id=0;

int timer_subscribe_int() {
	int hook_id_tmp=hook_id;
	if (OK==sys_irqsetpolicy(TIMER0_IRQ,  IRQ_REENABLE, &hook_id)) {
		if (OK==sys_irqenable(&hook_id))
			return BIT(hook_id_tmp);
	}
	printf("timer_subscribe failed\n");
	return 1;
}

int timer_unsubscribe_int() {
	int hook_id_tmp=hook_id;
	if (OK==sys_irqdisable(&hook_id)) {
		if (OK == sys_irqrmpolicy(&hook_id_tmp))
			return BIT(hook_id_tmp);
	}
	printf("timer_unsubscribe failed\n");
	return 1;
}
