#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include "timer.h"

static int g_hookID_TIMER = 3;

int timer_subscribe_int(void ) {

	int initialID = g_hookID_TIMER;

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &g_hookID_TIMER) != OK ) return -1;

	sys_irqenable(&g_hookID_TIMER);

	return initialID;
}

int timer_unsubscribe_int() {

	sys_irqrmpolicy(&g_hookID_TIMER);

	return 0;
}
