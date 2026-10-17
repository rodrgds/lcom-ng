#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/sysutil.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "kbd.h"

int hook_id_mouse = 12;
unsigned long packet[3];
int mouse_counter = 0;
int fullPacket = 0;

Mouse* initMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));
	mouse->deltaX = 0;
	mouse->deltaY = 0;

	return mouse;
}

void mouseHandler(Mouse* mouse) {
	mouse_handler();
	if (fullPacket) {
		long newDeltaX, newDelatY;

		newDeltaX = getDeltaX();
		newDelatY = getDeltaY();

		if (hasSameSignal(mouse->deltaX, newDeltaX))
			mouse->deltaX += getDeltaX();
		else
			mouse->deltaX = getDeltaX();

		if (hasSameSignal(mouse->deltaY, newDelatY))
			mouse->deltaY += getDeltaY();
		else
			mouse->deltaY = getDeltaY();

	}

}

void stopMouse(Mouse* mouse) {
	free(mouse);
}

void resetMouse(Mouse* mouse) {
	mouse->deltaX = 0;
	mouse->deltaY = 0;

}

long getDeltaX() {
	return convertNumber(packet[1], packet[0] & MOUSE_XSIGN);
}

long getDeltaY() {
	return convertNumber(packet[2], packet[0] & MOUSE_YSIGN);
}

int mouse_subscribe_int(void) {

	int temp_hook_id_mouse = hook_id_mouse;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE,
			&hook_id_mouse) != Ok)
		return IRQ_MOUSE_SET_ERROR;

	if (sys_irqenable(&hook_id_mouse) != Ok)
		return IRQ_MOUSE_ENAB_ERROR;

	return BIT(temp_hook_id_mouse);

}

int mouse_subscribe_Exc_int(void) {

	int temp_hook_id_mouse = hook_id_mouse;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE, &hook_id_mouse) != Ok)
		return IRQ_MOUSE_SET_ERROR;

	if (sys_irqenable(&hook_id_mouse) != Ok)
		return IRQ_MOUSE_ENAB_ERROR;

	return BIT(temp_hook_id_mouse);

}

int mouse_unsubscribe_int() {

	if (sys_irqdisable(&hook_id_mouse) != Ok)
		return IRQ_MOUSE_DISAB_ERROR;

	if (sys_irqrmpolicy(&hook_id_mouse) != Ok)
		return IRQ_MOUSE_REM_ERROR;

	return Ok;
}

unsigned long readByte() {
	unsigned long stat, byte;

	while (1) {
		if (sys_inb(STAT_REG, &stat) != Ok)
			return KBD_SYS_IN_ERROR;

		if (stat & OBF) {
			if ((stat & (PAR_ERR | TO_ERR | IBF)) == 0) {
				if (sys_inb(OUT_BUF, &byte) != Ok)
					return KBD_SYS_IN_ERROR;

				return byte;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

}

void mouse_handler() {
	unsigned long byte_read = readByte();

	switch (mouse_counter) {
	case 0:
		if (byte_read & BIT(3)) {
			packet[mouse_counter] = byte_read;
			mouse_counter++;
			fullPacket = 0;
		}
		break;

	case 1:
		packet[mouse_counter] = byte_read;
		mouse_counter++;
		break;
	case 2:
		packet[mouse_counter] = byte_read;
		mouse_counter = 0;
		fullPacket = 1;
		break;
	}

}

long convertNumber(char number, unsigned long msb) {
	if (msb == 0)
		return number;

	long teste = -((~(number | 0x100) + 1) & 0xFF);
	return teste;

}

int print_packets() {

	printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);
	printf("LB=%d ", packet[0] & MOUSE_LB ? 1 : 0);
	printf("MB=%d ", packet[0] & MOUSE_MB ? 1 : 0);
	printf("RB=%d ", packet[0] & MOUSE_RB ? 1 : 0);
	printf("XOV=%d ", packet[0] & MOUSE_XOVFL ? 1 : 0);
	printf("YOV=%d ", packet[0] & MOUSE_YOVFL ? 1 : 0);

	long xdelta = convertNumber(packet[1], packet[0] & MOUSE_XSIGN);
	long ydelta = convertNumber(packet[2], packet[0] & MOUSE_YSIGN);

	printf("X=%d ", xdelta);
	printf("Y=%d \n", ydelta);

	return 0;
}

int enable_stream_mode() {

	if (mouseWriteCommandByte(SET_STREAM_MODE) != Ok)
		return MOUSE_WRITE_CMD_ERROR;

	if (mouseWriteCommandByte(ENABLE_DR) != Ok)
		return MOUSE_WRITE_CMD_ERROR;

	return Ok;
}

int mouseWriteCommandByte(unsigned long cmd) {
	unsigned long stat;
	while (1) {
		if (sys_inb(STAT_REG, &stat) != Ok) {
			return KBD_SYS_IN_ERROR;

		}

		if ((stat & IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != Ok)
				return KBD_SYS_OUT_ERROR;

			if (sys_outb(IN_BUF, cmd) != Ok) {
				return MOUSE_SYS_OUT_ERROR;
			}
			unsigned long mouseOutput;
			mouseOutput = mouseReadOutput();
			if (mouseOutput == ACK) {
				return Ok;
			}

		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

unsigned long mouseReadOutput() {
	unsigned long stat, mouseOutput;
	while (1) {
		if (sys_inb(STAT_REG, &stat) != Ok)
			return KBD_IN_CMD_ERROR;

		if (stat & OBF) {

			if (sys_inb(OUT_BUF, &mouseOutput) != Ok)
				return KBD_IN_CMD_ERROR;

			if ((stat & (PAR_ERR | TO_ERR)) == Ok)
				return mouseOutput;
			else
				return KBD_IN_CMD_ERROR;

		}

		tickdelay(micros_to_ticks(DELAY_US));
	}
}

short hasSameSignal(long number1, long number2) {
	return ((number1 < 0) == (number2 < 0));
}

