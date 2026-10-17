#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "mouse.h"
#include "macros.h"

int hook_id_mouse = 0x02;

int is_mouse_byte() {
	unsigned long status = 0;

	sys_inb(OUT_BUF, &status);

	if (status & BIT(5))
		return 1;
	else
		return 0;
}

void mouse_command(unsigned long cmd) {
	unsigned long ans;

	sys_outb(CTRL_REG, DR_ENA);

	do {
		sys_outb(CTRL_REG, 0xD4);
		sys_outb(IN_BUF, cmd);
		sys_inb(IN_BUF, &ans);
		tickdelay(micros_to_ticks(DELAY_US));
	} while (ans != ACK);

	return;
}

char mouse_scan() {
	unsigned long status;
	char scan;

	sys_inb(CTRL_REG, &status);
	if (status & BIT(0))
		sys_inb(IN_BUF, &status);

	scan = status;

	return scan;
}

int mouse_subscribe(void) {
	int ret = hook_id_mouse;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&hook_id_mouse) == OK)
		if (sys_irqenable(&hook_id_mouse) == OK)
			return ret;

	return -1;
}

int mouse_unsubscribe(void) {
	if (sys_irqdisable(&hook_id_mouse) == OK)
		if (sys_irqrmpolicy(&hook_id_mouse) == OK)
			return 0;

	return -1;
}
