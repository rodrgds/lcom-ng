#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "timer.h"

unsigned int hookidmouse = 2;

int mouse_subscribe_int() {
	int temp = hookidmouse; //hookid temporario para não alterar o valor original
	if(sys_irqsetpolicy(12 ,IRQ_REENABLE | IRQ_EXCLUSIVE, &hookidmouse)!=OK){
		return 1;
	}
	if(sys_irqenable(&hookidmouse)!=OK){
		return 1;
	}
	return BIT(temp); //return dos bits da interrupt mask
}

int mouse_unsubscribe_int() {
	int temp = hookidmouse;
	if(sys_irqdisable(&temp)!=OK){
				return 2;
		}
	if(sys_irqrmpolicy(&temp)!=OK){
			return 1;
		}
	return 0;
}

unsigned short get_packets(){
	unsigned long data;
	unsigned long stat;
	int a =0;
	while( a<4 ) {//tenta no minimo 4 vezes
		sys_inb(STAT_REG, &stat);
		if( stat & OBF ) {
			if(sys_inb(OUT_BUF, &data)!=OK){
				return 1;
			}
			return data;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		a++;
	}
	return -1;
}

int activate_mouse(){
	if(sys_outb(STAT_REG, 0xA8)!=OK) return 1;
	if(sys_outb(STAT_REG, 0xD4)!=OK) return 1;
	if(sys_outb(IN_BUF, 0xF4)!=OK) return 1;
	if(get_packets()!=ACK) return 1;
	return 0;
}

int deactivate_mouse(){
	if(sys_outb(STAT_REG, 0xD4)!=OK) return 1;
	if(sys_outb(IN_BUF,0xF5)!=OK) return 1;
	if(get_packets()!=ACK) return 1;
	return 0;
}
