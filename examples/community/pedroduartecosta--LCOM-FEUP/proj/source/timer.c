#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"




int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned long lsb;
	unsigned char msb;
	unsigned long fr;
	unsigned char st;

	//verifica se o resultado da frequencia é menor que o valor maximo possivel de representar em 2 bytes.
	if(TIMER_FREQ/freq >= 0xFFFF)
	{
		printf("Frequencia nao suportavel pelo timer (overflow).\n");
		return 1;
	}

	fr = TIMER_FREQ / freq;
	if (timer == 0 || timer == 1 || timer == 2)
	{
		timer_get_conf(timer, &st);

		st=st<<4;
		st=st>>4;
		st=(TIMER_0 + timer | TIMER_LSB_MSB | st); //controlador criado mantendo o counting mode e operation
		if (sys_outb(TIMER_CTRL, st)) {
			printf("Error by calling sys_outb for confs\n");
			return 1;
		}

		lsb = (char)fr; //isola o primeiro byte da frequencia
		fr = fr>>8;
		msb = (char)fr; //isola o segundo byte da frequencia

		if (sys_outb(TIMER_0 + timer, lsb))	//envia o LSB para o timer
				{
			printf("Error by calling sys_outb for lsb.\n");
			return 1;
				}
		else if (sys_outb(TIMER_0 + timer, msb)) //envia o MSB para o timer
				{
			printf("Error by calling sys_outb for msb.\n");
			return 1;
				}
		else {
			return 0;
		}
	} else
	{
		printf("Not a possible timer.\n");
		return 1;
	}
}

int timer_subscribe_int(void) {

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer))==OK)
	{
		sys_irqenable(&hook_timer);//ativa as interrupcoes
		return BIT(NOTIFICATION_TIMER);
	}
	else return -1;

}

int timer_unsubscribe_int() {

	if(sys_irqdisable(&hook_timer)==OK) //desativar as interrupcoes
	{
		sys_irqrmpolicy(&hook_timer); //Remove a policy do qual foi feito o disable
		return 0;
	}
	else return 1;
}

void timer_int_handler() {
	mouse_counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if(timer==0 || timer==1 || timer==2) //Verificar se o timer é 0,1 ou 2
	{
	char CMD = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	if (sys_outb(TIMER_CTRL, CMD) == 0) {
		unsigned long stt=(long) st;
		if (sys_inb(TIMER_0 + timer, &stt) == 0) {
			return 0;
		} else
			return 1;
	} else
		return 1;
	}
	else //Caso seja introduzido um timer incorreto
	{
		printf("Wrong timer!\n");
		return 1;
	}
}

int timer_display_conf(unsigned char conf) {
	int CM = conf & BIT(0); //Verificar o bit 0(counting mode)
	if (CM == 0)
	{
		printf("BCD\n");
	} else if (CM == 1) {
		printf("Binary\n");
	}
	int OM = conf >> 1 & (BIT(0) | BIT(1) | BIT(2)); //Verificar o bit 1,2,3(operation mode)
	if (OM == 0){
		printf("Interrupt on Terminal Count\n");
	} else if (OM == 1){
		printf("Hardware Retriggerable One-shot\n");
	} else if (OM == 2) {
		printf("Rate Generator\n");
	} else if (OM == 6) {
		printf("Rate Generator\n");
	} else if (OM == 3) {
		printf("Square Wave Mode\n");
	} else if (OM ==7){
		printf("Square Wave Mode\n");
	} else if (OM == 4) {
		printf("Software Triggered Strobe\n");
	} else if (OM == 5) {
		printf("Hardware Triggered Strobe(Retriggerable)\n");
	}
	int TA = conf >> 4 & (BIT(0) | BIT(1)); //Verificar o bit 4,5(Type of Access)
	if (TA == 1) {
		printf("LSB");
	} else if (TA == 2) {
		printf("MSB\n");
	} else if (TA == 3) {
		printf("LSB followed by MSB\n");
	}
	int SC = conf >> 6 & BIT(0);//Verificar bit 6(Null Count)
	if (SC == 0) {
		printf("Null Count=True\n");
	} else if (SC == 1) {
		printf("Null Count=False\n");
	}
	int SC1 = conf >> 7 & BIT(0);//Verificar bit 7(Output)
	if (SC1 == 0) {
		printf("Output=0\n");
	} else if (SC == 1) {
		printf("Output=1\n");
	}
	return 0;
}

int timer_test_int(unsigned long time)
{
	int ipc_status;
	message msg;
	int r,irq_set=BIT(hook_timer);
	int x=0; //contar as interrupcoes feitas
	if(timer_test_square(60)==1) //verificar e colocar freq a 60 por segundo
	{
		return 1;
	}
	if(timer_subscribe_int()==1)
	{
		return 1;
	}

	while(time>x) //Condicao para o ciclo acabar quando for feito 60*time interrupcoes
	{
		if((r=driver_receive(ANY,&msg,&ipc_status))!=0){
			printf("driver_receive failed with: %d",r);
			continue;
		}
		if(is_ipc_notify(ipc_status)){
			 switch (_ENDPOINT_P(msg.m_source)) {
			 case HARDWARE:
				 if (msg.NOTIFY_ARG & irq_set) {
					 timer_int_handler();
					 if(mouse_counter==60) //sempre que counter for igual a 60 (passou 1 segundo)
					 {
						 printf("Interrupcao!\n");
						 mouse_counter=0;
							x++;
					 }
				 }
				 break;
			 default:
				 break;
			 }
		}
	}
	if(timer_unsubscribe_int()==1)
	{
		return 1;
	}
	return 0;
}


int timer_test_square(unsigned long freq) {
	if(timer_set_square(0,freq)==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int timer_test_config(unsigned long timer) {

	unsigned char conf;
	if(timer_get_conf(timer, &conf)==0)
	{
		timer_display_conf(conf);
		return 0;
	}
	else
	{
		return 1;
	}
}

