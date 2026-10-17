#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
//#include "Utilities.h"

//VARS============================
unsigned long counter;
unsigned int irq_set=0;
int irqTimerBIT=0;//timer's bit on irq_set
int ddTimer0Id;
int tick = 0;
//END VARS========================

int timer_set_square(unsigned long timer, unsigned long freq) {

	//Get timer address
	unsigned long timerAdress;
	if(timer ==0) timerAdress=TIMER_SEL0;
	else if(timer ==1 ) timerAdress=TIMER_SEL1;
	else if (timer ==2 ) timerAdress=TIMER_SEL2;
	else return 1;

	//Verify COUNTING MODE
	char getConfig;
	timer_get_conf(timer,&getConfig);
	if(getConfig & TIMER_BCD)
		{
		printf("Counting Mode BCD can't set freq\n");
		return 1;
		}


	//Write control word to the control register
	unsigned long setConfig = (timerAdress|TIMER_LSB_MSB|TIMER_SQR_WAVE);
	if(sys_outb(TIMER_CTRL,setConfig)!=OK)
	{
		printf("Failed to write control word");
		return 1;
	}

	// Wave Frequency: clock/div
	char LSBdiv= (char)(TIMER_FREQ/freq);
	char MSBdiv= (char)((TIMER_FREQ/freq)>>8);

	//Write LSB
	if(sys_outb(TIMER_0+timer,LSBdiv)!=OK)
			{
				printf("Failed to write LSB");
				return 1;
			}
	//Write MSB
	if(sys_outb(TIMER_0+timer,MSBdiv)!=OK)
			{
				printf("Failed to write MSB");
				return 1;
			}

	return 0;
}

int timer_subscribe_int(void ) {
	int hookid=irqTimerBIT;//TIMER0_IRQ;
	irq_set=1<<irqTimerBIT;
	printf("%d\n",irq_set);
	//Subscribe a notification on every interrupt in the input irq_line.
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid)!=OK)
	{
			printf("Subscription FAILED!\n");
			return -1;
		}

	ddTimer0Id=hookid;

	return TIMER0_IRQ;
}

int timer_unsubscribe_int() {
	//Disables interrupts on the IRQ line associated with the specified hook_id
	int hookid=ddTimer0Id;

	if(sys_irqdisable(&hookid)!=OK)
	{
		printf("Unsubscription FAILED!\n");
		return -1;
	}
	return 0;
}

void timer_int_handler(unsigned int bitmask){
	if (bitmask & (1<<irqTimerBIT))
	{
		counter--;//Update counter first so there is no print on first run
		tick = 1;
		//DEBUG_STR("Tick");
	}
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if(timer !=0 && timer !=1 && timer !=2 )
		return 1;

	//Write Read-Back command to the control register
	unsigned long config;
	config = (TIMER_RB_CMD | TIMER_RB_SEL(timer)|TIMER_RB_COUNT_);
	if(sys_outb(TIMER_CTRL,config)!=OK)
	{
		printf("Failed to write Read-Back command");
		return 1;
	}

	//Get configuration from RBC
	unsigned long status;
	if(sys_inb(TIMER_0+timer,&status)!=OK)
	{
		printf("Failed to get configuration from RBC");
		return 1;
	}

	*st=(unsigned char)status;

	return 0;

}

int timer_display_conf(unsigned char conf) {

	//Print bits
	unsigned char num=conf;
	printf("Timer Configuration Bits: ");
	int i=0;
	char lastBit=128;
	for(;i<8;i++)
	{
		printf("%d" , (num & lastBit) ? 1 : 0);
		num = num << 1;
	}
	printf("\n");

	//COUNTING MODE
	if(conf & TIMER_BCD)
		printf("Counting Mode: BCD\n");
	else
		printf("Counting Mode: Binary\n");

	//OPERATION MODE
	if((conf & TIMER_SQR_WAVE)==TIMER_SQR_WAVE)
		printf("Operating Mode: Square wave generator \n");
	else if((conf & TIMER_RATE_GEN)==TIMER_RATE_GEN)
		printf("Operating Mode: Rate generator \n");
	else
		printf("Operation Mode Unknown \n");


	//REGISTER SELECTION
	if ((conf & TIMER_LSB_MSB)==TIMER_LSB_MSB)
		printf("Register Selection: LSB followed by MSB \n");
	else if(conf & TIMER_LSB)
		printf("Register Selection: LSB \n");
	else if(conf & TIMER_MSB)
		printf("Register Selection: MSB \n");
	else
		printf("Register Selection: Unknown \n");

	return 0;
}

int timer_test_square(unsigned long freq) {
	if (timer_set_square(0, freq)!=OK) return 1;

		return 0;
}

int timer_test_int(unsigned long time) {

	if(time<1) return 1;

	//Set counter (prints "time" times
	counter=time*60;//presumes 60Hz


	if(timer_subscribe_int()<0)//subscribe timer0
	{
		printf("timer_subscribe_int Failed!\n");
		return 1;
	}

	//loop until time seconds have passed
	while( counter>0 ) {

		int ipc_status;
			message msg;

				/* Get a request message. */
				int r;
				if ( (r = driver_receive(ANY, &msg, &ipc_status))  != 0 )
				{
					printf("driver_receive failed with: %d", r);
					return 1;
				}
				if (is_ipc_notify(ipc_status)) {
					/* received notification */
					switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE:	/* hardware interrupt notification */
						if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

							/* process timer interruption*/
							timer_int_handler(msg.NOTIFY_ARG);
							//printf("DEBUG counter %d\n",counter);

						}
					/*no other notifications expected: do nothing */
					}
				} else {
					/* received a standard message, not a notification */
					/* no standard messages expected: do nothing */
				}

	//Print stuff once per second
	//note:could also use an extra variable to avoid the mod operation
	if (counter%60==0) printf("One second has passed\n");

	}

	if(timer_unsubscribe_int()!=OK)//unsubscribe timer0
	{
		printf("timer_unsubscribe_int Failed!\n");
		return 1;
	}

	return 0;
}

int timer_test_config(unsigned long timer) {

	unsigned char conf;
	if (timer_get_conf(timer,&conf)!=OK)
	{
		printf("Could not get timer configuration\n");
		return 1;
	}
	if(timer_display_conf(conf)!=OK)
	{
		printf("Could not display timer configuration\n");
		return 1;
	}

	return 0;

}

void timer_setCounter(unsigned long seconds)
{counter=seconds*60;}
unsigned long timer_getCounter() {return counter;}

int getTick(){return tick;}

void resetTick(){tick = 0;}
