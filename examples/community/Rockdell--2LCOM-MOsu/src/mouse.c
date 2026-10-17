#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse.h"
#include "i8042.h"

//Mouse's hook id
int mouse_hook_id = MC_IRQ;

//Mouse packets
unsigned char packet[3];

//Counter of bytes
int counter_m = 0;

//Booleans for processing packet (we'll use them in gesture)
int downRB = 0;
int downLB = 0; //For Mosu meno
int packetX;
int packetY;

int mouse_subscribe_int() {

	int mouse_id = mouse_hook_id;
	int valid = sys_irqsetpolicy(MC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);

	if(valid != OK)
		return -1;
	else
		return mouse_id;
}

int mouse_unsubscribe_int() {

	int valid = sys_irqrmpolicy(&mouse_hook_id);

	if(valid != OK)
		return 1;
	else
		return 0;
}

int mouse_int_handler() {

	//No need to check for STAT_REG

	//Get byte of packet from KBC's buffer
	unsigned long response;
	if(M_getResponse(&response))
		return 1;

	addBytePacket(response);

	return 0;
}

int addBytePacket(unsigned long byte) {

	//First byte of packet
	if(counter_m == 0) {
		//If it's the first byte and and bit 3 is 0, then code is not in sync with mouse
		if( (byte & BIT(3)) == 0 )
			return 1;
	}

	//Code is in sync with mouse, so we save it
	packet[counter_m] = byte;

	if(counter_m + 1 == 3)
		counter_m = 0;
	else
		counter_m++;

	return 0;
}

int getCounter() {
	return counter_m;
}

int getRB() {
	return downRB;
}

int getLB() {
	return downLB;
}

int getX() {
	return packetX;
}

int getY() {
	return packetY;
}

void printPacket() {
	unsigned char packet_b1 = packet[0], packet_b2 = packet[1], packet_b3 = packet[2];

	printf("B1=0x%X B2=0x%X B3=0x%X ", packet_b1, packet_b2, packet_b3);

	printf("LB=");
	if(packet_b1 & BIT(0))
		printf("1 ");
	else
		printf("0 ");

	printf("RB=");
	if(packet_b1 & BIT(1))
		printf("1 ");
	else
		printf("0 ");

	printf("MB=");
	if(packet_b1 & BIT(2))
		printf("1 ");
	else
		printf("0 ");

	int xSign = packet_b1 & BIT(4);
	int ySign = packet_b1 & BIT(5);

	printf("XOV=");
	if(packet_b1 & BIT(6))
		printf("1 ");
	else
		printf("0 ");

	printf("YOV=");
	if(packet_b1 & BIT(7))
		printf("1 ");
	else
		printf("0 ");

	int x, y;

	if(xSign){
		packet_b2 = ~packet_b2;
		x = -(packet_b2 + 1);
	}
	else {
		x = packet_b2;
	}

	if(ySign){
		packet_b3 = ~packet_b3;
		y = -(packet_b3 + 1);
	}
	else {
		y = packet_b3;
	}

	printf("X=%d Y=%d\n", x, y);

	//Is RB down?
	if(packet_b1 & BIT(1))
		downRB = 1;
	else
		downRB = 0;

	//Only updates if RB is down
	if(downRB) {
		packetX = x;
		packetY = y;
	}

}

void updateCoord() {

	unsigned char packet_b1 = packet[0], packet_b2 = packet[1], packet_b3 = packet[2];

	int xSign = packet_b1 & BIT(4);
	int ySign = packet_b1 & BIT(5);

	//Update LB for Mosu menu
	downLB = packet_b1 & BIT(0);

	int xOV = packet_b1 & BIT(6);
	int yOV = packet_b1 & BIT(7);

	if(xOV == 1 || yOV == 1)
		return;

	int x, y;

	if(xSign){
		packet_b2 = ~packet_b2;
		x = -(packet_b2 + 1);
	}
	else {
		x = packet_b2;
	}

	if(ySign){
		packet_b3 = ~packet_b3;
		y = -(packet_b3 + 1);
	}
	else {
		y = packet_b3;
	}

	packetX = x;
	packetY = y;
}

