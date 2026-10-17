/** @file timer.c
    @author Daniel Marques
    @author João Almeida
    Declaration of functions used to receive interruptions from timer.
    Imported from lab2
*/

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"

void timerSubscribe(int timer_id)
{
	sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_id);
	sys_irqenable(&timer_id);
}

void timerUnsubscribe(int timer_id)
{
	sys_irqrmpolicy(&timer_id);
}
