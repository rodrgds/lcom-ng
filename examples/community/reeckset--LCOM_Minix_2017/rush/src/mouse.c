#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include "mouse.h"
#include "i8042.h"

static int g_hookID_MOUSE = 5;


int mouse_subscribe_int() {


		int initialID = g_hookID_MOUSE;

		if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hookID_MOUSE) != OK ) return -1;

		sys_irqenable(&g_hookID_MOUSE);


		return initialID;
}

int mouse_unsubscribe_int() {

	sys_irqrmpolicy(&g_hookID_MOUSE);


	return 0;
}
int mouse_send_cmd(unsigned long cmd) {

	if(check_buffers() != 0) return 2;

	return mouse_send_cmd_raw(cmd);
}

int check_buffers(){
	unsigned long statusReg;

		sys_inb(MOUSE_IN_BUF, &statusReg);

		if((statusReg & BIT (0)) == BIT(0)) {
			//Output buffer is full, must read data
			unsigned long bufferData;
			sys_inb(MOUSE_OUT_BUF, &bufferData);
		}

		if((statusReg & BIT(1)) == BIT(1)) {
			//Input buffer is full, cannot write cmds
			return 1;
		}
		return 0;
}

int mouse_send_cmd_raw(unsigned long cmd){

	int stopFlag = 0;

	while(!stopFlag) {

			//write command to mouse
			sys_outb(KBD_IN_BUF, 0xD4);

			//send command
			sys_outb(KBD_OUT_BUF, cmd);

			tickdelay(micros_to_ticks(20000));

			//check if the command was OK
			unsigned long response;
			sys_inb(MOUSE_OUT_BUF, &response);

			//if ACK, the command was received well. return 0
			//if NACK, there was an error, retry command
			//if ERROR, second consecutive invalid byte. return 1
			if(response == ACK) return 0;
			else if(response == ERROR) stopFlag = 1;
		}
	return 1;
}

int mouse_enable_stream_mode() {

	//enable stream mode
	if(mouse_send_cmd(0xEA) == 0) {
		//enable data reporting
		return mouse_send_cmd(0xF4);
	}
	return 1;
}


int mouse_movement_handler(unsigned long bufferData, unsigned short *nPacketsReceived, unsigned char *returnPacket) {
//BUILD PACKET

	//initialize the array wich will keep the 3 bytes (char) of the received packet
	static unsigned char packet[3];
	static int bytePos = 0;

	if(bytePos == 0 && ((bufferData & BIT(3)) != BIT(3))){
		return -1; //if the bytePos is 0 and 4th bit is not 1, must not create packet
	}
	packet[bytePos] = bufferData;
	bytePos++;

	if(bytePos > 2){
		//[DEBUG]print_packet(packet);
		(*nPacketsReceived)++;
		bytePos = 0;
		memcpy(returnPacket, packet, 3);
		return 1;
	}
	return 0;

}

int print_packet(unsigned char* packet) {
	unsigned char byte1 = packet[0], byte2 = packet[1], byte3 = packet[2];

	unsigned char lb, mb, rb, xov, yov, deltaX, deltaY;

	lb = byte1 & BIT(0);
	rb = (byte1 & BIT(1)) >> 1;
	mb = (byte1 & BIT(2)) >> 2;

	xov = (byte1 & BIT(6)) >> 6;
	yov = (byte1 & BIT(7)) >> 7;

	deltaX = byte2;
	deltaY = byte3;

	printf("B1= 0x%x B2= 0x%x B3= 0x%x LB= %d MB= %d RB= %d XOV= %d YOV= %d X= %d Y= %d\n", byte1, byte2, byte3, lb, mb, rb, xov, yov, deltaX, deltaY);
	return 0;
}
/*
int mouse_test_gesture(short length){
	//enable mouse in stream mode
	if(mouse_enable_stream_mode() != 0) return 1;

	//subscribe mouse's interrupts, if some error occurs, return 1
	if(mouse_subscribe_int() == -1) return 1;

	int ipc_status, r;
	message msg;

	unsigned short nPacketsReceived = 0;

	//// GESTURE DETECTION VARIABLES
	short dxTotal = 0;
	short dyTotal = 0;
	int isMouseBtnDown = 0;
	int isPositiveSlope = 1;

	int stopFlag = 0;
	//look for interrupts until the number of packets received is == cnt
	while(!stopFlag) {
		// Get a request message.
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;//continue while loop
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification
					if (msg.NOTIFY_ARG & BIT(g_initialHookID_MOUSE)) { // subscribed interrupt
						unsigned long bufferData;

						//read the byte form out_buf
						sys_inb(MOUSE_OUT_BUF, &bufferData);
						unsigned char packet[3];
						if(mouse_movement_handler(bufferData, &nPacketsReceived, &(packet[0])) == 1){ //JUST FINISHED RECEIVING PACKET
							//PACKET FINISHED. PROCESS
							short X,Y;

							if((BIT(4) & packet[0]) == BIT(4)) //X is Negative
								X = 0xFF00;
							else X = 0x0000;

							if((BIT(5) & packet[0]) == BIT(5)) //Y is Negative
								Y = 0xFF00;
							else Y = 0x0000;

							X |= packet[1];
							Y |= packet[2];


							if(X * Y < 0 || (BIT(1) & packet[0]) != BIT(1)){//If slope is not positive or RMB is not pressed,
								//RESET VALUES
								isPositiveSlope = 1;
								isMouseBtnDown = 0;
								dxTotal = 0;
								dyTotal = 0;
							}
							if((BIT(1) & packet[0]) == BIT(1) && X * Y >= 0){ //RMB is down
								isMouseBtnDown = 1;
								dxTotal += X;
								dyTotal += Y;
							}
							if(isMouseBtnDown && isPositiveSlope){ //If RMB was pressed before and the slope is positive
								if(dxTotal*dxTotal + dyTotal*dyTotal >= length * length)
									stopFlag = 1;
							}
						}
					}
					break;
				default:
					break; // no other notifications expected: do nothing
			}
		} else { // received a standard message, not a notification
			// no standard messages expected: do nothing
		}
	}
	//unsubscribe mouse's interrupts
	mouse_unsubscribe_int();

	return 0;

}
*/
void mouseHandler(ev_queue_st* events, unsigned char* packet){
	short dX,dY;

	if((BIT(4) & packet[0]) == BIT(4)) //X is Negative
		dX = 0xFF00;
	else dX = 0x0000;

	if((BIT(5) & packet[0]) == BIT(5)) //Y is Negative
		dY = 0xFF00;
	else dY = 0x0000;

	dX |= packet[1];
	dY |= packet[2];

	event_st* eventToAdd = createEvent(MOUSE_CHANGE);
  eventToAdd->lmb = (BIT(0) & packet[0]) == BIT(0);
	eventToAdd->rmb = (BIT(1) & packet[0]) == BIT(1);
	eventToAdd->mouseX = dX;
	eventToAdd->mouseY = dY;
  ev_queue_push(events, *eventToAdd);
}
