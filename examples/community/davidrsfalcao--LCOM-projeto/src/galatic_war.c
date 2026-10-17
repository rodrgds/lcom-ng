#include "game.h"
#include "menu.h"

char irq_set;

int main() {
	sef_startup();

	if (vg_init(0x105) == NULL)
		{
			vg_exit();
			printf("Graphic mode failed!\n");
			return 1;
		}

	irq_set = BIT(khook_id);

	if (kbd_subscribe_int() == -1) {
		printf("Keyboard not subscribed!\n");
		return 1;
	}

	int opcao1 = 0;
	int opcao2 = 0;

	int lixo;
	int r;
	int ipc_status;
	message msg;

	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);

	}
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:

			if (msg.NOTIFY_ARG & irq_set) {
				kbd_scan_c(&lixo);
			}
		}
	}

	while(opcao1 != 1)
	{
		opcao1 = menu();

		if (opcao1 != 1) {
			while (opcao2 != 1) {
				opcao2 = game();

			}
			break;
		}

	}

	vg_exit();

	timer_unsubscribe_int();
	kbd_unsubscribe_int();


	return 0;
}
