static int mouse_hook_id;
#include "mouse.h"
#include "utils.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"

int mouseCounter = 0;

void setMousePackets() {

	//MOUSE_LEFT_BUTTON
	if (mouse_t.packet[0] & MOUSE_LEFT_BUTTON)
		mouse_t.mouse_left_button = 1;
	else
		mouse_t.mouse_left_button = 0;

	//MOUSE_MIDDLE_BUTTON
	if (mouse_t.packet[0] & MOUSE_RIGHT_BUTTON)
		mouse_t.mouse_right_button = 1;
	else
		mouse_t.mouse_right_button = 0;

	//MOUSE_RIGHT_BUTTON
	if (mouse_t.packet[0] & MOUSE_MIDDLE_BUTTON)
		mouse_t.mouse_middle_button = 1;
	else
		mouse_t.mouse_middle_button = 0;

	//MOUSE_X_SIGN
	if (mouse_t.packet[0] & MOUSE_X_SIGN) {
		mouse_t.sign_x = 1;
	}
	else {
		mouse_t.sign_x = 0;
	}

	//MOUSE_Y_SIGN
	if (mouse_t.packet[0] & MOUSE_Y_SIGN) {
		mouse_t.sign_y = 1;
	}
	else {
		mouse_t.sign_y = 0;
	}

	//MOUSE_X_OVERFLOW
	if (mouse_t.packet[0] & MOUSE_X_OVERFLOW) {
		mouse_t.overflow_x = 1;
	}
	else {
		mouse_t.overflow_x = 0;
	}

	//MOUSE_Y_OVERFLOW
	if (mouse_t.packet[0] & MOUSE_Y_OVERFLOW) {
		mouse_t.overflow_y = 1;
	}
	else {
		mouse_t.overflow_y = 0;
	}

	//POS_X
	mouse_t.pos_x = mouse_t.packet[1];

	//POS_Y
	mouse_t.pos_y = mouse_t.packet[2];
}

int mouse_handler()
{
	unsigned long byte;
	int aux;

	aux = sys_inb(OUT_BUF, &byte);

	if (aux != OK)
	{
		printf("Error in sys_inb(OUT_BUF, &byte).\n");
		return 1;
	}

	//First byte of an iteration must have MOUSE_VERIFICATION at 1
	if (mouseCounter == 0 && ((byte & MOUSE_VERIFICATION) == 0) && byte != ACK)
	{
		//printf("\nClearing packets.");
		resetPackets();
		return 1;
	}
	mouse_t.packet[mouseCounter] = byte;
	mouseCounter++;

	//Reset mouseCounter to 0 for next iteration
	if (mouseCounter >= 3)
		mouseCounter = 0;

	//If mouseCounter == 0 we have to set values
	if (mouseCounter == 0) {
		setMousePackets();
		return 0;
	}
	else
		return 1;
}

void resetPackets()
{
	mouse_t.packet[0] = 0;
	mouse_t.packet[1] = 0;
	mouse_t.packet[2] = 0;
}

int isRightButtonPressed()
{
	if (mouse_t.packet[0] & MOUSE_RIGHT_BUTTON)
		return 1;
	else
		return 0;
}

int isLeftButtonPressed()
{
	if (mouse_t.packet[0] & MOUSE_LEFT_BUTTON)
		return 1;
	else
		return 0;
}

int isCorrectMovement() {
	if (mouse_t.overflow_x == 0 && mouse_t.overflow_y == 0) {
		if (mouse_t.sign_x == 0 && mouse_t.pos_x >= 0)
			return 1;
		else if (mouse_t.sign_x == 1 && mouse_t.pos_x <= 0)
			return 1;
		else if (mouse_t.sign_y == 0 && mouse_t.pos_y >= 0)
			return 1;
		else if (mouse_t.sign_y == 1 && mouse_t.pos_y <= 0)
			return 1;
	}
	else
		return 0;
}

int mouse_subscribe_int()
{
	mouse_hook_id = MOUSE_NOTIFICATION;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)
			!= OK)
	{
		printf("Error while subscribing mouse - Set Policy. \n");
		return 1;
	}
	return 0;
}

int mouse_unsubscribe_int()
{
	if (sys_irqdisable(&mouse_hook_id) != OK)
		return 1;

	if (sys_irqrmpolicy(&mouse_hook_id) != OK)
		return 1;
	return 0;
}

int writeInfoThroughKBC(unsigned long kbd_port, unsigned long command)
{
	unsigned long stat;
	unsigned int count = 0;
	while (count < 10)
	{
		if (sys_inb(KBD_IN_BUF, &stat) != OK)
		{
			printf("Error no. 1 - (input buff not ready) in function writeInfoToKBD().\n");
			return 1;
		}
		if ((stat & BIT(1)) == 0)
		{
			if (sys_outb(kbd_port, command) != OK)
			{
				printf("Error n  o. 2 - (Can't read from out buff) in function writeInfoToKBD().\n");
				return 2;
			} else
				return 0;	//Everything works right as intended.
		}
		tickdelay(micros_to_ticks(DELAY_US));
		count++;
	}
}

int readInfoThroughKBC(unsigned long *data)
{
	unsigned long stat;
	unsigned int count = 0;
	while (count < 10)
	{
		if (sys_inb(KBD_IN_BUF, &stat) != OK)
		{
			printf(
					"Error no. 1 (check input buffer) - in function readInfoFromKBD().\n");
			return 1;
		}
		if (stat & BIT(0))
		{
			if (sys_inb(KBD_OUT_BUF, data) != OK)
			{
				printf(
						"Error no. 2 (writing from out buff to data) in function readInfoFromKBD().\n");
				return 2;
			}
			if ((stat & (PAR_ERR | TIMEOUT)) != 0)
			{
				printf(
						"Error no. 3 (invalid data error) - in function readInfoFromKBD().\n");
				return 3;
			} else
				return 0;	//Everything works right as intended.
		}
		tickdelay(micros_to_ticks(DELAY_US));
		count++;
	}

}

int mouseWrite(unsigned long command)
{
	int count = 0;
	unsigned long data;
	int aux;

	while (count < 10)
	{

		aux = writeInfoThroughKBC(KBD_IN_BUF, MOUSE_WRITE_BYTE);
		if (aux != 0)
		{
			printf(
					"Error on 1st writeInfoThroughKBC(KBD_IN_BUF,MOUSE_WRITE_BYTE).\n");
			return 1;
		}

		aux = readInfoThroughKBC(&data);
		if (aux != 0)
		{
			printf("Error on 1st readInfoThroughKBC(data).\n");
			return 1;
		}

		aux = writeInfoThroughKBC(KBD_OUT_BUF, command);
		if (aux != 0)
		{
			printf("Error on 2nd writeInfoThroughKBC(KBD_IN_BUF,command).\n");
			return 1;
		}

		aux = readInfoThroughKBC(&data);
		if (aux != 0)
		{
			printf("Error on 2nd readInfoThroughKBC(data).\n");
			return 1;
		}
		if (data == ACK)
			return 0;
		count++;

	}
}

void printPackets()
{
	//MOUSE_X_SIGN
	if (mouse_t.packet[0] & MOUSE_X_SIGN) {
		printf("X_SIGN=1 ");
		//xSign = 1;
	}
	else {
		printf("X_SIGN=0 ");
		//xSign = 0;
	}

	//MOUSE_Y_SIGN
	if (mouse_t.packet[0] & MOUSE_Y_SIGN) {
		printf("Y_SIGN=1 ");
		//ySign = 1;
	}
	else {
		printf("Y_SIGN=0 ");
		//ySign = 0;
	}

	//MOUSE_X_OVERFLOW
	if (mouse_t.packet[0] & MOUSE_X_OVERFLOW) {
		printf("XOV=1 ");
		//hasOverflowX= 1;
	}
	else {
		printf("XOV=0 ");
		//hasOverflowX = 0;
	}

	//MOUSE_Y_OVERFLOW
	if (mouse_t.packet[0] & MOUSE_Y_OVERFLOW) {
		printf("YOV=1 ");
		//hasOverflowY = 1;
	}
	else {
		printf("YOV=0 ");
		//hasOverflowY = 0;
	}

	printf("X=%d ", (char) mouse_t.packet[1]);
	printf("Y=%d ", (char) mouse_t.packet[2]);
}

