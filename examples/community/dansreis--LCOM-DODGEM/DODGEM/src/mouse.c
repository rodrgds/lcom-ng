#include"i8042.h"
#include "DODGEM.h"
#include "video_gr.h"
#include "graphics.h"
#include "utilities.h"
#include "mouse.h"
#include "bitmap.h"
#include "timer.h"

int hook_id4 = 1;

MOUSE * rato;

unsigned long mouse;
char mouse_char;

static  int xcoord = MODE1024_H_RES/2;
static  int ycoord = MODE1024_V_RES/2;

void StartMouse(){
	rato=malloc(sizeof(MOUSE));
	rato->x = MODE1024_H_RES/2;
	rato->y = MODE1024_V_RES/2;
	rato->button = 0;
	rato->lastButton = 0;
}

int MOUSE_subscribe_int() {
	int hook;
	hook = hook_id4;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&hook_id4) == OK)
		if (sys_irqenable(&hook_id4) == OK)
			return BIT(hook);

	return -1;
}

int MOUSE_unsubscribe_int() {
	if (sys_irqdisable(&hook_id4) == OK)
		if (sys_irqrmpolicy(&hook_id4) == OK)
			return 0;

	return 1;
}

int MOUSE_int_handler() {
	if (sys_inb(OUT_BUF, &mouse) != OK)
		return -1;
	mouse_char = (char)mouse;
	return 0;
}

int MOUSE_send_command(int irq){
	int irq_set;
	if ((irq_set = MOUSE_subscribe_int()) == -1){
		vg_exit();
		return -1;
	}

	if (sys_outb(KBC_CMD_REG, ENABLE_MOUSE) != OK){
		vg_exit();
		return -1;
	}
	if (sys_outb(KBC_CMD_REG, KBDCOMMAND) != OK){
		vg_exit();
		return -1;
	}
	if (sys_outb(OUT_BUF, SEND_PACKET) != OK){
		vg_exit();
		return -1;
	}
	if (rec_cmd() != ACK) {
		return -1;
	}

	return irq_set;
}

char packets[3];
int contador = 0;

int show_mouse() {
	if (((BIT(3) & mouse_char) == BIT(3)) && (contador == 0)){
		packets[0] = mouse_char;
		contador++;
		return 0;
	}
	else if (contador == 1){
		packets[1] = mouse_char;
		contador++;
		return 0;
	}
	else if (contador == 2) {
		packets[2] = mouse_char;
		print_mouse(packets);
		contador = 0;
		return 1;
	}
}

void drawMouse()
{

	if (rato->button == 0)
		drawBitmap(game->Cursor, rato->x, rato->y, ALIGN_LEFT);
	else if (rato->button == 1)
		drawBitmap(game->CursorLeft, rato->x, rato->y, ALIGN_LEFT);
	else if (rato->button == 2)
		drawBitmap(game->CursorRight, rato->x, rato->y, ALIGN_LEFT);
	else if (rato->button == 3)
		drawBitmap(game->CursorMiddle, rato->x, rato->y, ALIGN_LEFT);
	else if (rato->button == 4)
		drawBitmap(game->CursorLR, rato->x, rato->y, ALIGN_LEFT);
	else if (rato->button == 5)
		drawBitmap(game->CursorLRM, rato->x, rato->y, ALIGN_LEFT);

}

void drawMouseJogo(){
	if (Poderes->invencibilidade){
		drawBitmap(game->PlayInv, rato->x, rato->y, ALIGN_LEFT);
	}
	else{
		drawBitmap(game->PlaySquare, rato->x, rato->y, ALIGN_LEFT);
	}
}


void print_mouse(unsigned char *packets) {

	int dx = (packets[0] & BIT(4)) ? (int)packets[1] - 256 : packets[1];
	int dy = (packets[0] & BIT(5)) ? (int)packets[2] - 256 : packets[2];

	if (rato->x + dx < 0)
		rato->x = 0;
	else if (rato->x + dx > (MODE1024_H_RES - 20))
		rato->x = (MODE1024_H_RES - 20);
	else
		rato->x += dx;

	if (rato->y - dy < 0)
		rato->y = 0;
	else if (rato->y - dy > (MODE1024_V_RES - 20))
		rato->y = (MODE1024_V_RES - 20);
	else
		rato->y -= dy;

//	rato->x = xcoord;
//	rato->y = ycoord;

	rato->lastButton = rato->button;

	if ((BIT(0) & packets[0]) != 0 && (BIT(1) & packets[0]) >> 1 != 0 && (BIT(2) & packets[0]) >> 2 != 0)
		rato->button = 5; //LRM
	else if ((BIT(0) & packets[0]) != 0 && (BIT(1) & packets[0]) >> 1 != 0)
		rato->button = 4; //LR
	else if (((BIT(1) & packets[0]) >> 1) != 0)
		rato->button = 2; //R
	else if ((BIT(2) & packets[0]) >> 2 != 0)
		rato->button = 3; //M
	else if((BIT(0) & packets[0]) != 0)
		rato->button = 1; //L
	else
		rato->button = 0; //None
}

int rec_cmd(){
	unsigned long stat = 0;
	unsigned long OBF = 0x0001;
	unsigned long data = 0;

	while( 1 ) {
		sys_inb(STAT_REG, &stat);
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data);
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

void MouseBorder(){
	if (rato->x < 350)
		rato->x = 350;
	if (rato->y < 50)
		rato->y = 50;
	if (rato->y > (650 - (game->MainSquare->yf - game->MainSquare->yi)))
		rato->y = (650 - (game->MainSquare->yf - game->MainSquare->yi));
	if (rato->x > (950 - (game->MainSquare->xf - game->MainSquare->xi)))
		rato->x = (950 - (game->MainSquare->xf - game->MainSquare->xi));
}




