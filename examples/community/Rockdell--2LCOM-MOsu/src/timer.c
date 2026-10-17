#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"

int timer_hook_id = TIMER0_IRQ;
unsigned int counter = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	unsigned char output;
	int valid_in = timer_get_conf(timer, &output);

	if(!valid_in)
		return 1;

	unsigned long new_freq = TIMER_FREQ / freq;

	unsigned long temp = 0x00;

	//Verify bit 0, 1, 2 and 3 aren't changed (operating mode and bcd)
	if ((output & BIT(0)) != 0)
		temp = temp | BIT(0);

	if ((output & BIT(1)) != 0)
		temp = temp | BIT(1);

	if ((output & BIT(2)) != 0)
			temp = temp | BIT(2);

	if ((output & BIT(3)) != 0)
			temp = temp | BIT(3);

	switch (timer) {
		case 0:
		{
			//Warns TIMER_CTRL
			int valid1 = sys_outb(TIMER_CTRL, temp);
			//LSB
			int valid2 = sys_outb(TIMER_0, new_freq);
			//MSB
			int valid3 = sys_outb(TIMER_0, new_freq >> 8);

			if(valid1 | valid2 | valid3)
				return 1;

			break;
		}
		case 1:
		{
			temp = temp | BIT(6);

			//Warns TIMER_CTRL
			int valid1 = sys_outb(TIMER_CTRL, temp);
			//LSB
			int valid2 = sys_outb(TIMER_0, new_freq);
			//MSB
			int valid3 = sys_outb(TIMER_0, new_freq >> 8);

			if(valid1 | valid2 | valid3)
				return 1;
			break;
		}
		case 2:
		{
			temp = temp | BIT(7);

			//Warns TIMER_CTRL
			int valid1 = sys_outb(TIMER_CTRL, temp);
			//LSB
			int valid2 = sys_outb(TIMER_0, new_freq);
			//MSB
			int valid3 = sys_outb(TIMER_0, new_freq >> 8);

			if(valid1 | valid2 | valid3)
				return 1;
			break;
		}
	}

	printf("Timer alterado!");

	return 0;
}

int timer_subscribe_int(void ) {

	int timer_id = timer_hook_id;
    int valid = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);

	if(valid != OK)
		return -1;
	else
		return timer_id;
}

int timer_unsubscribe_int() {

	int valid = sys_irqrmpolicy(&timer_hook_id);

	if(valid != OK)
		return 1;

    return 0;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	//Read back command
	unsigned long readBackCommand = TIMER_RB_CMD | TIMER_RB_SEL(timer);

	//Send readBackCommand to the control register
	int valid_out = sys_outb(TIMER_CTRL, readBackCommand);

	//Check if it's valid
	if(!valid_out)
		return 1;

	//Get response from timer
	unsigned long response;
	int valid_in = sys_inb(TIMER_0 + timer, &response);

	//Check if it's valid
	if(!valid_in)
		return 1;

	*st = (unsigned char) response;

	return 0;
}

int timer_display_conf(unsigned char conf) {

	//Properties:

	/*
	 - 0 for value 0
	 - 1 for value 1
	 */
	int timerValue;

	/*
	 - 0 for LSB
	 - 1 for MSB
	 - 2 for LSB followed by MSB
	 */
	int timerAccess;

	/*
	 - 0 for Interrupt on Terminal Count
	 - 1 for Hardware Retriggerable One-shot
	 - 2 for Rate Generator
	 - 3 for Square Wave
	 - 4 for Software Triggered Strobe
	 - 5 for Hardware Triggered Strobe (Retriggerable)
	 */
	int timerOperatingMode;

	/*
	 - 0 for Binary
	 - 1 for BCD
	 */
	int timerCountingMode;


	//Timer Value:
	if((conf & BIT(7)) != 0)
		timerValue = 1;
	else
		timerValue = 0;

	//Timer Access:
	if((conf & BIT(4)) != 0)
	{
		if((conf & BIT(5)) != 0)
			timerAccess = 2;
		else
			timerAccess = 0;
	}
	else
		timerAccess = 1;

	//Timer Operating Mode:
	if((conf & BIT(1)) != 0)
	{
		if((conf & BIT(2)) != 0)
			timerOperatingMode = 3;
		else
		{
			if((conf & BIT(3)) != 0)
				timerOperatingMode = 5;
			else
				timerOperatingMode = 1;
		}
	}
	else
	{
		if((conf & BIT(2)) != 0)
			timerOperatingMode = 2;
		else
		{
			if((conf & BIT(3)) != 0)
				timerOperatingMode = 4;
			else
				timerOperatingMode = 0;
		}
	}

	//Timer Counting Mode:
	if((conf & BIT(0)) != 0)
		timerCountingMode = 1;
	else
		timerCountingMode = 0;

	//Printing value
	printf("Timer value: %d\n", timerValue);

	//Printing operating mode
	printf("Timer operating mode: ");
	switch (timerOperatingMode) {
		case 0:
			printf("Interrupt on Terminal Count\n");
			break;
		case 1:
			printf("Hardware Retriggerable One-shot\n");
			break;
		case 2:
			printf("Rate Generator\n");
			break;
		case 3:
			printf("Square Wave\n");
			break;
		case 4:
			printf("Software Triggered Strobe\n");
			break;
		case 5:
			printf("Hardware Triggered Strobe (Retriggerable)\n");
			break;
	}

	//Printing type of access
	printf("Timer type of access: ");
	switch (timerAccess) {
		case 0:
			printf("LSB\n");
			break;
		case 1:
			printf("MSB\n");
			break;
		case 2:
			printf("LSB followed by MSB\n");
			break;
	}

	//Printing counting mode
	printf("Timer counting mode: ");
	switch (timerCountingMode) {
		case 0:
			printf("Binary\n");
			break;
		case 1:
			printf("BCD\n");
			break;
	}

	return 0;
}
