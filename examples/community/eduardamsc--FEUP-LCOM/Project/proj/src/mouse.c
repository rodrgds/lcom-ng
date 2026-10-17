#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "graphics.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
int mouse_hook_id = 3;

int mouse_subscribe_int() {
	int mouse_hook_id_tmp = mouse_hook_id;
	if (OK == sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)) {
		if (OK == sys_irqenable(&mouse_hook_id))
			return BIT(mouse_hook_id_tmp);
	}
	return 1;
}

int mouse_unsubscribe_int() {
	if (sys_irqdisable(&mouse_hook_id) != OK) {
		return 1;
	}
	if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
		return 1;
	}
	return 0;
}

int mouse_write(unsigned long cmd) {
	unsigned long stat;
	int i = NTRIES;
	while (i) {
		if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK)
			return 1;
		if (sys_inb(OUT_BUF, &stat) != OK)
			return 1;
		if (sys_outb(IN_BUF, cmd) != OK)
			return 1;
		if (sys_inb(OUT_BUF, &stat) != OK)
			return 1;
		if (stat == ACK) {
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return 1;
}

int mouse_read(unsigned long* data) {
	unsigned long stat = 0;
	int i = NTRIES;

	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;
		if ((stat & OBF) && (stat & AUX)) {
			if (sys_inb(OUT_BUF, data) != OK) {
				return 1;
			}
			if ((stat & (TIMEOUT | PARITY)) == OK)
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;
	}
	return 1;
}

int mouse_reader(ObstacleCourse* course) {
	unsigned long data = 0;
	int temp = mouse.packetIndex;
	mouse_read(&data);

	if (temp == 0) {
		if (data & BIT(3)) {
			mouse.packet[temp] = data;
			temp++;
		}
	} else if (temp == 2) {
		mouse.packet[temp] = data;
		temp = 0;
		mouse.packetReady = 1;
		mouse.packetIndex = temp;
		return 2;
	} else {
		mouse.packet[temp] = data;
		temp++;
	}
	mouse.packetIndex = temp;
	return 0;
}
