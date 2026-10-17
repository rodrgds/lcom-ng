#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "stdio.h"
#include "i8042.h"
#include "keyboard.h"



static int hook_id;
static int timer_hook_id;


#define DELAY_US 20000
#define WAIT_KBD



#define KBC_HOOK_ID 3




int kbc_subscribe_int(void ) {
	hook_id = KBC_HOOK_ID;
	if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE , &hook_id) != OK || sys_irqenable(&hook_id) != OK) // IRQ_REENABLE is the policy we should specify
		return 0;

	return 0x01 << KBC_HOOK_ID;
}

int timer_subscribe_int1(void ){
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE  , &timer_hook_id) != OK || sys_irqenable(&timer_hook_id) != OK) // IRQ_REENABLE is the policy we should specify
		return 0;
	return 1;
}

int keyboard_unsubscribe_int() {

	if(sys_irqrmpolicy(&hook_id) != OK || sys_irqdisable(&hook_id) != OK)
		return 1;

	return 0;
}

int timer1_unsubscribe_int(){

	if(sys_irqrmpolicy(&timer_hook_id) != OK || sys_irqdisable(&timer_hook_id) != OK)
		return 1;

	return 0;
}

int interrupt() {

	long data;
	unsigned int r;
	int ipc_status;
	message msg;
	short irq_set;

	timer_subscribe_int1();
	irq_set = kbc_subscribe_int();


	while(1)
	{
		/* ANY -> receives msg from any process
		 *  2nd and 3rd arguments are the addresses of variables of type message and int
		 */
		if(r = driver_receive(ANY, &msg, &ipc_status) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if( is_ipc_notify(ipc_status) )	// receive notification of interrupt request. returns true if msg received is notification and false otherwise
		{
			switch(_ENDPOINT_P(msg.m_source)) // m_source contains the endpoint of the msg and _ENDPOINT extracts the process identifier from process's endpoint
			{
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set)
				{
					playtime++;
					unsigned long scancode;
					sys_inb(OUT_BUF, &scancode);	// gets scancode from OUT_BUF
					timer1_unsubscribe_int();
					keyboard_unsubscribe_int();

					if(scancode == LEFT_MAKE_CODE)
					{
						return 2;
					}
					else if(scancode==RIGHT_MAKE_CODE){
						return 1;
					}
					else if(scancode==DOWN_MAKE_CODE){
						return 3;
					}
					else if (scancode==UP_MAKE_CODE) {
						return 0;

					}
					else if (scancode == 0x81) {
						return -2;
					}
					else{
						timer_subscribe_int1();
						irq_set = kbc_subscribe_int();
					}
					tickdelay(micros_to_ticks(DELAY_US));

				}
				else{
					if((playtime%60) == 0 && playtime > 0){
						timer1_unsubscribe_int();
						keyboard_unsubscribe_int();
						return -1;
					}
				}
				playtime++;
				break;

			default:
				break;

			}

		}
		else
		{
			printf("Any interrupt received\n"); 	// Any interrupt received, so anything to do
		}

	}

	return 0;
}
