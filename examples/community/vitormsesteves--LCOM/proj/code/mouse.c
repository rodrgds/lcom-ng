#include "i8042.h"
#include "mouse.h"
#include "video_gr.h"
#include "rectangle.h"
#include "kbc.h"

static int hook_id = MOUSE_HOOK_ID;

Mouse* mouse = NULL;

void newMouse();

void newMouse() {
	mouse = (Mouse*) malloc(sizeof(Mouse));
	mouse->x = 0.15 * getH_res();
	mouse->y = 0.55 * getV_res();
	mouse->xSign = 0;
	mouse->ySign = 0;
	mouse->deltaX = 0;
	mouse->deltaY = 0;
	mouse->counter = 0;
	mouse->interrupts = 0;
	mouse->leftButtonDown = 0;
	mouse->middleButtonDown = 0;
	mouse->rightButtonDown = 0;
	mouse->color = rgb(0, 0, 255);
	mouse->rect = newRectangle(mouse->x, mouse->y, 10, 10);
}

Mouse* getMouse() {
	if (!mouse)
		newMouse();
	return mouse;
}

void setMouseColor(int color){
	mouse->color = color;
}

void updateMouse() {
	mouse->xSign = X_NEGATIVE(mouse->packet[0]);
	mouse->ySign = Y_NEGATIVE(mouse->packet[0]);
	if (mouse->ySign)
		mouse->deltaY = (int)(unsigned char)mouse->packet[2] - 256;
	else
		mouse->deltaY = mouse->packet[2];
	if (mouse->xSign)
		mouse->deltaX = (int)(unsigned char)mouse->packet[1] - 256;
	else
		mouse->deltaX = mouse->packet[1];
	mouse->x = mouse_outH(mouse->x + mouse->deltaX);
	mouse->y = mouse_outV(mouse->y - mouse->deltaY);
	mouse->leftButtonDown = LEFT_B(mouse->packet[0]);
	mouse->middleButtonDown = MIDDLE_B(mouse->packet[0]);
	mouse->rightButtonDown = RIGHT_B(mouse->packet[0]);
	setRecLocation(mouse->rect, mouse->x, mouse->y);
}

void drawMouse() {
	drawRectangle(mouse->rect, mouse->color, getGraphicsBuffer());
}

void deleteMouse() {
	deleteRectangle(mouse->rect);
	free(mouse);
}

void handlerMouse() {
	unsigned long data;
	mouse_read(&data);
	mouse->packet[mouse->counter] = data;

	if (mouse->counter == 0 && !(BIT(3) & mouse->packet[0]))
		return;
	mouse->counter = (mouse->counter + 1) % 3;
	mouse->interrupts++;
	if (mouse->counter != 0)
		return;
	updateMouse();
}

int mouse_write(unsigned char cmd) {
	unsigned long stat;
	int i = 0;

	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
		sys_outb(CMD_REG, WRITE_BYTE);
		//tickdelay(micros_to_ticks(DELAY_US));
		sys_outb(IN_BUF, cmd); /* no args command */
		sys_inb(OUT_BUF, &stat); /* assuming it returns OK */

		if (stat == ACK) {
			return 0;
		}
	}
//	        if (kbc_write(CMD_REG, WRITE_BYTE) != 0)
//	                return 1;
//	        if (kbc_write(DATA, cmd) != 0)
//	                return 1;
//	        return 0;
}

int mouse_read(unsigned long* val) {
	return sys_inb(DATA, val);
//	unsigned long read = 0;
//	int i = 0;
//	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
//		sys_inb(STAT_REG, &read);
//		if ((read & OBF) && (read & AUX)) {
//			if (sys_inb(OUT_BUF, val) != 0) {
//				return -1;
//			}
//			return 0;
//		}
//		tickdelay(micros_to_ticks(DELAY_US));
//	}
//	return -1;
}

int mouse_subscribe() {
	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)
			!= 0) {
		printf("\nMouse_subscribe() failed \n");
		return -1;
	}
	if (sys_irqenable(&hook_id) != 0) {
		printf("\nMouse_subscribe() failed \n");
		return -1;
	}
	mouse_write(STREAM_MODE);
	mouse_write(ENABLE_PACKETS);
	return BIT(MOUSE_HOOK_ID);
}

int mouse_unsubscribe() {
	if (sys_irqdisable(&hook_id) != 0) {
		printf("\nMouse_unsubscribe failed \n");
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != 0) {
		printf("\nMouse_unsubscribe failed \n");
		return 1;
	}
	return 0;
}

void resetMouse(){
	mouse->x = 0.15 * getH_res();
	mouse->y = 0.55 * getV_res();
}

int mouse_outH(int aux)
{
	if (aux >= getH_res())
		return getH_res();
	else if (aux <= 0)
		return 0;
	else
		return aux;
}

int mouse_outV(int aux)
{
	if (aux >= getV_res())
		return getV_res();
	else if (aux <= 0)
		return 0;
	else
		return aux;
}
