#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "keyboard.h"

//KBD's hook id
int kbd_hook_id = KBD_IRQ;

//Last code from KBD
unsigned long lastCode = 0;

//Is 1 if last code was 0xE0
int doubleCode = 0;

int keyboard_subscribe_int() {

	int kbd_id = kbd_hook_id;
	int valid = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);

	if(valid != OK)
		return -1;
	else
		return kbd_id;
}

int keyboard_unsubscribe_int() {

	int valid = sys_irqrmpolicy(&kbd_hook_id);

	if(valid != OK)
		return 1;
	else
		return 0;
}

int keyboard_int_handler() {

	//Register status
	unsigned long status;

	int nrIter = 0;

	while(nrIter != 10)
	{
		//Get status from register
		if(getStatus(&status))
			return 1;

		//Check if OBF is 1
		if(status & STATUS_OBF)
			break;

		nrIter++;

		if(nrIter == 10)
			return 1;
	}

	//Check for parity and timeout error
	if(status & PAR_ERR || status & TO_ERR )
		return 1;

	//Get code from KBC's buffer
	unsigned long response;
	if(getResponse(&response))
		return 1;

	//Does the code have two bytes?
	if(lastCode == TWOBYTE)
		doubleCode = 1;

	//Save code
	lastCode = response;

	return 0;
}

void printCode(unsigned long code) {

	lastCode = 0;

	if(doubleCode)
	{
		//Reset the variable
		doubleCode = 0;

		if(code == 0x2A || code == 0xAA)
			return;

		//Print either the make or break code
		if(code & BREAK)
			printf("Break code: 0xE0 0x%X \n", code);
		else
			printf("Make code: 0xE0 0x%X \n", code);
	}
	else
	{
		//Print either the make or break code
		if(code & BREAK)
			printf("Break code: 0x%X \n", code);
		else
			printf("Make code: 0x%X \n", code);
	}

}

void printLetter(unsigned long code) {

	lastCode = 0;

	if (doubleCode) {
		//Reset the variable
		doubleCode = 0;

		if (code == 0x2A || code == 0xAA)
			return;

		if (!(code & BREAK))
		{
			printf("Make code: 0xE0 0x%X \n", code);

			if (code == Q)
				printf("Q\n");
			else if (code == W)
				printf("W\n");
			else if (code == E)
				printf("E\n");
			else if (code == R)
				printf("R\n");
			else if (code == X)
				printf("X\n");
			else if (code == Z)
				printf("<\n");
			else
				printf("Not Found\n");
		}
	}
	else
	{
		if (!(code & BREAK))
		{
			printf("Make code: 0x%X \n", code);

			if (code == Q)
				printf("Q\n");
			else if (code == W)
				printf("W\n");
			else if (code == E)
				printf("E\n");
			else if (code == R)
				printf("R\n");
			else if (code == X)
				printf("X\n");
			else if (code == Z)
				printf("<\n");
			else
				printf("Not Found\n");
		}
	}
}

unsigned long getLastCode() {
	return lastCode;
}

int getDoubleCode() {
	return doubleCode;
}
