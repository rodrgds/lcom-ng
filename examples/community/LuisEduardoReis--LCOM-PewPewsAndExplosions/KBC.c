#include <minix/syslib.h>
#include <minix/drivers.h>

#include "KBC.h"
#include "timer.h"
#include <stdio.h>





int kbd_subscribe_int() {

	KBD_HOOK_ID = KBD_INT_BIT;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &KBD_HOOK_ID) != OK
			|| sys_irqenable(&KBD_HOOK_ID) != OK)
		return -1;

	return 1 << KBD_INT_BIT; //Returns bit order in interrupt mask
}

int kbd_unsubscribe_int() {
	if (sys_irqrmpolicy(&KBD_HOOK_ID) == OK)
		return 0;

	return 1;
}



int kbd_int_handler(char* key) {
	unsigned long in;
	unsigned char scancode;

	if (sys_inb(KBD_IOPORT, &in) != OK)
		return 0; //Error recieving stuff

	*key = (unsigned char) in;

	return 0;
}




int kbd_wait_response() {
	unsigned long stat, in;
	int c = 0;
		delay_milis(KBD_DELAY);
		while (c < KBD_MAX_POOLS) {
				sys_inb(KBD_IOPORT, &in);
				switch((unsigned char) in) {
					case ACK: return 0;
					case RESEND: return 1;
					case ERROR: return 2;
					default: return 3;
				}
			c++;
			delay_milis(KBD_DELAY);
		}
	return 4;
}



int kbd_send_command(unsigned long command) {
	unsigned long stat;
	int c = 0;

	while (c < KBD_MAX_POOLS) {
		sys_inb(KBD_STATPORT, &stat);

		if( (stat & KBC_IBF) == 0 ) {
			if (sys_outb(KBD_IOPORT, command) != 0) {
				printf("Error sending command\n");
				return 6;
			}
			switch(kbd_wait_response()) {
				case 0: return 0;
				case 2: return 2;
				case 3: return 3;
				case 4: return 4;
				default: break;
			}
		}
		c++;
		delay_milis(KBD_DELAY);
	}
	return 1;
}


int kbd_send_command_arg(unsigned long command, unsigned long arg) {
	int r;

	if ((r = kbd_send_command(command)) != 0) {
		printf("kbd_send_command error %d\n", r);
		return 1;
	}
	if ((r = kbd_send_command(arg)) != 0) {
		printf("kbd_send_argument error %d\n", r);
		return 2;
	}
};



int kbd_set_leds(unsigned char leds) {
	return kbd_send_command_arg(0xED, leds);
}
