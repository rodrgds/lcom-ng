#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "mouse.h"
#include "Utilities.h"

//VARS============================
int irqMousedBIT=KBD_AUX_IRQ;//Mouse's bit on irq_set
int ddMouseID;
unsigned int packetIndice=0;
unsigned char packet[3];
unsigned short packetsCounter;
unsigned int local_irq_set;
//===display packets
short displayPackets = 0;
//===info vars
int xDelta=0;//a package x dislocation
int yDelta=0;//a package y dislocation
int lButtonState=0;//left Mouse button state (0=not pressed,1=pressed)
int rButtonState=0;//right Mouse button state (0=not pressed,1=pressed)
int mButtonState=0;//middle Mouse button state (0=not pressed,1=pressed)
//END VARS========================

int Mouse_subscribe_int(void ) {
	int hookid=irqMousedBIT;//TIMER0_IRQ;
	local_irq_set=1<<irqMousedBIT;

	//Subscribe a notification on every interrupt in the input irq_line.
	if(sys_irqsetpolicy(irqMousedBIT, IRQ_REENABLE| IRQ_EXCLUSIVE, &hookid)!=OK)
	{
			printf("Subscription FAILED!\n");
			return -1;
	}
	ddMouseID=hookid;
	return irqMousedBIT;
}

int Mouse_unsubscribe_int(){
	//Disables interrupts on the IRQ line associated with the specified hook_id
	int hookid=ddMouseID;

	if(sys_irqdisable(&hookid)!=OK)
	{
		printf("Unsubscription FAILED!\n");
		return -1;
	}

	return 0;
}

/*int Mouse_enable()
{
	//write command byte (i think this is not needed
	 * if(sys_inb(STAT_REG,&status)!=OK)
		{
			printf("Failed to get status from KBC");
			return -1;
		}
		if(status & IBF)
		{
			printf("Input buffer full, could not write command");
			return -1;
		}
	 * unsigned long commandByde=BIT(1);
	sys_outb(DATA_REG,commandByde);
	return 0;
}*/

int Mouse_sendCommand(unsigned long commandArgument)
{
	unsigned long acknowledgmentByte=0;
	unsigned long status;
	int loopControl=50;
	while (acknowledgmentByte!=ACK && loopControl>0)
	{
		DEBUG_SDEC(loopControl);
		loopControl--;
		//1)send COMMAND 0xD4 to 64
		if(write_to(STAT_REG,WRITEBYTE_2MOUSE_KBC)!=OK)
		{
			printf("\nFailed to write 0xD4 command to KBC");
			return -1;
		}
		//2)set byte (command argument) to be sent
		if (write_to(DATA_REG,commandArgument)!=OK)
		{
			printf("\nFailed to write Mouse command to 0x60");
			return -1;
		}
		//3)get acknowledgmentByte
		read_DATA_REG_Byte(&acknowledgmentByte);
		DEBUG_HEXADEC(acknowledgmentByte);
		if(acknowledgmentByte==ERROR)
		{
			printf("\nError, received two consecutive invalid bytes from Mouse controller");
			return -1;
		}
	}//repeat if not acknowledged

	//clear buffer?? did not solve 1st byte problem
	//sys_inb(DATA_REG,&acknowledgmentByte);

	return 0;
}

int Mouse_enable_stream()
{
	//default is stream MODE if(Mouse_sendCommand(MCMD_SET_STREAM_DATA)!=OK) return -1;
	if(Mouse_sendCommand(MCMD_ENABLE_SEND_DATAPACKS)!=OK)
	{
		printf("\nFailed to enable stream");
		return -1;
	}
return 0;
}


int Mouse_read_packet_byte()
{
	unsigned long data;
	if(read_DATA_REG_Byte(&data)!=OK) return -1;
	//sync 1st package
	if (packetIndice==0 && !(data&BIT(3)) )
	{
		printf("\nbadSync...");
		return 0;
	}
	packet[packetIndice]=data;
	packetIndice=(packetIndice+1)%3;
	return 0;
}

int Mouse_getConfig()
{
	unsigned long response,clear;

	//Make sure minix doesnt "steal" our out_buf answer
	Mouse_subscribe_int();

	//Make sure we disable Stream Mode before issuing any command to Mouse
	if(Mouse_sendCommand(MCMD_DISABLE_STREAM_MODE)!=OK){
		printf("Mouse_getConfig: Could not disable Stream Mode.\n");
		return -1;
	}


	//Request Status
	if (Mouse_sendCommand(MCMD_STATUS_REQUEST) != OK) {
	        printf("Mouse_getConfig: Mouse_SendCommand failed.\n");
	        return -1;
	}
	//Read Byte 1
	while(1){
			read_DATA_REG_Byte(&response);
			//Syncronizing driver with Mouse - bit 3 and bit 1 must be "0" ( on Lab Handout)
			if(!(response & BIT(7)) && !(response & BIT(3)))
				break;
		}

	//Read Byte 2

	while (1){
		read_DATA_REG_Byte(&response);
		//Resolution is always between 0 and 3
		if(response < 4 && response >= 0)
			break;
	}
	packet[1]= response;

	//Read Byte 3
	read_DATA_REG_Byte(&response);
	packet[2]= response;


	Mouse_unsubscribe_int();


	return 0;
}

void Mouse_displayPacket()
{
	//supposing that the bytes come in the right order
	unsigned char stateByte = packet[0]	;

	printf("B1=%d  ",stateByte);
	printf("B2=%d  ",packet[1]);
	printf("B3=%d  ",packet[2]);
	//format of print package B1=0x8 	B2=0x12 	B3=0x14 	LB=0		MB=0 	RB=0 	XOV=0 	YOV=0 	X=18 	Y=20
	//Y overflow 	X overflow 	Y sign bit 	X sign bit 	Always 1 	Middle Btn 	Right Btn 	Left Btn
	if(stateByte&1) printf("LB=1"); else printf("LB=0"); printf(" ");
	if(stateByte&BIT(2)) printf("MB=1"); else printf("MB=0"); printf(" ");
	if(stateByte&BIT(1)) printf("RB=1"); else printf("RB=0"); printf(" ");
	if(stateByte&BIT(6)) printf("XOV=1"); else printf("XOV=0"); printf(" ");
	if(stateByte&BIT(7)) printf("YOV=1"); else printf("YOV=0"); printf(" ");

	//note 0x100 is in hexa not binary!
	//shift the sign bit to the the 9'sbit position to make & with "max number"
	//if it's true subtract "max" to get the 2's complement
	int dx = packet[1] - ((stateByte << 4) & 0x100);
	int dy = packet[2] - ((stateByte << 3) & 0x100);
	 printf("X=%d ",dx);
	 printf("Y=%d",dy);
	 printf("\n");
}

void Mouse_displayConfig()
{
	short check;//works as a bool

	check=packet[0]&BIT(6);
	printf("Remote = %d ->",check);
	if(check) printf("Report Mode");
	else printf("Stream Mode");

	check=packet[0]&BIT(5);
	printf("\nEnable = %d ->",check);
	if(check) printf("Enabled");
	else printf("Disabled");

	check=packet[0]&BIT(4);
	printf("\nScaling = %d ->",check);
	if(check) printf("Scaling is 2:1");
	else printf("Scaling is 1:1");

	check=packet[0]&BIT(2);
	printf("\nLeft = %d ->",check);
	if(check) printf("Left Button Pressed");
	else printf("Left Button Released");
	check=packet[0]&BIT(1);
	printf("\nMiddle = %d ->",check);
	if(check) printf("Middle Button Pressed");
	else printf("Middle Button Released");
	check=packet[0]&1;
	printf("\nRight = %d ->",check);
	if(check) printf("Right Button Pressed");
	else printf("Right Button Released");

	check = BIT( packet[1] );
	printf("\nResolution = %d -> %d counts per mm",packet[1],check);
	printf("\nSample Rate = %d",packet[2]);
}

void Mouse_int_handler_temp(unsigned short cnt){
	int ipc_status;
	message msg;
	packetsCounter=cnt;
	printf("\ndebug0");
	while (packetsCounter>0){
		/* Get a request message. */
		int r;
		if ( (r = driver_receive(ANY, &msg, &ipc_status))  != 0 )
		{
			printf("driver_receive failed with: %d", r);
			return;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:	/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & local_irq_set) { /* subscribed interrupt */
					printf("\ndebug1_packetsCounter:%d",packetsCounter);
					Mouse_int_handler(msg.NOTIFY_ARG);
				}
				/*no other notifications expected: do nothing */
			}
		} else {
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
}

//after having the code working we should try to
//use this one with the ih on "intHandler.h"
void Mouse_int_handler(unsigned int bitmask){

	if (bitmask & (1<<irqMousedBIT))
	{
		//DEBUG_SDEC(packetIndice);
		if (Mouse_read_packet_byte()!=OK) return ;
		if (packetIndice==0){
			if (displayPackets) Mouse_displayPacket();
			packetsCounter++;//packetsCounter--;
			//update Mouse info
			lButtonState = packet[0]&1;
			mButtonState = (packet[0]&BIT(2))>>2;
			rButtonState = (packet[0]&BIT(1))>>1;
			xDelta=packet[1] - ((packet[0] << 4) & 0x100);
			yDelta=packet[2] - ((packet[0] << 3) & 0x100);
		}
	}
}


void Mouse_resetPacketsCounter() { packetsCounter=0;}
int Mouse_getPacketsCounter() { return packetsCounter;}


void Mouse_activatePacketsDisplay() {displayPackets=1;}

int Mouse_getXdelta(){return xDelta;}
int Mouse_getYdelta(){return yDelta;}

void Mouse_resetDeltas(){
	xDelta=0;
	yDelta=0;
}

int Mouse_getLbuttonState(){return lButtonState;}
int Mouse_getRbuttonState(){return rButtonState;}
int Mouse_getMbuttonState(){return mButtonState;}

short Mouse_packageRead() {return !(packetIndice==0);}
