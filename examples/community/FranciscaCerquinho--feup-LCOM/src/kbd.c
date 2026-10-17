#include "kbd.h"

int hook_id_kbd=0;

#define SCROLL 0
#define NUM 1
#define CAPSL 2
unsigned long led_command;
unsigned long msg;

int keyboard_subscribe_int(){
	hook_id_kbd=KBO_IRQ;
	if(sys_irqsetpolicy(KBO_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd)!= 0) //subscribes the interruption
		return 1;
	return BIT(KBO_IRQ);
}

int keyboard_unsubscribe_int(){

	if(sys_irqrmpolicy(&hook_id_kbd)!=0)
		return 1;

	return 0;

}


unsigned long keyboard_long_print(unsigned long state)
{
	unsigned long tmp;
	tmp = ((READ_TEST_INPUTS << 8)| state);

	if(BIT(7) & state)
	{
		printf("MAKECODE %x\n", state);
		return state;
	}
	else {
		printf("BREAKCODE %x\n\n", tmp);
		return  tmp;
	}

}

unsigned long keyboard_int_handler_c(){

	unsigned long st2;

	sys_inb(KBD_OUT_BUF, &st2);
	if(READ_TEST_INPUTS != st2){
		return keyboard_long_print(st2);
	}
	else	return READ_TEST_INPUTS;

	tickdelay(micros_to_ticks(DELAY_US));

}



