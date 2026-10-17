#include "libraries.h"

//interuption variavels
//timer:
//static int hook_id_timer = 0;
static int arg_timer = 1;
//Keyboard:
//static int hook_id_kbd = 1;
static int arg_kbd = 2;
//Serial port COM2:
//static int hook_id_COM2 = 3;
static int arg_COM2 = 8;
//Serial port COM1
//static int hook_id_COM1 = 4;
static int arg_COM1 = 16;
//RTC:
//static int hook_id_rtc = 8;
static int arg_rtc = 256;
//MOUSE:
//static int hook_id_mouse = 12;
static int arg_mouse = 4096;

int counter =0;
unsigned char packet[3];

///susbscribes the devices used in game
void subscribe_interrupts()
{
	keyboard_subscribe_int();
	timer_subscribe_int();
	RTC_subscribe();
}

void unsubscribe_interrupts()
{
	clean_OBF();
	//unsubscribe the devices
	keyboard_unsubscribe_int();
	timer_unsubscribe_int();
	RTC_unsubscribe();
	//mouse_unsubscribe_int();
}

//Interrupt receptor
void interupt_handler(int time)
{
	int ipc_status;
	message msg;
	//int irq_set=timer_subscribe_int();
	int error;
	//ligar RTC
	unsigned long regB;
	counter = 0;
	regB = read_rtc(RTC_REG_B);
	regB = regB ^ RTC_UIE;
	write_rtc(RTC_REG_B,regB);

	while(counter<(time*60) && check_number_of_tries()==0) {
		/* Get a request message. */
		error = driver_receive(ANY, &msg, &ipc_status);
		if (error != 0 ) {
			printf("driver_receive failed with: %d",error);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				/*if ((msg.NOTIFY_ARG & BIT(0)) && (counter%60==0)) //subscribed interruptTimer
				{
					//timer_int_handler(counter); //same function as in lab3
					RTC_handler_date();
					counter++;
				}
				else*/
				if(msg.NOTIFY_ARG & BIT(1))
				{
					keyboard_handler(); //different function from the original
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

	return;

}



