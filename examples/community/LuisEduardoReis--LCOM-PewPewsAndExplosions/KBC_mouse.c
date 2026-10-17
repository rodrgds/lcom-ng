#include <minix/syslib.h>
#include <minix/drivers.h>

#include "KBC_mouse.h"
#include <stdio.h>



int mouse_enable_packets() {
	unsigned long in;
	unsigned char command_word;

	sys_outb(KBC_STATPORT, 0xD4);
	sys_outb(KBC_IOPORT, 0xF4);

	sys_inb(KBC_IOPORT, &in); //ACK

	return 0;
}




int mouse_subscribe_int() {

	MOUSE_HOOK_ID = MOUSE_INT_BIT;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &MOUSE_HOOK_ID) != OK
			|| sys_irqenable(&MOUSE_HOOK_ID) != OK)
		return -1;

	return 1 << MOUSE_INT_BIT; //Returns bit order in interrupt mask
}

int mouse_unsubscribe_int() {
	if (sys_irqrmpolicy(&MOUSE_HOOK_ID) == OK)
		return 0;

	return 1;
}


int mouse_int_handler(mousePacket* out) {
	unsigned long in;
	unsigned char packet_byte;

	if (sys_inb(KBC_IOPORT, &in) != OK)
		return 1; //Error recieving stuff
	packet_byte = (unsigned char) in;

	// Syncronize and write
	if (packet_sync == 0) {
		if ((packet_byte & 0x08) != 0) {
			packet_sync = 1;
			packet[0] = packet_byte;
			packet_count = 1;
		}
	} else {
		packet[packet_count] = packet_byte;
		packet_count++;
	}

	// Handle Packet

	if (packet_count >= 3) {
		packet_count = 0;
		if ((packet[0] & 0x08) == 0)
			packet_sync = 0;
		else {
			/*printf("B1: 0x%x B2: 0x%x B3: 0x%x LB: %d MB: %d RB: %d X: %d Y: %d\n",
					packet[0], packet[1], packet[2],
					(packet[0] & 0x01) != 0, (packet[0] & 0x04) != 0, (packet[0] & 0x02) != 0,
					(char) packet[1], (char) packet[2]
			);*/

			out->yoverflow = (packet[0] & 0x80);
			out->xoverflow = (packet[0] & 0x40);
			out->mbutton = (packet[0] & 0x04);
			out->rbutton = (packet[0] & 0x02);
			out->lbutton = (packet[0] & 0x01);
			out->xdelta = (char) (packet[1]);
			out->ydelta = (char) (packet[2]);

			return 0;
		}
	}

	return -1;

}





int mouse_get_config() {
	unsigned long in;
	unsigned char status[3];

	// Stream Disable

		sys_outb(KBC_STATPORT, 0xD4);
		sys_outb(KBC_IOPORT, 0xF5);

		sys_inb(KBC_IOPORT, &in);



	//Request mouse config
		sys_outb(KBC_STATPORT, 0xD4);
		sys_outb(KBC_IOPORT, 0xE9);

		sys_inb(KBC_IOPORT, &in); // ACK



	sys_inb(KBC_IOPORT, &in);
	status[0] = (unsigned char) in;
	sys_inb(KBC_IOPORT, &in);
	status[1] = (unsigned char) in;
	sys_inb(KBC_IOPORT, &in);
	status[2] = (unsigned char) in;


	printf("RM: %d EN: %d SC: %d LB: %d MB: %d RB: %d RES: %d SR: %d\n",
			(status[0] & 0x40) != 0, (status[0] & 0x20) != 0, (status[0] & 0x10) != 0,
			(status[0] & 0x4) != 0, (status[0] & 0x2) != 0, (status[0] & 0x1) != 0,
			status[1] & 0x3, status[2]);

	// Stream Enable

		sys_outb(KBC_STATPORT, 0xD4);
		sys_outb(KBC_IOPORT, 0xF4);

		sys_inb(KBC_IOPORT, &in);

	return 0;
}
