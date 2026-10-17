#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"
int hook = 2;
int timeCounter = 0;
int timer_set_square(unsigned long timer, unsigned long freq) {

	if(timer==0){
		sys_outb(TIMER_CTRL, 0x37);
		sys_outb(TIMER_0 , freq & 0xff);
		sys_outb(TIMER_0 , freq>>8);
	}
	else if(timer==2) {
		sys_outb(TIMER_CTRL, 0xB7);
		sys_outb(TIMER_2 , freq & 0xff);
		sys_outb(TIMER_2 , freq>>8);
	}

	return 0;
}

int timer_subscribe_int(void ) {

	sys_irqsetpolicy(0, IRQ_REENABLE, &hook);
	sys_irqenable(&hook);
	return 1;
}

int timer_unsubscribe_int() {
	sys_irqdisable(&hook);
	return 1;
}

void timer_int_handler() {
	//timeCounter++;
	//if ((timeCounter % 60) == 0) printf("%d", timeCounter/60);
	//printf("%d\n", timeCounter);
}

int timer_test_square(unsigned long freq) {
	
	timer_set_square(0,freq) ;

	return 0;
}

int timer_test_int(unsigned long time) {

	int ipc_status ;
	message msg ;


	while( timeCounter < time ) {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			//printf("driver_receive failed with: %d", r);
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & 0x4)
					{
						timer_int_handler();
					}
					break;
				default: break;
			}
		}
		else { /* received a standard message, not a notification */ }
	}
	return 1;
}



