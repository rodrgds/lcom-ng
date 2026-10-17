#include "IRQ.h"


IRQ* Subscribe()
{
	IRQ *newIrq=(IRQ*)(malloc(sizeof(IRQ)));
	newIrq->irq_set_timer=timer_subscribe_int();
	newIrq->irq_set_kbd=keyboard_subscribe_int();
	newIrq->irq_set_mouse=mouse_subscribe_int();
	return newIrq;
}



void Unsubscribe()
{
    timer_unsubscribe_int();
    keyboard_unsubscribe_int();
    mouse_unsubscribe_int();
}
