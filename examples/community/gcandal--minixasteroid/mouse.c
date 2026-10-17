#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse.h"

int mouse_subscribe_int() {

	sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&mousehook);
	sys_irqenable(&mousehook);

	return 1;
}

int mouse_unsubscribe_int() {

	unsigned long stat;

	sys_inb(IN_BUF,&stat);
	sys_irqrmpolicy(&mousehook);
	sys_irqdisable(&mousehook);

	return 1;
}

int mouse_int_handler(int* x, int* y, int* posx_inicial, int* posy_inicial, const int H_RES, const int V_RES) {

	sys_inb(IN_BUF,&stat);

	int confirma=0, changed=0;

	if( !(stat&BIT(3)) && (count==0))
		confirma=1;

	packet[count]=stat;
	count++;

	if(count==3 && confirma==0) {
		count=0;

		if(packet[1]!=0 || packet[2]!=0) {
			changed=1;

			if(packet[1]!=0) {
				if(packet[1]<128)
					*posx_inicial+=packet[1];
				else *posx_inicial+=packet[1]-256;

				if(*posx_inicial<0)
					*posx_inicial=0;
				else if(*posx_inicial>(H_RES-40))
					*posx_inicial=H_RES-40;

				*x=*posx_inicial;
			}

			if(packet[2]!=0) {
				if(packet[2]<128)
					*posy_inicial-=packet[2];
				else *posy_inicial-=packet[2]-256;

				if(*posy_inicial<0)
					*posy_inicial=0;
				else if(*posy_inicial>(V_RES-100))
					*posy_inicial=V_RES-100;

				*y=*posy_inicial;
			}
		}
	}

	return changed;
}
