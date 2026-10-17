/** @file keyboard.c
 * @brief file containing definitions of the functions that deal with the keyboard
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "keyboard.h"
#include "keyboard_static.h"

#define DEBUG_MALLOC 0

Keyboard *create_keyboard() {
	//allocate space for the "object"
	Keyboard *keyboard = (Keyboard *)malloc(sizeof(Keyboard));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for keyboard\n", sizeof(Keyboard));
	if (keyboard == NULL)
		return NULL;
	keyboard->scan_code = 0;
	keyboard->make_code = 0;
	keyboard->break_code = 0;
	keyboard->last_make = 0;
	keyboard->dual_code = 0;
	return keyboard;
}

static long int kbd_int_processor_c() {
	unsigned long data;
	unsigned long stat;
	/* loop while 8042 output buffer is empty */
	while (1) {
		sys_inb(KBC_STAT_REG, &stat); 										/* assuming it returns OK */
		if (stat & KBC_OBF) {
			sys_inb(KBC_OUT_BUF, &data);									/* assuming it returns OK */
			if ((stat & (KBC_PAR_ERR | KBC_TO_ERR)) == 0)
				return (long) data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_KBC));
	}
}

void keyboard_int_handler(Keyboard *keyboard) {
	keyboard->scan_code = kbd_int_processor_c();
	int msb = keyboard->scan_code >> 7;				/* msb is 1 if breakcode, 0 otherwise */
	if (keyboard->scan_code == DB_BYTE_CODE)
		keyboard->dual_code = 1;						/* first byte denotes double scancode*/
	else {
		keyboard->dual_code = 0;
		if (msb) {											/* it is a breakcode */
			keyboard->last_make = 0;
			keyboard->break_code = keyboard->scan_code;
		}
		else {
			keyboard->last_make = 1;
			keyboard->make_code = keyboard->scan_code;
		}
	}
}
