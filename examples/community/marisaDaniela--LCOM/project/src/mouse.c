#include "mouse.h"

int g_hook3 =3;
Mouse* mouse = NULL;
Mouse mouse_PS;

int mouse_read(unsigned long *inf) {
	unsigned long condition = 0;

	if(sys_inb(STAT_REG, &condition) != OK) {
		return -1;
	}
	if (condition & (OUT_BUF_FULL|AUX)) {
		if (sys_inb(OUT_BUF, inf) != OK) {
			return -1;
		}
	}
	return 0;
}

int mouse_write(unsigned long inf)
{
	unsigned long condition = 0;

	if(sys_inb(STAT_REG, &condition) != 0) {
		return -1;
	}
	if ((condition & IN_BUF_FULL) == 0)
	{
		if (sys_outb(CMD_REG, WRITE_TO_MOUSE) != 0) {
			return 1;
		}

		if (sys_outb(IN_BUF, inf) != 0) {
			return 1;
		}

		tickdelay(micros_to_ticks(200000));
		mouse_read(&condition);

		return condition;
	}
}

int mouse_subscribe_int() {
	int bitmask = BIT(g_hook3);
	unsigned long i = 0;
	mouse_write(STREAM_MODE_DISABLE);
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook3) != 0) {
		printf("System call sys_irqsetpolicy() failed! \n");
		return -1;
	}

	if (sys_irqenable(&g_hook3) != 0) {
		printf("System call sys_irqenable() failed! \n");
		return -1;
	}
	mouse_write(SET_STREAM_MODE);
	mouse_write(STREAM_MODE_ENABLE);

	sys_inb(OUT_BUF, &i);

	return bitmask;
}

// unsubscribe mouse
int mouse_unsubscribe_int() {
	if (sys_irqdisable(&g_hook3) != 0) {
		return -1;
	}
	if (sys_irqrmpolicy(&g_hook3) != 0) {
		return -1;
	}
	mouse_write(STREAM_MODE_DISABLE);
	return 0;
}

int mouseInside(Mouse* mouse, int xi,int xf,int yi,int yf)
{
	if (mouse->x <= xf && mouse->x >= xi && mouse->y <= yf && mouse->y >= yi)
		return 0;
	
	return -1;
}

Mouse* newMouse()
{
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 0;
	mouse->y = 0;
	
	mouse->xSign =0;
	mouse->ySign =0;
	
	mouse->xD = 0;
	mouse->yD = 0;
	
	mouse->speed = 1.7; 
	
	mouse->counter = 0;

	mouse->leftButtonDown = 0;
	mouse->rightButtonDown = 0;

	mouse->leftButtonReleased = 0;
	mouse->rightButtonReleased = 0;

	mouse->draw = 1;
	mouse->updated = 0; 
	
	mouse_PS = *mouse;

	return mouse;
}

Mouse* getMouse()
{
	if (!mouse)
	{	
		mouse = newMouse();
	}
	return mouse;
}

void updateMouse()
{
	Mouse* mouse = getMouse();

	sys_inb(OUT_BUF	, &mouse->packet[mouse->counter]);

	if((mouse->counter == 0) && (((mouse->packet[mouse->counter] & MOUSE_UNUSED ) == 0) || mouse->packet[mouse->counter] == ACK))
		return;

	mouse->counter++;

	if(mouse->counter == 3)
	{
		mouse->counter = 0;
		mouse->xD = mouse->packet[1];
		mouse->yD = mouse->packet[2];
		
		mouse->ySign = mouse->packet[0] & MOUSE_Y_SIGN;
		mouse->xSign = mouse->packet[0] & MOUSE_X_SIGN;

		mouse->leftButtonDown = mouse->packet[0] & MOUSE_LB;
		mouse->rightButtonDown = mouse->packet[0] & MOUSE_RB;


		if (mouse_PS.leftButtonDown != 0 && mouse->leftButtonDown == 0)
			mouse->leftButtonReleased = 1;
		if (mouse_PS.rightButtonDown != 0	&& mouse->rightButtonDown == 0)
			mouse->rightButtonReleased = 1;

		if(mouse->xSign)	
			mouse->xD |= (-1 << 8);	

		if(mouse->ySign)
			mouse->yD |= (-1 << 8);


		/* Updating the X coordinate */
		if (mouse->xD != 0)
		{
			if((mouse->x += mouse->speed * mouse->xD) < 0)
				mouse->x = 0;
			else if (mouse->x >= getHorResolution())
				mouse->x = getHorResolution() - 1;
		}

		if (mouse->yD != 0)
		{
			if((mouse->y -=  mouse->yD * mouse->speed ) < 0)
				mouse->y = 0;
			else if (mouse->y >= getVerResolution())
				mouse->y = getVerResolution() - 1;
		}

		mouse->xD = mouse->x - mouse_PS.x;
		mouse->yD = mouse->y - mouse_PS.y;
		mouse_PS = *mouse;

		mouse->updated = 1;
		mouse->draw = 1;
	}
}

void deleteMouse()
{
	free(getMouse());
}
