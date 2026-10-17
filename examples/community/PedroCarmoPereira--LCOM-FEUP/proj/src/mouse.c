#include <minix/devio.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8254.h"
#include "kbd.h"
#include "mouse.h"

static int mouse_hook_id = 4;
uint8_t byte_no = 0;
bool pintou; // i.e. It worked
struct packet p;
struct mouse_ev event;
state_t state = INIT;
uint16_t length = 0;


int (mouse_subscribe_int) (uint8_t *bit_no){
	*bit_no = BIT(mouse_hook_id);
  	sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
  	return 0;
}

int (mouse_unsubscribe_int)(){
	sys_irqrmpolicy(&mouse_hook_id);
	return 0;
}

int mouse_cmd_aux(uint8_t cmd){
	unsigned int tries = 0;
	uint32_t tmp_scan;
	kbd_cmd(MOUSE_CMD);
	while(tries < 5){
    	sys_inb(STATUS_REG, &tmp_scan);
    	if(!(tmp_scan & IBF)) {
				sys_outb(OUT_BUF, cmd);
      		return 0;
    	}
    	tries++;
    	tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int mouse_cmd(uint8_t cmd){
	uint32_t tmp_scan, r;
	mouse_cmd_aux(cmd);
	sys_inb(STATUS_REG, &tmp_scan);
	if(tmp_scan & (OBF | TOPARR_ERROR)) {
		sys_inb(OUT_BUF, &r);
		switch(r){
			case NACK:
				mouse_cmd(cmd);
				break;
			case ERROR:
				return 1;
			default:
				break;
			}
		}
	return 0;
}

int set_stream(){
	int r;
	if((r = mouse_cmd(SET_STREAM)) != 0) printf("ERROR SETTING STREAM MODE %d\n", r);
	if((r = mouse_cmd(DIS_DATA_R)) != 0) printf("ERROR DIS. DATA RECORDING %d\n", r);
	if((r = mouse_cmd(EN_DATA_R)) != 0) printf("ERROR EN. DATA RECORDING %d\n", r);
	return r;
}

int set_remote(){
	int r;
	if((r = mouse_cmd(DIS_DATA_R)) != 0) printf("ERROR DIS. DATA RECORDING %d\n", r);
	if((r = mouse_cmd(SET_REMOTE)) != 0) printf("ERROR SETTING REMOTE MODE %d\n", r);
	return r;
}

bool isNegX(uint8_t byte){
	return byte & BIT(4);
}

bool isNegY(uint8_t byte){
	return byte & BIT(5);
}

bool parseByte(uint8_t byte){
	uint16_t tmp_byte;
	if(byte_no == 0 && (byte & BIT(3))){
		p.lb = (byte & BIT(0));
		p.rb = (byte & BIT(1));
		p.mb = (byte & BIT(2));
		p.x_ov = (byte & BIT(6));
		p.y_ov = (byte & BIT(7));
		p.bytes[byte_no] = byte;
		return true;
	}

	if(byte_no == 1){
		if (isNegX(p.bytes[0])) tmp_byte = ((uint16_t) byte) | 0xFF00;
		else tmp_byte = byte;
		p.delta_x = tmp_byte;
		p.bytes[byte_no] = byte;
		return true;
	}

	if(byte_no == 2){
		if (isNegY(p.bytes[0]))tmp_byte = ((uint16_t) byte) | 0xFF00;
		else tmp_byte = byte;
		p.delta_y = tmp_byte;
		p.bytes[byte_no] = byte;
		return true;
	}
	return false;
}


void (mouse_ih)(){
	uint32_t byte;
	read_OB(&byte);
	if(parseByte((uint8_t) byte)) {
		byte_no++;
		if(byte_no > 2) byte_no = 0;
		pintou = true;
	}
	else pintou = false;
}

int mouse_poll(){
	uint32_t byte;
	mouse_cmd(READ_DATA);
	tickdelay(micros_to_ticks(DELAY_US));
	read_OB(&byte);
	if(parseByte((uint8_t) byte)){
			byte_no++;
			pintou = true;
	}
	else pintou = false;
	read_OB(&byte);
	if(parseByte((uint8_t) byte) && pintou) byte_no++;
	else pintou = false;
	read_OB(&byte);
	if (pintou)
		if(!parseByte((uint8_t) byte)) pintou = false;
	byte_no = 0;
	return 0;
}

void printState(state_t t){
	switch(t){
		case INIT:
			puts("INIT");
			break;
		case DRAW_UP:
			puts("DRAW_UP");
			break;
		case DRAW_DOWN:
			puts("DRAW_DOWN");
			break;
		case VERT:
			puts("VERT");
			break;
		case COMP:
			puts("COMP");
			break;
		default:
			break;
		}
}

void printEvent(enum mouse_ev_t ev){
	printf("%d\n", ev);
}

void mouse_process_event(struct mouse_ev * e, uint8_t len, uint8_t tol){

	enum mouse_ev_t et = (*e).type;

	int16_t dx = (*e).delta_x, dy = (*e).delta_y;

	if(state == INIT && et == LB_PRESSED){
	 state = DRAW_UP;
	 length = 0;
	}

	else if (state == DRAW_UP && et == MOUSE_MOV){
		if(dy/dx > 1){
			if (dx > 0) length += dx;
			else if (abs(dx) > tol) state = INIT;
		}
		else if (abs(dx) > tol || abs(dy) > tol)state = INIT;
	}

	else if (state == DRAW_UP && et == LB_RELEASED){
		if (length >= len) {
			state = VERT;
			length = 0;
		}
		else state = INIT;
	}

	else if (state == VERT){
	 if(et == RB_PRESSED){
	  state = DRAW_DOWN;
	}
	 else if (!(et == MOUSE_MOV && abs(dx) <= tol && abs(dy) <= tol))state = INIT;
	}

	else if (state == DRAW_DOWN && et == MOUSE_MOV){
		if(abs(dy)/dx > 1){
			if (dx > 0) length += dx;
			else if (abs(dx) > tol) state = INIT;
		}
		else if (abs(dx) > tol || abs(dy) > tol) state = INIT;
	}

	else if (state == DRAW_DOWN && et == RB_RELEASED){
		if (length >= len) state = COMP;
		else state = INIT;
	}

	else state = INIT;
}
