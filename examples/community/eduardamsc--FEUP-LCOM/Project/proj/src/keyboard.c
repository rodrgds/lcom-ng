#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"

int keyboard_hook_id=1;

int keyboard_subscribe_int() {
	int keyboard_hook_id_tmp=keyboard_hook_id;
	if (OK==sys_irqsetpolicy(KBD_IRQ,  IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id)) {
		if (OK==sys_irqenable(&keyboard_hook_id))
			return BIT(keyboard_hook_id_tmp);
	}
	printf("keyboard_subscribe failed\n");
	return 1;
}

int keyboard_unsubscribe_int() {
	int keyboard_hook_id_tmp=keyboard_hook_id;
	if (OK==sys_irqdisable(&keyboard_hook_id)) {
		if (OK == sys_irqrmpolicy(&keyboard_hook_id_tmp))
			return BIT(keyboard_hook_id_tmp);
	}
	printf("keyboard_unsubscribe failed\n");
	return 1;
}

unsigned long keyboard_read() {
	unsigned long stat;
	unsigned long key;
	int i = NTRIES;
	while (i) {
		if (sys_inb(STAT_REG, &stat) != OK) {
			printf("Sys in failed!\n");
			return 1;
		}
		if (stat & OBF) {
			if (sys_inb(OUT_BUF, &key) != OK) {
				printf("Sys in failed!\n");
				return 1;
			}

			if ((stat & (TIMEOUT | PARITY)) == 0) {

				return key;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i--;

	}
	return 1;
}
