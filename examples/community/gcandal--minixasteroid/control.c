#include <minix/syslib.h>
#include <minix/drivers.h>
#include "control.h"

int kbd_subscribe_int(void ) {

	sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbdhook);
	sys_irqenable(&kbdhook);

	return 1;
}

int kbd_unsubscribe_int() {

	kbd_readscancode();
	sys_irqrmpolicy(&kbdhook);
	sys_irqdisable(&kbdhook);

	return 1;
}

static unsigned char kbd_readscancode() {

	 unsigned long data, stat;

	 while( 1 ) {
		 sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		 /* loop while 8042 output buffer is empty */
		 if( stat & OBF ) {
			 sys_inb(OUT_BUF, &data); /* assuming it returns OK */

			 if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				 return (unsigned char)data;
			 else return -1;
		 }

		 tickdelay(micros_to_ticks(DELAY_US));
	 }
}

int kbd_int_handler(const unsigned int shots) {

	unsigned char scancode; //Will store C@KBD responses

	scancode = kbd_readscancode();

	if(scancode==-1) {
		printf("Reading Error from Status Register");
		return -1;
	}

	if(scancode==SPACE_BREAK && shots>0)
		return 1;

	if(scancode==ESC_BREAK)
		return 0;

	return -1;
}

int mouse_subscribe_int() {

	unsigned long stat;

	//Enable Mouse
	sys_outb(KBC_CMD_REG,ENABLE_MOUSE);

	//Enable Sending Data Packets
	sys_outb(KBC_CMD_REG,WRITE_BYTE);
	sys_inb(IN_BUF,&stat);
	sys_outb(OUT_BUF,ENABLE_DATA);
	sys_inb(IN_BUF, &stat);

	sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&mousehook);
	sys_irqenable(&mousehook);

	return 1;
}

int mouse_unsubscribe_int() {

	unsigned long stat;

	sys_outb(KBC_CMD_REG,WRITE_BYTE);
	sys_outb(OUT_BUF,DISABLE_DATA);

	sys_inb(IN_BUF,&stat);

	sys_irqrmpolicy(&mousehook);
	sys_irqdisable(&mousehook);

	return 1;
}

int mouse_int_handler(int* x, int* y, int* posx_inicial, int* posy_inicial, int H_RES, int V_RES) {

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
