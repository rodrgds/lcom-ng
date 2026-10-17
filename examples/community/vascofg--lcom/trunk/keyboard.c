#include <minix/syslib.h>
#include <minix/drivers.h>

#include "keyboard.h"
#include "kbc.h"

unsigned char scancode;

static int hook;
static int t_hook;

unsigned long kbc_subscribe_int(void) {
	int* bitmask;
	int r;
	bitmask = (int*) malloc(sizeof(int));
	*bitmask = 2;
	r = sys_irqsetpolicy(KB_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), bitmask);
	hook = *bitmask;
	if (r == OK && sys_irqenable(bitmask) == OK) {
		return 4;
	} else
		return 0;
}

int kbc_unsubscribe_int() {
	if (sys_irqdisable(&hook) == OK && sys_irqrmpolicy(&hook) == OK)
		return 0;
	else
		return 1;
}

unsigned long timer_subscribe_int(void) {
	int* timer_bitmask;
	timer_bitmask = (int*) malloc(sizeof(int));
	*timer_bitmask = 2;
	t_hook = *timer_bitmask;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, timer_bitmask) == OK
			&& sys_irqenable(timer_bitmask) == OK) {
		return 4;
	} else
		return 0;
}

int timer_unsubscribe_int() {
	if (sys_irqrmpolicy(&t_hook) == OK && sys_irqdisable(&t_hook) == OK)
		return 0;
	else
		return 1;
}

int readkey(void) {
	int ipc_status;
	message msg;
	int i = 1;
	int irq_set = kbc_subscribe_int();

	while (1) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			return -1;
		}
		if (is_ipc_notify(ipc_status)) {
			/*got notification*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed*/
					scancode = read_kbc();
					if (scancode >> 7 == 0)
						return scancode;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	kbc_unsubscribe_int();
}

int test_scan(void) {
	int ipc_status;
	message msg;
	int i = 1;
	int irq_set = kbc_subscribe_int();

	while (i) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/*got notification*/
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed*/
					scancode = read_kbc();
					if (scancode == 1)
						i = 0;
					if (scancode >> 7 == 1)
						printf("Break: 0x%2x", scancode);
					else
						printf("Make: 0x%2x", scancode);
					printf("\n");

				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	kbc_unsubscribe_int();
}

int test_leds(unsigned short n, unsigned short *leds) {

	int i, s;
	unsigned long data, bitmask, t_bitmask;
	int ipc_status;
	message msg;
	int irq_set = timer_subscribe_int();
	int led0, led1, led2;
	led0 = 0;
	led1 = 0;
	led2 = 0;
	bitmask = 0;
	for (i = 0; i < n; i++) {
		write_kbc(DATA_REG, LEDS_SWITCH);
		sys_inb(DATA_REG, &data);
		if (data == 0xfe) {
			i--;
			printf("\nERRO NO ULTIMO BYTE ESCRITO");
		} else if (data == 0xfc) {
			printf("\nERRO NA SEQUENCIA COMPLETA");
			i = 0;
		} else {
			if (leds[i] == 0) {
				if (led0 == 0)
					led0 = BIT(0);
				else
					led0 = 0;
			}

			if (leds[i] == 1) {
				if (led1 == 0)
					led1 = BIT(1);
				else
					led1 = 0;
			}

			if (leds[i] == 2) {
				if (led2 == 0)
					led2 = BIT(2);
				else
					led2 = 0;
			}

			bitmask = led0 | led1 | led2;

			write_kbc(DATA_REG, bitmask);
			sys_inb(DATA_REG, &data);
			if (data == 0xfe)
				i--;
			else if (data == 0xfc)
				i = 0;
			else {
				switch (leds[i]) {
				case 0:
					if (led0 == 0)
						printf("\nTurn off Scroll Lock");
					else
						printf("\nTurn on Scroll Lock");
					break;
				case 1:
					if (led1 == 0)
						printf("\nTurn off Num Lock");
					else
						printf("\nTurn on Num Lock");
					break;
				case 2:
					if (led2 == 0)
						printf("\nTurn off Caps Lock");
					else
						printf("\nTurn on Caps Lock");
					break;
				}
				s = 0;
				while (1) {
					if (driver_receive(ANY, &msg, &ipc_status) != 0) {
						printf("driver_receive failed");
						continue;
					}
					if (is_ipc_notify(ipc_status)) {
						/*got notification*/
						switch (_ENDPOINT_P(msg.m_source)) {
						case HARDWARE:
							if (msg.NOTIFY_ARG & irq_set) { /* subscribed*/
								s++;
							}
							break;
						default:
							break;
						}
					} else {
					}
					if (s >= 10)
						break;
				}
				timer_unsubscribe_int();
			}
		}
	}
}

int crash_leds(unsigned short n, unsigned short *leds) {

	int i, s, f;
	unsigned long data, bitmask, t_bitmask;
	int ipc_status;
	message msg;
	int irq_set = timer_subscribe_int();
	int led0, led1, led2;
	led0 = 0;
	led1 = 0;
	led2 = 0;
	bitmask = 0;
	i = 0;
	f = 0;
	while (1) {
		write_kbc(DATA_REG, LEDS_SWITCH);
		sys_inb(DATA_REG, &data);
		if (data == 0xfe) {
			i--;
			printf("\nERRO NO ULTIMO BYTE ESCRITO");
		} else if (data == 0xfc) {
			printf("\nERRO NA SEQUENCIA COMPLETA");
			i = 0;
		} else {
			if (leds[i] == 0) {
				if (led0 == 0)
					led0 = BIT(0);
				else
					led0 = 0;
			}

			if (leds[i] == 1) {
				if (led1 == 0)
					led1 = BIT(1);
				else
					led1 = 0;
			}

			if (leds[i] == 2) {
				if (led2 == 0)
					led2 = BIT(2);
				else
					led2 = 0;
			}

			bitmask = led0 | led1 | led2;

			write_kbc(DATA_REG, bitmask);
			sys_inb(DATA_REG, &data);
			if (data == 0xfe)
				i--;
			else if (data == 0xfc)
				i = 0;
			else {
				s = 0;
				while (1) {
					if (driver_receive(ANY, &msg, &ipc_status) != 0) {
						printf("driver_receive failed");
						continue;
					}
					if (is_ipc_notify(ipc_status)) {
						/*got notification*/
						switch (_ENDPOINT_P(msg.m_source)) {
						case HARDWARE:
							if (msg.NOTIFY_ARG & irq_set) { /* subscribed*/
								s++;
							}
							break;
						default:
							break;
						}
					} else {
					}
					if (s >= TIMER_DELAY)
						break;
				}
				timer_unsubscribe_int();
			}
		}
		i++;
		f++;
		if (i == n)
			i = 0;
		if (f == REPEAT)
			break;
	}
}
