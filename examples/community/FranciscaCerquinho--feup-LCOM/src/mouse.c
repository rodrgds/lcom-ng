#include "mouse.h"

int hook_id_mouse=12;

Mouse* newMouse()
{
	Mouse *auxMouse;
	auxMouse=(Mouse *)malloc(sizeof(Mouse));

	auxMouse->x=0;
	auxMouse->y=0;
	auxMouse->LeftPressed=0;
	return auxMouse;
}


void mousePermissions()
{
	unsigned long st;
	sys_outb(STATUS_PORT,WRITE_MOUSE_BYTE);
	sys_inb(WRITE_COMMAND_BYTE, &st);
	sys_outb(WRITE_COMMAND_BYTE,SET_STREAM_MODE);
	sys_inb(WRITE_COMMAND_BYTE, &st);
	sys_outb(STATUS_PORT,WRITE_MOUSE_BYTE);
	sys_inb(WRITE_COMMAND_BYTE, &st);
	sys_outb(WRITE_COMMAND_BYTE,ENABLE_DATA);
	sys_inb(STATUS_PORT, &st);
}

int mouse_subscribe_int(){

	int ret=hook_id_mouse;
	if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)!= 0) //subscribes the interruption
		return 1;

	return BIT(ret);
}


void LeftButton(Mouse* auxMouse)
{
	if(auxMouse->packet[0] & BIT(0))
		auxMouse->LeftPressed=1;
	else
		auxMouse->LeftPressed=0;
}



int mouse_unsubscribe_int(){
	if(sys_irqrmpolicy(&hook_id_mouse) != 0)
		return 1;

	if(sys_irqdisable(&hook_id_mouse) != 0)
		return 1;

	return 0;
}

void mouse_print(unsigned char packet[]){

	printf("B1=0x%02x B2=Ox%02x	B3=0x%02x ", packet[0], packet[1],packet[2]);

	if(BIT(0) & packet[0])
		printf( "LB=%d	",1);
	else
		printf("LB=%d	",0);

	if(BIT(2) & packet[0])
		printf( "MB=%d	",1);
	else
		printf("MB=%d	",0);

	if(BIT(1) & packet[0])
		printf( "RB=%d	",1);
	else
		printf("RB=%d	",0);

	if(BIT(6) & packet[0])
		printf( "XOV=%d	",1);

	else
		printf("XOV=%d	",0);

	if(BIT(7) & packet[0])
		printf( "YOV=%d	",1);
	else
		printf("YOV=%d	",0);

	printf("X=");
	if(BIT(4) & packet[0])
		printf("-%d	", 256 -packet[1]);
	else
		printf("%d	", packet[1]);

	printf("Y=");
	if(BIT(5) & packet[0])
		printf("-%d	", 256 -packet[2]);
	else
		printf("%d	", packet[2]);

	printf("\n");
}

int mouse_int_handler(char packet[]){

	unsigned long read;
	static int i=0;
	sys_inb(WRITE_COMMAND_BYTE, &read);

	packet[i%3]= (char) read;

	if(i==0 && packet[0] != MOUSE_ACK && (BIT(3) & packet[0]))
		return 0;

	i++;


	if((i%3)==0){
		mouse_print(packet);
		return 1;
	}

	return 0;

}

