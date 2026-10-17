#include "libraries.h"

static int counter = 0;

int timer_set_square(unsigned long timer, unsigned long freq)
{
	freq=TIMER_FREQ/freq;

	switch(timer)
	{
	case 0:
		//printf("\tTimer 0 selected \n");
		if(sys_outb(TIMER_CTRL,(TIMER_SEL0|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
		{printf("\t timer_set_square: ERROR setting timer 0 \n");return 1;}
		sys_outb(TIMER_0,((freq) & 0xFF)); /* LSB */
		sys_outb(TIMER_0,((freq) >> 8)); /* MSB */
		break;
	case 1:
		//printf("\tTimer 1 selected \n");
		if(sys_outb(TIMER_CTRL,(TIMER_SEL1|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
		{printf("\t timer_set_square: ERROR setting timer 0 \n"); return 1;}
		sys_outb(TIMER_1,((freq) & 0xFF)); /* LSB */
		sys_outb(TIMER_1,((freq) >> 8)); /* MSB */
		break;
	case 2:
		//printf("\tTimer 2 selected \n");
		if(sys_outb(TIMER_CTRL,(TIMER_SEL2|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
		{printf("\t timer_set_square: ERROR setting timer 0 \n"); return 1;}
		sys_outb(TIMER_2,((freq) & 0xFF)); /* LSB */
		sys_outb(TIMER_2,((freq) >> 8)); /* MSB */
		break;
	}
	return 0;
}

int timer_subscribe_int() {

	if(sys_irqsetpolicy(IRQ_TIMER0,IRQ_REENABLE,&hook_id_timer)!=OK)
	{
		printf("IRQ_SETPOLICY failure /n");
		return -1;
	}
	if(sys_irqenable(&hook_id_timer)!=OK)
	{
		printf("IRQENABLE failure /n");
		return -1;
	}

	return hook_id_timer;
}

int timer_unsubscribe_int() {
	if(sys_irqdisable(&hook_id_timer)!= OK)
	{
		printf("IRQDISABLE failed /n");
		return -1;
	}
	if(sys_irqrmpolicy(&hook_id_timer)!=OK)
	{
		printf("IRQRMPOLICY failed /n");
		return -1;
	}
	return 0;
}

void timer_int_handler(int counter) {
	printf("Counter value : %d \n",counter/60+1);
}

int timer_test_square(unsigned long freq) {

	if((timer_set_square(0, freq)!= 0))
	{
		printf("Wave generation failed /n");
		return -1;
	}
	return 0;
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	message msg;
	int irq_set=timer_subscribe_int();
	int r;

	while(counter<(time*60) ) { /* You may want to use a different condition */
		/* Get a request message. */
		if (r=driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d",  r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				if ((msg.NOTIFY_ARG & BIT(hook_id_timer)) && (counter%60==0)) /* subscribed interruptTimer 0*/
				{ /* process it */
					counter++;
					timer_int_handler(counter);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}

		} else
		{ /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		counter++;
	}
	timer_unsubscribe_int();
	return 0;
}
