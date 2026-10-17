#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "stdio.h"
#include "i8254.h"


unsigned int timer_counter;

static int timer_hook_id;



int timer_set_square(unsigned long timer, unsigned long freq)
{
	char LSB, MSB, RegControl, port;
	unsigned long division;

	division = TIMER_FREQ/freq;


	if(timer > 2 || freq == 0)
		return 1;

	LSB = (char) division;
	MSB = (char) (division >> 8);	// 16/2


	switch(timer)
	{
	case 0:
		RegControl = TIMER_SEL0;
		port = TIMER_0;
		break;

	case 1:
		RegControl = TIMER_SEL1;
		port = TIMER_1;
		break;

	case 2:
		RegControl = TIMER_SEL2;
		port = TIMER_2;
		break;
	default: return 1;
	}

	RegControl |= TIMER_BIN;	// BCD mode
	RegControl |= TIMER_SQR_WAVE; // wave mode
	RegControl |= TIMER_LSB_MSB;

	if( sys_outb( TIMER_CTRL, RegControl) != OK ||
		sys_outb(  port, LSB) != OK	 ||
		sys_outb(  port, MSB) != OK )
		return 1;

	return 0;
}


int timer_get_config(unsigned long timer, unsigned char *st) {

	int port ;
	char ControlReg = 0;
	unsigned long tmp ;



	if(timer  > 2)
		return 1;

	switch(timer)
	{
	case 0:
		ControlReg =  TIMER_RB_SEL(0);
		port = TIMER_0;
		break;
	case 1:
		ControlReg = TIMER_RB_SEL(1);
		port = TIMER_1;
		break;

	case 2:
		ControlReg = TIMER_RB_SEL(2);
		port = TIMER_2;
		break;

	}


	ControlReg |=  TIMER_RB_COUNT_;
	ControlReg |=  TIMER_BCD;
	ControlReg |=  TIMER_RB_CMD;


	if (sys_outb(TIMER_CTRL, ControlReg) != OK)
		return 1;
	if( sys_inb(port,  &tmp) != OK )
		return 1;

	memcpy(st, &tmp, 1);
	return 0;

}
int bitRead(unsigned char number, int bit)
{
	number >> bit;
	number &= 0x01;
	return (int ) number;
}


int timer_show_config(unsigned long timer) {

	unsigned char regcontrol;
	char tmp;


	if(timer_get_config(timer, &regcontrol) == 0)
	{

		printf("%x\n", regcontrol);

		printf("BCD : %d\n", bitRead(regcontrol, 0));

		tmp = 0x0E;
		tmp &= regcontrol;
		printf("Programmed Mode: %d%d%d\n", bitRead(regcontrol, 3),bitRead(regcontrol, 2),bitRead(regcontrol, 1));

		tmp = 0x30;
		tmp&= regcontrol;
		printf("Type of Access : %d\n", tmp);

		tmp = 0x40;
		tmp &= regcontrol;
		printf("Null Count : %d\n", tmp);

		tmp = 0x80;
		tmp &= regcontrol;
		printf("Output : %d\n", tmp);
		return 0;

	}
	else
		return 1;
}


int time_interrupt_handler(){
	timer_counter = 0;
	unsigned int i = 0, r;
	int ipc_status;
	message msg;
	short irq_set;

	timer_subscribe_int1();

	irq_set = timer_hook_id;

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
				if(msg.NOTIFY_ARG && irq_set)
				{
					if(i%60 == 0 && i >0){
						timer1_unsubscribe_int();
						return 1;
					}
					i++;
				}
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
	timer1_unsubscribe_int();

	return 0;
}
