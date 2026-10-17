#include "keyboard.h"
#include "macros.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

int hook_id_kbd = 0x01;

int kbd_subscribe(void) {
	int ret = hook_id_kbd;

	if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) == OK)
		if (sys_irqenable(&hook_id_kbd) == OK)
			return ret;

	return -1;
}

int kbd_unsubscribe(void) {
	if (sys_irqdisable(&hook_id_kbd) == OK)
		if (sys_irqrmpolicy(&hook_id_kbd) == OK)
			return 0;

	return -1;
}

unsigned long kbd_scan() {
	unsigned long scan = 0, code = 0;
	unsigned long status;

	sys_inb(OUT_BUF, &status);
	if (status & BIT(0))
		sys_inb(IN_BUF, &scan);
	if (scan == SPECIAL) {
		scan = scan << 8;
		scan = scan | code;
	}

	tickdelay(micros_to_ticks(DELAY_US));

	return scan;
}
