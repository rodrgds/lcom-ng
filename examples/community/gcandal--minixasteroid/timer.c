#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "i8254.h"

int timer_subscribe_int(void ) {

	sys_irqsetpolicy(0,IRQ_REENABLE,&timerhook);
	sys_irqenable(&timerhook);

	return 0;
}

int timer_unsubscribe_int() {

	sys_irqrmpolicy(&timerhook);
	sys_irqdisable(&timerhook);

	return 0;
}
