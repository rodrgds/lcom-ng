static int timer_hook_id;
#include "timer.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "utils.h"

int timer_subscribe_int()
{
	timer_hook_id = TIMER_NOTIFICATION;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
	{
		printf("Error while subscribing - irqsetpolicy().\n");
		return -1;
	}

	if (sys_irqenable(&timer_hook_id) != OK)
	{
		printf("Error while subscribing - irqenable().\n");
		return -1;
	}

	return 0;
}

int timer_unsubscribe_int()
{
	if (sys_irqdisable(&timer_hook_id) != OK)
		return 1;

	if (sys_irqrmpolicy(&timer_hook_id) != OK)
		return 1;
	return 0;
}

void timer_int_handler()
{
	//GLOBAL STATIC VARIABLE
	counter++;
}

int timer_test_int(unsigned long time)
{
	printf("\n");

	//Check input validation
	if (time <= 0)
	{
		return 1;
	}

	int cpu_time = time * DEFAULT_FREQUENCY; //assuming timer is running on default frequency-60 hz
	int irq_set = (1 << timer_hook_id);
	int ipc_status, r;
	message msg;
	int i = 0; //variable to count the number of interruptions

	timer_subscribe_int();

	while (counter < cpu_time)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					timer_int_handler();
					if (counter % DEFAULT_FREQUENCY == 0)
					{
						i++;
						printf("Interruption %d - Counter: %d\n", i, counter);
					}
				}
				break;
			default:
				break;
			}
		} else
		{
			//NOTHING HAPPENS
		}
	}

	timer_unsubscribe_int();
	return 0;
}

int timer_set_square(unsigned long timer, unsigned long freq)
{
	char regCtrl, mostSignBit, leastSignBit;
	int aux;
	unsigned long div = TIMER_FREQ / freq;
	leastSignBit = (char) div;
	mostSignBit = (char) (div >> 8);

	//TIMER 0
	regCtrl = TIMER_SEL0;
	regCtrl = TIMER_BIN | TIMER_SQR_WAVE | TIMER_LSB_MSB;

	aux = sys_outb(TIMER_CTRL, regCtrl);
	if (aux != OK)
	{
		printf("Error in sys_outb() - regCtrl.\n");
		return 1;
	}

	aux = sys_outb(TIMER_0, leastSignBit);
	if (aux != OK)
	{
		printf("Error in sys_outb() - LSB.\n");
		return 1;
	}

	aux = sys_outb(TIMER_0, mostSignBit);
	if (aux != OK)
	{
		printf("Error in sys_outb() - MSB.\n");
		return 1;
	}

	return 0;
}

int timer_test_square(unsigned long freq)
{
	printf("\n");

	//Check input validation
	if (freq <= 0 || freq > TIMER_FREQ)
	{
		printf("Frequency out of bounds.\n");
		return 1;
	}

	timer_set_square(0, freq);
	return 0;
}

int timer_get_conf(unsigned long timer, unsigned char *st)
{
	unsigned char status; //variable that contains the format of the read-back command
	unsigned long information; //variable to store the information from sys_inb()
	int aux; //auxiliar variable to check if system calls ran correctly

	status = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
	aux = sys_outb(TIMER_CTRL, status);

	//check if sys_outb() ran correctly
	if (aux != 0)
	{
		printf("sys_outb() didn't run correctly.\n");
		return 1;
	}

	switch (timer)
	{
	//TIMER 0
	case 0:
		aux = sys_inb(TIMER_0, &information);
		break;
		//TIMER 1
	case 1:
		aux = sys_inb(TIMER_1, &information);
		break;
		//TIMER 2
	case 2:
		aux = sys_inb(TIMER_2, &information);
		break;
	default:
		return 1;
	}

	//check if the system call sys_inb() was done correctly
	if (aux != 0)
	{
		printf("sys_inb() didn't run correctly.\n");
		return 1;
	}

	*st = information;
	return 0;
}

int timer_display_conf(unsigned char conf)
{
	int output, nullCount, accessType, programmedMode, bcd;

	bcd = conf & BIT(0);
	programmedMode = conf >> 1 & (BIT(2) | BIT(1) | BIT(0));
	accessType = conf >> 4 & (BIT(1) | BIT(0));
	nullCount = conf >> 5 & (BIT(0));
	output = conf >> 6 & (BIT(0));

	printf("---------------READ BACK STATUS FORMAT---------------\n\n");

	//OUTPUT
	if (output == 0)
		printf("Output: False\n");
	else
		printf("Output: True\n");
	//NULLCOUNT
	if (nullCount == 0)
		printf("Null Count: False\n");
	else
		printf("Null Count: True\n");
	//ACCESS TYPE
	if (accessType == 1)
		printf("Type of Access: LSB\n");
	else if (accessType == 2)
		printf("Type of Access: MSB\n");
	else
		printf("Type of access: LSB followed by MSB.\n");

	//PROGRAMMED MODE
	switch (programmedMode)
	{
	case 0:
		printf("Programmed Mode: INTERRUPT ON TERMINAL COUNT.\n");
		break;
	case 1:
		printf("Programmed Mode: HARDWARE RETRIGGERABLE ONE-SHOT.\n");
		break;
	case 2:
	case 6:
		printf("Programmed Mode: RATE GENERATOR.\n");
		break;
	case 3:
	case 7:
		printf("Programmed Mode: SQUARE WAVE MODE.\n");
		break;
	case 4:
		printf("Programmed Mode: SOFTWARE TRIGGERED STROBE.\n");
		break;
	case 5:
		printf("Programmed Mode: HARDWARE TRIGGERED STROBE (RETRIGGERABLE).\n");
		break;
	}

	//BCD
	if (bcd == 0)
		printf("BCD: Binary\n");
	else
		printf("BCD: Binary Coded Decimal.\n");

	return 0;
}

int timer_test_config(unsigned long timer)
{
	printf("\n");

	//Check input validation
	if (timer < 0 || timer > 2)
	{
		printf("Invalid timer. Use between 0 and 2.\n");
		return 1;
	}

	unsigned char status;
	int aux;
	aux = timer_get_conf(timer, &status);

	if (aux != 0)
	{
		printf("timer_get_conf() didn't run correctly.\n");
		return 1;
		if (sys_outb(TIMER_CTRL, status))
		{
			printf("Sys_outb error.\n");
			return 1;
		}
	}

	aux = timer_display_conf(status);
	if (aux != 0)
	{
		printf("timer_display_conf() didn't run correctly.\n");
		return 1;
	}

	return 0;
}
