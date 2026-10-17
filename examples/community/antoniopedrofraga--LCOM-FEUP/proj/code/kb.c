#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "kb.h"
#include "i8254.h"
#include "timer.h"
int hookidkb = 8;
unsigned char CAPSON = 0;
unsigned char NUMLOCKON = 0;
unsigned char LOCKON =0;

//KB FUNCTIONS
int kb_subscribe_int() {
	int temp = hookidkb; //hookid temporario para não alterar o valor original
	if(sys_irqsetpolicy(1,IRQ_REENABLE|IRQ_EXCLUSIVE,&temp)!=OK){
		return -1;
	}
	if(sys_irqenable(&temp)!=OK){
		return -1;
	}
	return BIT(hookidkb); //return dos bits da interrupt mask
}

int kb_unsubscribe_int() {
	int hookidkbtemp = hookidkb;
	if(sys_irqrmpolicy(&hookidkbtemp)!=OK){
		return 1;
	}
	if(sys_irqenable(&hookidkbtemp)!=OK){
		return 1;
	}
	return 0;
}

unsigned long get_kb(){
	unsigned long data;
	unsigned long stat;
	int a =0;
	while( a<4 ) {//tenta no minimo 4 vezes
		sys_inb(STAT_REG, &stat);
		if( stat & OBF ) {
			if(sys_inb(OUT_BUF, &data)!=OK){
				return -1;
			}
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		a++;
	}
}
