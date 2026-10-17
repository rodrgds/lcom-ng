#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"
#include <stdio.h>
#include <time.h>


int timer_set_square(unsigned long timer, unsigned long freq) {

	char control_word = 0;
	char current_word = 0;
	port_t controlPort = TIMER_CTRL;
	port_t dataPort;

	// Read Current control word for target timer
	timer_get_config(timer, &current_word);

	//Select the timer
	switch (timer) {
	case 0:
		control_word |= TIMER_SEL0;
		dataPort = TIMER_0;
		break;
	case 1:
		control_word |= TIMER_SEL1;
		dataPort = TIMER_1;
		break;
	case 2:
		control_word |= TIMER_SEL2;
		dataPort = TIMER_2;
		break;
	default:
		return 1; // Invalid Timer
	}

	// Initialization Method
	control_word |= TIMER_LSB_MSB;

	// Operating Mode
	control_word |= TIMER_SQR_WAVE;

	// Counting Mode
	control_word |= (current_word & 0x01);

	// Set Control word
	sys_outb(controlPort, control_word);

	// Write LSB and MSB

	int val = TIMER_FREQ / freq;
	if (val < 0 || val >= 1 << 16)
		return 2; // Invalid Freq

	sys_outb(dataPort, (val & 0x00FF));
	sys_outb(dataPort, (val & 0xFF00) >> 8);

	return 0;
}

int timer_subscribe_int() {

	TIMER_HOOK_ID = TIMER_INT_BIT;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &TIMER_HOOK_ID) != OK
			|| sys_irqenable(&TIMER_HOOK_ID) != OK)
		return -1;

	return 1 << TIMER_INT_BIT; //Returns bit order in interrupt mask
}

int timer_unsubscribe_int() {
	if (sys_irqrmpolicy(&TIMER_HOOK_ID) == OK)
		return 0;

	return 1;
}


int timer_get_config(unsigned long timer, unsigned char *st) {

	char RB_control = 0;
	port_t outPort = TIMER_CTRL;
	port_t inPort;
	// Select Timer
	switch (timer) {
	case 0:
		RB_control |= TIMER_RB_SEL(0);
		inPort = TIMER_0; break;
	case 1:
		RB_control |= TIMER_RB_SEL(1);
		inPort = TIMER_1; break;
	case 2:
		RB_control |= TIMER_RB_SEL(2);
		inPort = TIMER_2; break;
	default:
		return 1; // Invalid Timer

	}

	// Read Back Command
	RB_control |= TIMER_RB_CMD;

	// Request Status Byte
	RB_control |= TIMER_RB_COUNT_; // Count bit to 1 means we don't want the count bytes
	RB_control &= ~TIMER_RB_STATUS_; // Status bit to 0 means we want the status byte

	// Reserved bit
	RB_control &= ~0x01;

	// Request Status Byte
	if (sys_outb(outPort, RB_control) != OK) return 2; //Error requesting info

	//Read Result
	unsigned long res;
	if (sys_inb(inPort, &res)!=OK) return 3; //Error retrieving info
	*st = (unsigned char) res;

	return 0;
}

int delay_milis(unsigned long milis) {
	usleep(milis*1000);
	return 0;
}



