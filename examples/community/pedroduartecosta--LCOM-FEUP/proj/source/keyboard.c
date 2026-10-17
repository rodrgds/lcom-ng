#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"



int kbd_subscribe() {

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_kbd) == OK)
		if (sys_irqenable(&hook_kbd) == OK)
			return BIT(NOTIFICATION_KBD);
	return -1;
}

int kbd_unsubscribe() {
	if (sys_irqdisable(&hook_kbd) == OK)
		if (sys_irqrmpolicy(&hook_kbd) == OK)
			return 0;
	return 1;
}

int kbd_code_scan() {
	unsigned long cod;
	sys_inb(OUT_BUF, &cod);
	unsigned char codigo = (unsigned char) cod;
	return codigo;
}

unsigned char kbd_code() {
	int r, ipc_status;
	message msg;
	unsigned char codigo;
	char irq_set = BIT(hook_kbd);
	int verifica = 0; //verificar se é uma tecla com info de apenas um byte ou dois

	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
	}
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & irq_set) {
				if (verifica == 0) {
					if (kbd_code_scan(&codigo) == 0) {
						//print_code(codigo);
					} else {
						verifica = 1;
					}
				} else {
					if (kbd_code_scan(&codigo) == 0) {
						//print_code(codigo);
					} else {
						verifica = 0;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return codigo;
}


