#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"

static int hook_id;

int kbd_subscribe() {
	// atualiza hook_id, passando a ser 1
	hook_id = IRQ_KBD;

	int bitmask = BIT(hook_id);

	/*The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE | IRQ_REENABLE*/
	if (sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK
			|| sys_irqenable(&hook_id) != OK) {
		printf("\nkbd_subscribe() failed \n");
		return -1;
	}

	return bitmask;

}

int kbd_unsubscribe() {
	/*sys_irqrmpolicy(int *hook_id) Unsubscribes a previous interrupt notification.

	 sys_irqdisable(int *hook_id) Masks an interrupt line associated
	 with a previously subscribed interrupt notification.*/

	if (sys_irqdisable(&hook_id) != 0) {
		printf("\nkbd_unsubscribe failed \n");
		return 1;
	}

	if (sys_irqrmpolicy(&hook_id) != 0) {
		printf("\nkbd_unsubscribe failed \n");
		return 1;
	}

	return 0;
}

