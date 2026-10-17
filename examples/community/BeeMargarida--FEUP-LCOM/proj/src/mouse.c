#include "mouse.h"


int count = 0, counter = 0;
unsigned char packet[3];


static int mouse_wait_ack(void) {
	unsigned long status, reply;
	unsigned int retries;
	for (retries = 0; retries < 100; ++retries) {
		if (sys_inb(KBC_CMD_REG, &status) != OK) return 1;
		if (status & OBF) {
			if (sys_inb(OUT_BUF, &reply) != OK) return 1;
			return reply != 0xFA;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 1;
}

void mouse_set_stream() {

	//retorna 1 em caso de sucesso, 0 em caso de falha
	unsigned long stat;
	//Write command to mouse

	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	tickdelay(micros_to_ticks(DELAY_US));

	//Activate stream mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, STREAM_MODE); //0xEA
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//tickdelay(micros_to_ticks(DELAY_US));
	if (mouse_wait_ack() != 0) return;

	//Write command to mouse again
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	tickdelay(micros_to_ticks(DELAY_US));
	//Activate data mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, DATA_MODE); //0xF4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	//tickdelay(micros_to_ticks(DELAY_US));
	if (mouse_wait_ack() != 0) return;

	//sys_inb(OUT_BUF, &stat);
}

int mouse_disable_stream(){
	unsigned long stat;
	int end = 1;
	//Write command to mouse
	sys_inb(KBC_CMD_REG,&stat);

	while(1){
		if((stat & IBF) == 0){
			sys_outb(KBC_CMD_REG, WRITE_MOUSE); //0xD4
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	//Disable stream mode
	while(1){
		sys_inb(KBC_CMD_REG,&stat);
		if((stat & IBF) == 0){
			sys_outb(IN_BUF, DIS_STREAM_MODE); //0xF5
			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	if (mouse_wait_ack() != 0) return 0;
	return 1;
}

int mouse_handler(unsigned char packets[3]) {
	//retorna 1 em caso de sucesso, 0 em caso de falha, para ser usado em condições no mouse_int

	unsigned long data;
	sys_inb(OUT_BUF, &data);

	if(counter != 2 && counter != 3){
		packet[0] = 0;
		packet[1] = 0;
		packet[2] = 0;

		packet[0] = data;
		if((packet[0] & BYTE_3) == 0){
			packet[0] = 0;
			counter = 1;
			return 0;
		}
		else{
			counter = 2;
			return 0;
		}
	}
	if(counter == 2){
		packet[1] = data;
		counter = 3;
		return 0;
	}
	if(counter == 3){
		packet[2] = data;
		counter = 1;
		return 1;
	}
	return 0;

}


void updateCursor(Cursor *c, unsigned char packets[3]){
	double dis = 0;
	if((XSIGN & packet[0]) != 0){ //negative X
		dis = (0xFF & ~packet[1]) + 1;
		if(c->x - dis > 0)
			c->x = c->x - dis;
		else
			c->x = 0;
	}
	else{ //positive x
		dis = packet[1];
		if(c->x + c->size + dis < 1024)
			c->x += dis;
	}
	if((YSIGN & packet[0]) != 0){ //negative y
		dis = (0xFF & ~packet[2]) + 1;
		if(c->y + c->size + dis < 768)
			c->y += dis;
	}
	else{ //positive y
		dis = packet[2];
		if(c->y - dis > 0)
			c->y -= dis;
	}

	if(LB & packet[0])
		c->left = 1;
	else
		c->left = 0;
}


void checkCursor(Menu *menu){

	if(menu->cursor->x >= menu->Opcao1->xi && menu->cursor->x <= menu->Opcao1->xi + menu->Opcao1->wd && menu->cursor->y >= menu->Opcao1->yi && menu->cursor->y + menu->cursor->size <= menu->Opcao1->yi + menu->Opcao1->ht && menu->cursor->left == 1){
		menu->checkOpcao1 = 1;
	}
	if(menu->cursor->x >= menu->Opcao2->xi && menu->cursor->x <= menu->Opcao2->xi + menu->Opcao2->wd && menu->cursor->y >= menu->Opcao2->yi && menu->cursor->y + menu->cursor->size <= menu->Opcao2->yi + menu->Opcao2->ht && menu->cursor->left == 1){
		menu->checkOpcao2 = 1;
	}
}
