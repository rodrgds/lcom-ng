#include <minix/drivers.h>
#include <stdbool.h>
#include "mouse.h"
#include "Bitmap.h"
#include "interface.h"

unsigned long irq_set;



int ipc_status;
message msg;
int conta = 0, ind = 0, r, ipc_status;
unsigned long irq_set, data;
char cmd;
message msg;

Mouse* mouse=NULL;

Mouse* getMouse() {
	unsigned long r = 0;
	unsigned long stat;
	if (mouse == NULL) {
		while (r != ACK) {
			sys_outb(STAT_REG, ENABLE_MOUSE);
			while (1) {
				sys_inb(STAT_REG, &stat);
				if ((stat & IBF) == 0) {
					sys_outb(STAT_REG, W_TO_MOUSE);
					break;
				}

				tickdelay(micros_to_ticks(DELAY_US));
			}
			while (1) {
				sys_inb(STAT_REG, &stat);
				if ((stat & IBF) == 0) {
					sys_outb(OUT_BUF, ENABLE_SEND);
					break;
				}

				tickdelay(micros_to_ticks(DELAY_US));
			}
			sys_inb(OUT_BUF, &r);
		}
		mouse = newMouse();
	}

	return mouse;
}

void setMouse(Mouse* m){
	mouse=m;
}

Mouse* newMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 400;
	mouse->y = 300;

	mouse->image = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/mouse.bmp");

/*
	if ((irq_set = mouse_subscribe()) == -1) {
		printf("Unable to subscribe mouse!\n");
	}
	if (sys_outb(STAT_REG, ENABLE_MOUSE) != OK) //rato enable
		printf("Error\n");
	if (sys_outb(STAT_REG, W_TO_MOUSE) != OK) //MOUSE
		printf("Error-MC\n");
	if (sys_outb(OUT_BUF, ENABLE_SEND) != OK) //Ativar o envio
		printf("Error-SEND\n");
*/
	return mouse;
}


void drawMouse(){
	Mouse* m=getMouse();
	drawBitmap(m->image,m->x,m->y,ALIGN_LEFT);
	memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
}

void updateMouse() {
	Mouse* mouse=getMouse();
	int mouseVelocity = 2; //higher is slower
	int convert = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);

	/*
	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
	}
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & irq_set) {*/

				get_packet();

				int ys = 0;
				int xs = 0;


				int mb = 0;
				int rb = 0;
				int lb = 0;


				if (packet[0] & BIT(2))
						mb = 1;
				if (packet[0] & BIT(1))
						rb = 1;
				if (packet[0] & BIT(0))
						lb = 1;

				if(mb){
					mouse->mb_pressed=1;
					mouse->mb_released=0;
				}else{
					mouse->mb_pressed=0;
					mouse->mb_released=1;
				}
				if(rb){
					mouse->rb_pressed=1;
					mouse->rb_released=0;
				}else{
					mouse->rb_pressed=0;
					mouse->rb_released=1;
				}
				if(lb){
					mouse->lb_pressed=1;
					mouse->lb_released=0;
				}else{
					mouse->lb_pressed=0;
					mouse->lb_released=1;
				}


				if (packet[0] & BIT(5))
					ys = 1;
				if (packet[0] & BIT(4))
					xs = 1;

				if (packet[1] & BIT(7)) {
					char p = packet[1] ^ convert;
					p++;
					if (xs == 0) {
						mouse->x += (int) ((short) p / mouseVelocity);
					} else {
						if (abs((int) ((short) p / mouseVelocity)) > mouse->x) {
							if (mouse->x > 2) {
								mouse->x--;
							}
						} else {
							mouse->x -= (int) ((short) p / mouseVelocity);
						}
					}

				} else if (xs == 0) {
					mouse->x += (int) (packet[1] / mouseVelocity);
				} else {
					if (abs((int) (packet[1] / mouseVelocity)) > mouse->x) {
						if (mouse->x > 2) {
							mouse->x--;
						}
					} else {
						mouse->x -= (int) (packet[1] / mouseVelocity);
					}

				}

				if (packet[2] & BIT(7)) {
					char p = packet[2] ^ convert;
					p++;
					if (ys == 1) {
						mouse->y += (int) ((short) p / mouseVelocity);
					} else {
						if (abs((int) ((short) p / mouseVelocity)) > mouse->y) {
							if (mouse->y > 2) {
								mouse->y--;
							}
						} else {
							mouse->y -= (int) ((short) p / mouseVelocity);
						}

					}
				} else if (ys == 1) {
					mouse->y += (int) (packet[2] / mouseVelocity);
				} else {
					if (abs((int) (packet[2] / mouseVelocity)) > mouse->y) {
						if (mouse->y > 2) {
							mouse->y--;
						}
					} else {
						mouse->y -= (int) (packet[2] / mouseVelocity);
					}

				}

				if (mouse->x > (getHorResolution() - 35)) {
					mouse->x = (getHorResolution() - 35);
				} else if (mouse->x < 1) {
					mouse->x = 1;
				}
				if (mouse->y > (getVerResolution() - 48)) {
					mouse->y = (getVerResolution() - 48);
				} else if (mouse->y < 1) {
					mouse->y = 1;
				}

			/*}
			break;
		default:
			break;
		}
	}*/

	setMouse(mouse);
}


int mouse_subscribe() {
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_mouse)
			== OK)
		if (sys_irqenable(&hook_mouse) == OK)
			return BIT(NOTIFICATION_MOUSE);
	return -1;
}

int mouse_unsubscribe() {

	if (sys_outb(STAT_REG, W_TO_MOUSE) != OK)
		printf("ERROR-MC");
	if (sys_outb(OUT_BUF, DISABLE_STREAM) != OK)
		printf("ERROR-DISABLE_STREAM");
	if (sys_inb(OUT_BUF, &data) != OK)
		printf("OUT_BUF not full!\n");
	if (data != ACK)
		printf("Not ACK!\n");

	if (sys_irqdisable(&hook_mouse) == OK)
		if (sys_irqrmpolicy(&hook_mouse) == OK)
			return 0;
	return -1;
}

int mouse_write(unsigned long cnt, unsigned char cmd) {
	unsigned long stat = 0;
	unsigned int i = 0;
	while (i < 10) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(cnt, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return 1;
}

int mouse_read() {
	unsigned long stat, data;
	unsigned int i = 0;
	int ind = 0;

	while (i < 10) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data);
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}



int get_packet(){

	long data;

	data=mouse_read();
	if(data & BIT(3)){
		packet[0] = data;
		data=mouse_read();
		packet[1] = data;
		data=mouse_read();
		packet[2] = data;
	}
	else{
		data=mouse_read();
		if(data & BIT(3)){
			packet[0] = data;
			data=mouse_read();
			packet[1] = data;
			data=mouse_read();
			packet[2] = data;
		}
		else{
			data=mouse_read();
			if(data & BIT(3)){
				packet[0] = data;
				data=mouse_read();
				packet[1] = data;
				data=mouse_read();
				packet[2] = data;
			}
		}
	}
}


void mouse_print() {
	int mb = 0, rb = 0, lb = 0, yo = 0, xo = 0, ys = 0, xs = 0;
	int convert = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
	if (packet[0] & BIT(7))
		yo = 1;
	if (packet[0] & BIT(6))
		xo = 1;
	if (packet[0] & BIT(5))
		ys = 1;
	if (packet[0] & BIT(4))
		xs = 1;
	if (packet[0] & BIT(2))
		mb = 1;
	if (packet[0] & BIT(1))
		rb = 1;
	if (packet[0] & BIT(0))
		lb = 1;
	printf("B1: 0x%x ", packet[0]);
	printf("B2: 0x%x ", packet[1]);
	printf("B3: 0x%x ", packet[2]);
	printf("LB: %*d ", 1, lb);
	printf("MB: %*d ", 1, mb);
	printf("RB: %*d ", 1, rb);
	printf("XOV: %*d ", 1, xo);
	printf("YOV: %*d ", 1, yo);
	printf("ys: %*d ", 1, ys);
	printf("xs: %*d ", 1, xs);


	if (packet[1] & BIT(7)) {
		char p = packet[1]^convert;
		p++;
		printf("X:%d ", (short) p);
	} else
		printf("X:%d ", packet[1]);
	if (packet[2] & BIT(7)) {
		char p = packet[2] ^ convert;
		p++;
		printf("Y:%d ", (short) p);
	} else
		printf("Y:%d ", packet[2]);
	printf("\n");
}




int mouseInside(int x1, int y1, int x2, int y2){
	return x1 <= mouse->x && mouse->x <= x2 && y1 <= mouse->y && mouse->y <= y2;
}
int mouseInsideBox(Box* box){
	return mouseInside(box->x1,box->y1,box->x2,box->y2);
}

