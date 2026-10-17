
#include "keyboard.h"
#include "general.h"
int hook_id_kbc= 1;

int subscribe_kbc() {

	int irqf = 1;

	int irq_set = BIT(hook_id_kbc);
	if (sys_irqsetpolicy(irqf, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbc) != OK
			|| sys_irqenable(&hook_id_kbc) != OK)
		return -1;
	else
		return irq_set;
}

int unsubscribe_kbc() {
	if (sys_irqdisable(&hook_id_kbc) != OK) {
		printf("IRQDISABLE falhou /n");
		return -1;
	}
	if (sys_irqrmpolicy(&hook_id_kbc) != OK) {
		printf("IRQRMPOLICY falhou /n");
		return -1;
	}
	return 0;
}

