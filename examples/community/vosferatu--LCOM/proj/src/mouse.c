#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse.h"
#include "utils.h"
#include "i8042.h"
#include "graphics.h"
#include "rectangle.h"
#include <stdlib.h>

int mouse_hook = MOUSE_IRQ;

Mouse* mouse = NULL;

Mouse* newMouse(){
	if (mouse != NULL)
		return mouse;

	mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = get_hres()/2;
	mouse->y = get_vres()/2;
	mouse->leftButtonPressed = 0;
	mouse->leftButtonReleased = 0;
	mouse->readingInfo = 0;
	mouse->update = 0;
	mouse->draw = 1;
	mouse->bmp = loadBitmap("cursor.bmp");
	enableMouse();
	return mouse;
}


void updateMouse(){

	switch(mouse->readingInfo){
		case 0:
			mouse_int_handler(mouse->readingInfo, mouse->packet);
			if((mouse->packet[0] != ACK) && (mouse->packet[0] & BIT(3))){
				mouse->readingInfo = 1;
			}
			break;
		case 1:
			mouse_int_handler(mouse->readingInfo, mouse->packet);
			mouse->readingInfo = 2;
			break;
		case 2:
			mouse_int_handler(mouse->readingInfo, mouse->packet);
			mouse->readingInfo = 0;
			mouse->update = 1;
			break;
		default:
			break;
	}

	if(mouse->update){
		//update LMB
		if (!(mouse->packet[0] & LMB_PRESSED)) {
			mouse->leftButtonReleased = 0;
		}

		if (mouse->leftButtonPressed && !(mouse->packet[0] & LMB_PRESSED)) {
			mouse->leftButtonPressed = 0;
			mouse->leftButtonReleased = 1;
		}
		if (mouse->packet[0] & LMB_PRESSED) {
			mouse->leftButtonPressed = 1;
			mouse->leftButtonReleased = 0;
		}


		//Update x position
		if (mouse->packet[0] & X_SIGNED) {
			short xdiff = ( (0xFF << 8) | mouse->packet[1]);
			mouse->x += xdiff;
			if (mouse->x < 0)
				mouse->x = 0;
			if (mouse->x > get_hres())
				mouse->x = get_hres()-10;//bmp width
		} else {
			mouse->x += mouse->packet[1];
			if (mouse->x > get_hres())
				mouse->x = get_hres()-10;//bmp width
			if (mouse->x < 0)
				mouse->x = 0;
		}

		//Update y position
		if (mouse->packet[0] & Y_SIGNED) {
			short ydiff = ((0xFF << 8) | mouse->packet[2]);
			mouse->y -= ydiff;
			if (mouse->y > get_vres())
				mouse->y = get_vres()-16;//bmp height
			if (mouse->y < 0)
				mouse->y = 0;
		} else {
			mouse->y -= mouse->packet[2];
			if (mouse->y < 0)
				mouse->y = 0;
			if (mouse->y > get_vres())
				mouse->y = get_vres()-16;//bmp height
		}
		mouse->draw = 1;
		mouse->update = 0;
	}


}

Mouse* getMouse(){
	if(mouse == NULL){
		enableMouse();
		mouse = newMouse();
	}
	return mouse;
}

void enableMouse(){
	mouse_interface(SET_STREAM);
}

void disableMouse(){
	mouse_interface(DISABLE_STREAM);
}

void drawMouse(){
	drawBitmapT(mouse->bmp, mouse->x, mouse->y, ALIGN_LEFT);
	flip_Mouse();
	mouse->draw = 0;
}

void deleteMouse(){
	deleteBitmap(mouse->bmp);
	disableMouse();
	free(mouse);
}

int mouseInsideRect(Rectangle* rect){
	if(mouse->x >= rect->xi && mouse->x <= rect->xf &&
	   mouse->y >= rect->yi && mouse->y <= rect->yf){
  		return 1;
	}
	else
		return 0;
}


int mouse_subscribe_int() {
	unsigned long status;

	if ((sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE),
			&mouse_hook) == OK) && (sys_irqenable(&mouse_hook) == OK)) {
		do {
			mouse_write(KBC_CMD, ENABLE_MOUSE);
			mouse_write(KBC_CMD, WRITE_TO_MOUSE);
			mouse_write(OUT_BUF, ENABLE_PACKETS);
			mouse_read(&status);
		} while (status != ACK);

		return BIT(MOUSE_IRQ); //return activated bit by kernel on hook_id. which is value we set
	} else {
		printf("\nCouldn't subscribe mouse interrupts.\n");
		return -1;  //return negative on failure
	}
}

int mouse_unsubscribe_int() {
	if ((sys_irqdisable(&mouse_hook) == OK)
			&& (sys_irqrmpolicy(&mouse_hook) == OK))
		return 0;
	else {
		printf("\nUnable to unsubscribe mouse interrupts.\n");
		return 1;
	}
}

int mouse_read(unsigned long *data) {
	unsigned long stat;
	unsigned int i = 0;

	while (i < N_TRY) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;
		if ((stat & IN_OK) == 0) {
			if (sys_inb(OUT_BUF, data) != OK)
				return -1;
			if ((stat & STAT_ERR) == 0)
				return 0;
			else
				return -1;
		}
		//tickdelay(micros_to_ticks(DELAY_MS));
		i++;
	}

}

int mouse_write(char where_to, unsigned long command) {
	unsigned long stat;
	unsigned int i = 0;

	while (i < N_TRY) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;
		if ((stat & IN_OK) == 0) {
			if (sys_outb(where_to, command) != OK)
				return -1;
			else
				return 0;
		}
		//tickdelay(micros_to_ticks(DELAY_MS));
		i++;
	}

	return -1;
}

void mouse_interface(unsigned long command) {
	unsigned long status;

	do {
		mouse_write(STAT_REG, WRITE_TO_MOUSE);
		mouse_write(OUT_BUF, command);
		mouse_read(&status);
	} while (status != ACK);

	return;
}

void mouse_int_handler(unsigned int count, char packets[]) {
	unsigned long onebyte;
	unsigned int i = -1;

	while (i != 0) {
		i = mouse_read(&onebyte);
	}

	packets[count] = (char) onebyte;

	return;
}

void clear_mouse_buffer(){
	unsigned long status;
	sys_inb(STAT_REG, &status);

	while(status & OUT_OK){
		unsigned long garbage;
		sys_inb(OUT_BUF, &garbage);
		sys_inb(STAT_REG, &status);
	}
}
