#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"
#include "utilities.h"

int counter = 0;
int hook_id3 = 5;

//123
int getCounter() {return counter;}

void resetCounter()
{
	counter = 0;
}


int timer_set_square(unsigned long timer, unsigned long freq)
{
	unsigned long div = TIMER_FREQ/freq;
	unsigned char conf;
		if(freq == 0 || freq > TIMER_FREQ || div >= (1 << 16))
		{
			return 1;
		}

		if(timer >= 0 && timer <= 2)
		{

		if(timer_get_conf(timer,&conf)!= 0)
		{
			return 1;
		}

		if(sys_outb(TIMER_CTRL,TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE | (conf & BIT(0))) != 0)
		{
			return 1;
		}

		if(sys_outb(TIMER_0+timer, (char)div) != 0)
		{
			return 1;
		}

		if(sys_outb(TIMER_0+timer,(char)(div>>8)) != 0)
		{
			return 1;
		}

		return 0;
		}

		return 1;
}



int timer_subscribe_int(void )
{
	int hook;
	hook = hook_id3;
	if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id3) == OK)
		if (sys_irqenable(&hook_id3) == OK)
			return BIT(hook);

	printf("erro no timer subscribe!!\n");

	return -1;
}

int timer_unsubscribe_int()
{
	if (sys_irqdisable(&hook_id3) == OK)
		if(sys_irqrmpolicy(&hook_id3) == OK)
			return 0;

	return 1;
}

void timer_int_handler()
{
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st)
{
	unsigned long temporario;
	int x;

	temporario = TIMER_RB_CMD | TIMER_RB_COUNT_  |TIMER_RB_SEL(timer) ;

	if(sys_outb(TIMER_CTRL, temporario) != 0)
	{
		return 1;
	}

	unsigned long status;
	if(sys_inb(TIMER_0+timer, &status)!= 0)
	{
		return 1;
	}
	*st = (unsigned char) status;
	//printf("%X",*st);
	//printf("\n");
	return 0;
}

int timer_display_conf(unsigned char conf)
{
	if(BIT(7) == (BIT(7) & conf))
	{
		printf("Output: 1 \n");
	}
	else
	{
		printf("Output: 0\n");
	}



	if((BIT(5)|BIT(4)) == ((BIT(5)|BIT(4))& conf))
	{
		printf("Type of access: LSB followed by MSB \n ");
	}
	else if(BIT(5) == (BIT(5) & conf))
	{
		printf("Type of access: MSB \n ");
	}
	else
		printf("Type of access: LSB\n ");
	
	if(((BIT(3)|BIT(2)|BIT(1)) == ((BIT(3)|BIT(2)|BIT(1)) & conf))|| ((BIT(2)|BIT(1)) == ((BIT(2)|BIT(1)) & conf)))
	{
		printf("Programmed mode: 3 \n");
	}
	else if((BIT(3)|BIT(2))== ((BIT(3)|BIT(2)) & conf))
	{
		printf("Programmed mode: 2 \n");
	}
	else if((BIT(3)| BIT(1))== ((BIT(3)| BIT(1)) & conf))
	{
		printf("Programmed mode: 5 \n");
	}
	else if(BIT(3)== (BIT(3) & conf))
	{
		printf("Programmed mode: 4 \n");
	}
	else if(BIT(2) == (BIT(2) & conf))
	{
		printf("Programmed mode: 2 \n");
	}
	else if(BIT(1) == (BIT(1) & conf))
	{
		printf("Programmed mode: 1 \n");
	}
	else
		printf(" Programmed mode: 0 \n");


//mudar o BIT(2) | BIT(1) por BIT_SQ
	if(BIT(0) == (BIT(0) & conf))
		printf("Counting mode: BCD (4  decades) \n");
	else
		printf("Counting mode: Binary (16 bits) \n");
	return 0;
}


int timer_test_square(unsigned long freq)
{
	return timer_set_square(0,freq);
}


int timer_test_int(unsigned long time)
{
	int irq_set = timer_subscribe_int(); //subscreve e inicia as interrupções do timer0
	timer_set_square(0,60); //coloca a frequencia a 60

	int ipc_status;
	int r;
	message msg;
	unsigned i = 0;
	while( i < time) { // enquando a contagem é menor que o valor passado no parametro
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}



		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			 case HARDWARE: /* hardware interrupt notification */
				 if (msg.NOTIFY_ARG & irq_set)
				 { /* subscribed interrupt */
					 timer_int_handler();
					 if (counter % 60 == 0) //a cada segundo (60 contagens a 60 de frequencia)
					 {
						 i++;
						 //printf("%d", i); // imprime o valor do segundo.
						// printf("\n");
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
	if (timer_unsubscribe_int() != OK)  //termina a subscrição, caso dê erro retorna 1
		return 1;
	
	return 0;
}

int timer_test_config(unsigned long timer)
{
	unsigned char tempo;
	int erro;
	if(timer<0 || timer >2)
	{
		return 1;
	}
	erro = timer_get_conf(timer,&tempo);

	if(erro == 1)
		return 1;


	erro = timer_display_conf(tempo);

	if(erro == 1)
		return 1;

	return 0;
}
