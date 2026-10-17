#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "ModTeclado.h"






unsigned long data;
int value;

int kbd_subscribe_int(void) { //
	value = KBD_HOOK_ID;
	sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(KBD_HOOK_ID);
}

int kbd_unsubscribe_int() {

	return (sys_irqrmpolicy(&value)||sys_irqdisable(&value));
}

void kbd_int_handler(){ //function that handles the interrupts in C
	sys_inb(IN_BUF,&scan_code);
}
