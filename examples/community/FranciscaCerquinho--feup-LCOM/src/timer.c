#include "timer.h"

int counter=0;
int hook_id;


Timer *newTimer()
{
	Timer *auxTimer;
	auxTimer=(Timer *)malloc(sizeof(Timer));

	auxTimer->counter=0;
	auxTimer->frame=0;
	auxTimer->ticked=0;
	return auxTimer;
}

void DestroyTimer(Timer *auxTimer)
{
	free(auxTimer);
}

void timerUpdate(Timer *auxTimer)
{
	auxTimer->counter++;
	auxTimer->frame++;

	if(auxTimer->frame > 60)
	{
		auxTimer->frame%=60;
	}
	auxTimer->ticked=1;
}

void resetTimer(Timer *auxTimer)
{
	auxTimer->ticked=0;
}


int timer_set_square(unsigned long timer, unsigned long freq) {

	int test;
	unsigned long newFreq;
	newFreq=TIMER_FREQ/freq;

	test=sys_outb(TIMER_CTRL,timer);
	if(test!=0)
	{
		return 1;
	}

	test=sys_outb(TIMER_0,newFreq);
	if(test!=0)
	{
		return 1;
	}

	test=sys_outb(TIMER_0,newFreq<<8);
	if(test!=0)
	{
		return 1;
	}

	return 0;
}

int timer_subscribe_int() {

	//this function subscribes and enables Timer0 interrupts

	hook_id=TIMER0_IRQ;

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!=0)
	{
		return -1;
	}
	return BIT(TIMER0_IRQ);
}

int timer_unsubscribe_int() {

	//this function unsubcribes Timer0 interrupts
	if (sys_irqrmpolicy(&hook_id)!=0)
		return 1;

	return 0;

}

void timer_int_handler() {
	//Timer0 interrupt handler and increments counter
	counter++;

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned char readback=0;
	unsigned char selec=0;
	readback=readback | TIMER_RB_CMD | TIMER_RB_COUNT_;
	if(timer==0)
	{
		readback = readback |  TIMER_RB_SEL(0);
		selec = selec | TIMER_0;
	}
	else if(timer==1)
	{
		readback = readback |  TIMER_RB_SEL(1);
		selec = selec | TIMER_1;
	}
	else if(timer==2)
	{
		readback = readback |  TIMER_RB_SEL(2);
		selec = selec | TIMER_2;
	}
	else{
		printf("Mal sucedido\n");
		return 1;
	}
	if(sys_outb(TIMER_CTRL,readback) != 0)
	{
		printf("Chamada ao kernel não permitida\n");
		return 1;
	}

	if(sys_inb(selec,(unsigned long*) st) != 0)
	{
		printf("Chamada ao kernel não permitida\n");
		return 1;
	}

	printf("Bem sucedido\n");
	return 0;
}

int timer_display_conf(unsigned char conf) {

	if(BIT(7) & conf == BIT(7))
	{
		printf("Output 1\n");
	}
	else printf("Output 0\n");

	if(BIT(6) & conf == BIT(6))
	{
		printf("Null count = 1\n");
	}
	else printf("Null count = 0\n");


	switch(conf & (BIT(5)|BIT(4)))
	{
	case TIMER_LSB:
		printf("LSB\n");
		break;
	case TIMER_MSB:
		printf("MSB\n");
		break;
	case TIMER_LSB_MSB:
		printf("LSB followed by MSB\n");
		break;
	default: printf("ERRO no acesso\n");
	return 1;
	}

	if(conf & BIT(3) == 0)
	{
		if(conf & BIT(2) == 0)
		{
			if(conf & BIT(1) == 0)
				printf("0\n");
			else printf("1\n");
		}
		else
		{
			if(conf & BIT(1) == 0)
				printf("2\n");
			else
				printf("3\n");
		}
	}
	else
	{
		if(conf & BIT(2) == 0)
		{
			if(conf & BIT(1) == 0)
				printf("4\n");
			else printf("5\n");
		}
		else
		{
			if(conf & BIT(1) == 0)
				printf("2\n");
			else printf("3\n");
		}

	}



	if(conf & BIT(0) == 1)
	{
		printf("BCD\n");
	}else if(conf & BIT(0) == 0)
	{
		printf("Binary\n");
		return 0;
	}
}


int timer_test_square(unsigned long freq) {

	if (timer_set_square(TIMER_0, freq)!=0)
		return 1;
	else
		return 0;

}

int timer_test_int(unsigned long time) {

	int ipc_status;
	int irq_set;
	int r;
	message msg;

	irq_set=timer_subscribe_int();
	if(timer_subscribe_int()<0)
		return 1;

	while( counter <=  (time * 60 ) ){ /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();
					if(counter%60==0)
					{
						printf("%d   seconds\n", counter/60);
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if(timer_unsubscribe_int()!=0){
		return 1;
	}
	return counter/60;
}

int timer_test_config(unsigned long timer) {

	if(timer==0 ||  timer==1 || timer==2)
	{
		unsigned char conf;
		timer_get_conf(timer, &conf);
		timer_display_conf(conf);
		printf("Funcionou\n");
		return 0;
	}
	printf("Erro\n");
	return 1;
}
