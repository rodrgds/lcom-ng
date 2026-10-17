#include "test3.h"

unsigned int hook_id_kbd = 0;
unsigned int hook_id_timer = 2;
unsigned int time;

unsigned long asm_handler(unsigned long tecla);

int kbd_subscribe_int(void) {
	if ((sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd))
			< 0)
		return -1;
	else
		return KBD_IRQ;
}

int kbd_unsubscribe_int(void) {
	int r = sys_irqrmpolicy(&hook_id_kbd);
	if (r != 0)
		return -1;
	else
		return r;
}

int kbd_int_handler() {
	unsigned long stat;
	unsigned long data;
	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		// loop while 8042 output buffer is empty
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

}

int kbd_asm_handler() {
	unsigned long letra;
	letra = asm_handler(letra);

	if (letra == BC_ESC)
		return -1;
	return 0;
}

int kbd_test_scan(unsigned short ass) {
	int r;
	int ipc_status = 0;
	message msg;
	int irq_set = ~hook_id_kbd;
	if (kbd_subscribe_int() == -1)
		return 1;
	if (ass == 0) {
		while (1) { // You may want to use a different condition

			//Get a request message.
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { // received notification
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification

					if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt

						unsigned long data = kbd_int_handler();
						if (data == BC_ESC) {
							printf("Breakcode: 0x%d\n", data);
							return 0;
						} else {
							if ((data & BREAK_CODE) == 0) {
								if (data == TWO_BYTE)
									printf("Makecode: 0xE0%d\n", data);
								else
									printf("Makecode: 0x%d\n", data);
							} else {
								if (data == TWO_BYTE)
									printf("Breakcode: 0xE0%d\n", data);
								else
									printf("Breakcode: 0x%d\n", data);
							}
						}
					}
					break;
				default:
					break; //no other notifications expected: do nothing
				}
			} else { // received a standard message, not a notification //
					 //no standard messages expected: do nothing
			}
		}
	} else {
		while (1) { // You may want to use a different condition

			//Get a request message.
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { // received notification
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification

					if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt

						if (kbd_asm_handler() == -1)
							break;
					}
					break;
				default:
					break; //no other notifications expected: do nothing
				}
			} else { // received a standard message, not a notification //
				//no standard messages expected: do nothing
			}
		}
	}
	printf("ESC was pressed\n");
	if (kbd_unsubscribe_int() < 0)
		return -1;
	else
		return kbd_unsubscribe_int();

}

int kbd_handler_leds(unsigned short led) {
	unsigned long stat;
	while (1) {
		sys_outb(IN_BUF, LED_CMD); /* assuming it returns OK */
		if ((stat & IBF) == 0) { //if the input isn't full
			if (led == 0) {
				sys_outb(IN_BUF, led); //puts in the in buffer the led we wan't to turn on
			}
			if (led == 1) {
				unsigned short l = led << 1;
				sys_outb(IN_BUF, l);
			}
			if (led == 2) {
				unsigned short l = led << 2;
				sys_outb(IN_BUF, led);
			}
		}
		sys_inb(OUT_BUF, &stat); //to read the stat of the out buffer
		if ((stat & OBF) == 0) //if the buffer isn't full yet
			tickdelay(micros_to_ticks(DELAY_US)); //we wait
		int end = 0;
		while (end == 0) {
			if ((stat & ACK) != 0) {	//if everything went ok
				end = -1;
			} else if ((stat & RESEND) != 0) { //if we have to resend the data
				end = 1;
			} else if ((stat & ERROR) != 0) { //if there has been an error
				end = 1;
			}
		}
		if (end == -1)	//to end the kbd_handler_leds
			return 0;
	}
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	int r;
	int ipc_status = 0;
	message msg;
	int irq_set = ~hook_id_kbd;
	if (kbd_subscribe_int() == -1)
		return 1;
	int end = 1;
	while (end == 1) { // You may want to use a different condition

		//Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt
					int i = 0;
					while (i < n) { //n is the number of elements of the array of leds
						int K = kbd_handler_leds(leds[i]);
						if (K == 0) {
							if (leds[i] == 0) {
								printf("Scroll Lock LED\n");
							} else if (leds[i] == 1) {
								printf("Numeric Lock LED\n");
							} else if (leds[i] == 2) {
								printf("Caps Lock LED\n");
							}
							if (wait_time(1) < 0) //waits one second, using the timer
								return -1;
							i++;
						}
					}
					if (i == n) {
						printf("The sequence has ended.\n");
						return 0;
					}
					end = 0;
				}
				break;
			default:
				break; //no other notifications expected: do nothing
			}
		}
	}
	if (kbd_unsubscribe_int() < 0)
		return -1;
	else
		return kbd_unsubscribe_int();
}

int timer() {

}

int kbd_test_timed_scan(unsigned short n) {
	int r;
	int counter = 0;
	int ipc_status = 0;
	message msg;
	int irq_set_kbd = ~hook_id_kbd;
	int irq_set_timer = ~hook_id_timer;
	if (kbd_subscribe_int() == -1)
		return 1;
	if (timer_subscribe_int() == -1)
		return 1;
	while (counter < n * 60) {

		//Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set_kbd) { // subscribed interrupt
					unsigned long data = kbd_int_handler();
					if (data == BC_ESC) {
						printf("Breakcode: 0x%d\n", data);
						return 0;
					} else {
						if ((data & BREAK_CODE) == 0) {
							if (data == TWO_BYTE) {
								printf("Makecode: 0xE0%02X\n", data);
								counter = 0;
							} else {
								printf("Makecode: 0x%02X\n", data);
								counter = 0;
							}
						} else {
							if (data == TWO_BYTE) {
								printf("Breakcode: 0xE0%02X\n", data);
								counter = 0;
							} else {
								printf("Breakcode: 0x%02X\n", data);
								counter = 0;
							}
						}
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					counter++;

				}
				break;
			default:
				break; //no other notifications expected: do nothing
			}

		}
	}
	printf("Time has run out!\n");

	if (kbd_unsubscribe_int() < 0)
		return -1;
	else
		return kbd_unsubscribe_int();
}

