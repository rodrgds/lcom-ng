/**
	@file keyboard.c
	@author Daniel Marques
	@author João Almeida

	Declaration of the core functions used to be able to receive data from the keyboard.
	All imported from our lab3.
*/


#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"

void kbdSubscribe(unsigned int kbd_id) {
	sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id);
	sys_irqenable(&kbd_id);
}

void kbdUnsubscribe(unsigned int kbd_id) {
	sys_irqrmpolicy(&kbd_id);
}

int kbdReadByte(unsigned long int *code, char *two_bytes) {
	sys_inb(OUT_BUF, code); //READ BYTE

	if (*two_bytes) { //IF KEY NEEDS TWO BYTES
		if (*code != TWO_BYTE) { //IF IT'S THE E0 BYTE, IGNORE IT (BOOL IS ALREADY SET)
			if (BIT(7) & *code) { //TRUE = BREAK CODE; FALSE = MAKE CODE
				*two_bytes = 0; //IT'S THE BREAK CODE OF A TWO BYTE KEY, RESET THE TWO BYTE BOOLEAN
				return 1;
			} else
				return 0;
		} else
			return 0;
	} else if (*code == TWO_BYTE) { //IT'S THE E0 BYTE OF THE MAKE CODE
		*two_bytes = 1; //SET THE TWO BYTE BOOLEAN
		return 0;
	} else //IT'S A ONE BYTE KEY
	if (BIT(7) & *code) { //TRUE = BREAK CODE; FALSE = MAKE CODE
		return 1;
	} else
		return 0;
}
