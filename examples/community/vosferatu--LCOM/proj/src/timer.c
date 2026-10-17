#include "i8254.h"
#include "timer.h"
#include <minix/syslib.h>
#include <minix/drivers.h>



int hook_id = TIMER0_IRQ;

static unsigned long ih_counter = 0;

Timer* newTimer(){
	Timer* timer = (Timer*) malloc(sizeof(Timer));
	timer->ticked = 0;
	timer->counter = 0;

	return timer;
}

void deleteTimer(Timer* timer){
	free(timer);
}

void resetTimerTickedFlag(Timer* timer){
	timer->ticked = 0;
}

void timerHandler(Timer* timer){
	timer->counter++;
	timer->ticked = 1;
}



void timed_scan_handler(unsigned short *time) {

	ih_counter++;
	if(ih_counter % 60 == 0){
		(*time)++;
	}

}

void reset_counter_handler(){
	ih_counter = 0;
}

int timer_set_square(unsigned long timer, unsigned long freq) { //1st week

	int div;

	unsigned char control, config;

	if (freq <= 0)
	{
		printf("\nFrequency must be a positive integer.\n");
		return 1;
	}

	div = TIMER_FREQ/freq;

	timer_get_conf(timer, &config);

	config &= BITMASK_4LSB;

	switch(timer){
		case 0:
			control = config | BIT(4) | BIT(5) | TIMER_SEL0;
			break;
		case 1:
			control = config | BIT(4) | BIT(5) | TIMER_SEL1;
			break;
		case 2:
			control = config | BIT(4) | BIT(5) | TIMER_SEL2;
			break;
		default:
			{
				printf("\nTimer must range from 0 to 2.\n");
				return 1;
			}
			break;
	}

	if (sys_outb(TIMER_CTRL, (unsigned long) control) != OK)
		{
			printf("\nCouldn't read from Timer.\n");
			return 1;
		}

	timer = TIMER_0 + timer; //timer variable becomes TIMER_X port

	if (sys_outb(timer, div) != OK) //push lsb
		{
			printf("\nCouldn't write LSB to Timer.");
			return 1;
		}

	if (sys_outb(timer, div >> 8) != OK)  //push msb
		{
			printf("\nCouldn't write MSB to Timer.");
			return 1;
		}

	return 0;
}

int timer_subscribe_int(void ) { //2nd week

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK) && (sys_irqenable(&hook_id) == OK))
		return BIT(TIMER0_IRQ); //return activated bit by kernel on hook_id. which is value we set
	else
		{
			printf("\nCouldn't subscribe interrupts.\n");
			return -1;  //return negative on failure
		}
}

int timer_unsubscribe_int() { //2nd week

	if ((sys_irqdisable (&hook_id) == OK) && (sys_irqrmpolicy(&hook_id) == OK))
		return 0;
	else
		{
			printf("\nUnable to unsubscribe interrupts.\n");
			return 1;
		}
}

void timer_int_handler() {  //2nd week

	ih_counter = ih_counter +1;

}

int timer_get_conf(unsigned long timer, unsigned char *st) { //1st week

	unsigned long config;

	char read_back;

	if ((timer > 2) || (timer < 0))
			{
				printf("\nTimer must range from 0 to 2.\n");
				return 1;
			}


	read_back = 0 | TIMER_RB_SEL(timer)| TIMER_RB_COUNT_ | BIT(6) | BIT(7);

	if(sys_outb(TIMER_CTRL, read_back) != OK)
		{
			printf("\nCouldn't write to Timer.\n");
			return 1;
		}

	if(sys_inb(TIMER_0+timer, &config ) != OK)
		{
			printf("\nCouldn't read from Timer.\n");
			return 1;
		}

	*st = (unsigned char) config;

	return 0;
}

int timer_test_int(unsigned long time) { //2nd week
	
	//test

	int ipc_status, r, irq_set = timer_subscribe_int();
	unsigned long seconds = 0;
	message msg;

	if (irq_set == -1)
		return 1; //message printed in subscribe_int

	if(time < 0)
	{
		printf("\nTime must be positive integer.\n");
		return 1;
	}

	if (time > 0)
		printf("\t\t START\n");

	while( seconds < time )
	{

		if( (r = driver_receive(ANY, &msg, &ipc_status)) != OK )
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
			{ /* received notification */
				switch (_ENDPOINT_P(msg.m_source))
				{
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) //msg.NOTIFY_ARG works like hook_id received from kernel. So irq_set must have that value
					{ // subscribed interrupt with subscribe_int function
						timer_int_handler();

						if (ih_counter == 60)
						{
							seconds = seconds +1;
							printf("\t %lu seconds passed out of %lu.\n", seconds, time);
							ih_counter = 0;
						}

					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
				}
			} else
				{ /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
				timer_unsubscribe_int();
				}
	}

	timer_unsubscribe_int();

	if (time == 0)
		printf("\tTime argument was 0. 0 seconds passed instantly.\n");
	else printf("\t Timer has reached its value.\n");

	return 0;
}
