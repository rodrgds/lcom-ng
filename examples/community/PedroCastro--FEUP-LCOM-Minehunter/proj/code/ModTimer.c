#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "ModTimer.h"

int hook_id;

int timer_subscribe_int(void) {
	hook_id = HOOK_ID;
	sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id); //used to subscribe a notification on every interrupt in the input TIMER0_IRQ
	sys_irqenable(&hook_id); //enables interrupts on the IRQ line associated with the specified hook_id
	return BIT(HOOK_ID);
}

int timer_unsubscribe_int() {
	return (sys_irqrmpolicy(&hook_id)||sys_irqdisable(&hook_id)); //unsubscribes a previous subscription of the interrupt notification associated with the specified hook_id
}

void timer_int_handler() { //increments a counter on every interrupt
	global_counter++;
}
