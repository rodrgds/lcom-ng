#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "i8254.h"
unsigned long counter = 0;
int hookid = 6;

int timer_subscribe_int(void ) {
	int hookidtemp = hookid;
	if(sys_irqsetpolicy(0,IRQ_REENABLE,&hookidtemp)!=OK){
        return -1;
	}
	if(sys_irqenable(&hookidtemp)!=OK){
		return -1;
	}
    return BIT(hookid); //return dos bits da interrupt mask
}

int timer_unsubscribe_int() {
	int hookidtemp = hookid;
	if(sys_irqrmpolicy(&hookidtemp)!=OK){
		return 1;
	}
	if(sys_irqenable(&hookidtemp)!=OK){
		return 1;
	}
	return 0;
}

