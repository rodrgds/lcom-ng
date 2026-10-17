#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"

int g_hook=2;
int g_counter=0;

int timer_get_conf(unsigned char timer, unsigned char *st) {

	//see if the timer is between 2 and 0 (timer 0, 1 or 2)

	if(timer>2){
		printf("Invalid argument! Try again!\n");
		return -1;
	}
	if(timer<0)	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}


	// select the timer

	switch (timer) {
	case 0:
		sys_outb (TIMER_CTRL, TIMER_RB_SEL(0) | TIMER_RB_CMD | TIMER_RB_COUNT_);
		sys_inb (TIMER_0, (unsigned long*) st);
		break;
	case 1:
		sys_outb (TIMER_CTRL, TIMER_RB_SEL(1) | TIMER_RB_CMD | TIMER_RB_COUNT_);
		sys_inb (TIMER_1, (unsigned long*) st);
		break;
	case 2:
		sys_outb (TIMER_CTRL, TIMER_RB_SEL(2) | TIMER_RB_CMD | TIMER_RB_COUNT_);
		sys_inb (TIMER_2, (unsigned long*) st);
		break;
	}

	//ctrl_word is written in timer control register
}

int timer_display_conf(unsigned char conf) {

	//display of the counter that was chosen

	switch(conf & TIMER_RB_CMD)
	{
	case TIMER_SEL0:
		printf("COUNTER: Timer 0 \n");
		break;
	case TIMER_SEL1:
		printf("COUNTER: Timer 1 \n");
		break;
	case TIMER_SEL2:
		printf("COUNTER: Timer 2 \n");
		break;
	default:
		printf("COUNTER: Invalid \n");
		break;
	}

	// display of the type of access

	switch(conf & TIMER_LSB_MSB)
	{
	case TIMER_LSB:
		printf("TYPE OF ACCESS: LSB \n");
		break;
	case TIMER_MSB:
		printf("TYPE OF ACCESS: MSB \n");
		break;
	case TIMER_LSB_MSB:
		printf("TYPE OF ACCESS: 1st LSB then MSB \n");
		break;
	default:
		printf("TYPE OF ACCESS: Invalid \n");
		break;
	}

	//display of the operating mode

	switch (conf & (TIMER_SQR_WAVE | TIMER_RATE_GEN))
	{
	case TIMER_SQR_WAVE:
		printf("OPERATING MODE: Square Wave Generator (3) \n");
		break;
	case TIMER_RATE_GEN:
		printf("OPERATING MODE: Rate Generator (2) \n");
		break;
	default:
		printf("OPERATING MODE: Invalid \n");
		break;
	}

	//display of the counting mode

	switch(conf & TIMER_BCD)
	{
	case TIMER_BIN:
		printf("COUNTING MODE: Binary \n");
		break;
	default:
		printf("COUNTING MODE: BCD \n");
		break;
	}

	return 0;
}


int timer_test_config(unsigned char timer)
{
	unsigned char st;

	//check if the timer is between the right values
	if (timer > 2)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}

	if (timer < 0)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}

	// calls the functions: one that read the configuration of the timer
	// and other that displays in a human friendly way the status/configuration of a timer

	timer_get_conf(timer, &st);
	timer_display_conf(st);

	return 0;
}


int timer_set_frequency(unsigned char timer, unsigned long freq) {

	//see if the timer and the freq are between the right values

	if (freq <= 0)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}

	if (freq > TIMER_FREQ)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}

	if (timer > 2)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}

	if (timer < 0)
	{
		printf("Invalid argument! Try again!\n");
		return -1;
	}


	unsigned char flags = 0;

	//calling get conf in order to get the chosen timer configuration

	timer_get_conf(timer, &flags);

	unsigned char g_counter;
	unsigned long divider = TIMER_FREQ / freq;

	if (divider > USHRT_MAX)
	{
		printf("Timer frequency is too low!\n");
		return -1;
	}

	flags = (flags << 4) - 1;

	//Select the g_counter according to value of flags variable

	switch (timer) {
	case 0:
		flags |= TIMER_SEL0;
		g_counter = TIMER_0;
		break;
	case 1:
		flags |= TIMER_SEL1;
		g_counter = TIMER_1;
		break;
	case 2:
		flags |= TIMER_SEL2;
		g_counter = TIMER_2;
		break;
	default:
		printf("Invalid timer!");
		return 1;
	}

	flags |= TIMER_LSB_MSB;

	// System call sys_outb
	if (sys_outb(TIMER_CTRL, flags) != 0)
	{
		printf("System call sys_outb() failed! \n");
		return -1;
	}

	if (sys_outb(g_counter, divider & 0xff) != 0)
	{
		printf("System call sys_outb() failed! \n");
		return -1;
	}

	if (sys_outb(g_counter, (divider >> 8) & 0xff) != 0)
	{
		printf("System call sys_outb() failed! \n");
		return -1;
	}

	return 0;
}

int timer_subscribe_int(void ) {

	int BIT_MASK = BIT(g_hook);

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &g_hook) != 0)
	{
		printf("System call sys_irqsetpolicy() failed! \n");
		return -1;
	}

	if (sys_irqenable(&g_hook) != 0)
	{
		printf("System call sys_irqenable() failed! \n");
		return -1;
	}

	return BIT_MASK;
}

int timer_unsubscribe_int()
{
	if (sys_irqdisable(&g_hook) == 0 && sys_irqrmpolicy(&g_hook) == 0)
	{
		return 0;
	}

	return -1;
}

void timer_int_handler()
{
	g_counter++;
}


int timer_test_time_base(unsigned long freq)
{
	return timer_set_frequency(0, freq);
}

int timer_test_int(unsigned long time)
{
	int ipc_status, r;
	message msg;
	int irq_set = timer_subscribe_int();

	if (irq_set == -1) {
		printf("error subscribing timer interrupts...\n");
		return -1;
	}

	while (g_counter < 60 * time)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			//received notification
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:

				//hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set)
				{
					//subscribed interrupt
					timer_int_handler();

					if (g_counter % 60 == 0)
					{
						printf("Passed %d seconds \n", g_counter/60);
					}
				}

				break;
			}
		}
	}

	return timer_unsubscribe_int();
}
