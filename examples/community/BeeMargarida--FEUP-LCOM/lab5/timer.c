#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>
#include "i8254.h"

int hook_id_t0;
unsigned int cont_int = 0;

int timer_set_square(unsigned long timer, unsigned long freq) {

	unsigned long *control; //used to preserve the 4 lsb of the ctrl register word
	control = malloc(sizeof(unsigned long));
	sys_inb(TIMER_CTRL, control); //sets control with the timer ctrl value

	//preserves the 4 lsb and sets the rest to 0
	unsigned char x = BIT(0) | BIT(1) | BIT(2) | BIT(3);
	*control &= x;

	//type of access lsb followed by msb
	*control &= TIMER_LSB_MSB;

	unsigned long msb, lsb;

	//chooses timer and sets it's counter value
	if (timer == 0) {
		*control &= TIMER_SEL0;
		sys_outb(TIMER_CTRL, *control);
		lsb = TIMER_FREQ / freq;
		msb = (TIMER_FREQ / freq) >> 8;
		sys_outb(TIMER_0, lsb);
		sys_outb(TIMER_0, msb);
	} else if (timer == 1) {
		*control &= TIMER_SEL1;
		sys_outb(TIMER_CTRL, *control);
		lsb = TIMER_FREQ / freq;
		msb = (TIMER_FREQ / freq) >> 8;
		sys_outb(TIMER_1, lsb);
		sys_outb(TIMER_1, msb);
	} else if (timer == 2) {
		*control &= TIMER_SEL2;
		sys_outb(TIMER_CTRL, *control);
		lsb = TIMER_FREQ / freq;
		msb = (TIMER_FREQ / freq) >> 8;
		sys_outb(TIMER_0, lsb);
		sys_outb(TIMER_0, msb);
	}

	else {
		printf("Invalid timer\n");
		return 1;
	}

	return 0;

}

int timer_subscribe_int(void) {
	hook_id_t0 = 0;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_t0) < 0)
		return -1;
	sys_irqenable(&hook_id_t0);
	return 0;
}

int timer_unsubscribe_int() {
	sys_irqdisable(&hook_id_t0);
	return (sys_irqrmpolicy(&hook_id_t0));
}

void timer_int_handler() {
	cont_int++;

}

int timer_get_conf(unsigned long timer, unsigned char *st) { //fazer esta
	unsigned char l = 0x00;
	if (timer == 0)
		l |= TIMER_RB_SEL(0);
	else if (timer == 1)
		l |= TIMER_RB_SEL(1);
	else if (timer == 2)
		l |= TIMER_RB_SEL(2);

	l |= TIMER_RB_CMD;
	sys_outb(TIMER_CTRL, l);

	unsigned long x;
	if (timer == 0)
		sys_inb(TIMER_0, &x);
	else if (timer == 1)
		sys_inb(TIMER_1, &x);
	else if (timer == 2)
		sys_inb(TIMER_2, &x);

	*st = (unsigned char) x;

	return 1;
}

int timer_display_conf(unsigned char conf) { //fazer esta
	unsigned char mask = BIT(7);

	if ((mask & conf) == 0)
		printf("Current value of timer's OUTPUT is 0\n");
	else
		printf("Current value of timer's OUTPUT is 1\n");

	mask = BIT(6);

	if ((mask & conf) == 0)
		printf("Current value of timer's NULL COUNT is 0\n");
	else
		printf("Current value of timer's NULL COUNT is 1\n");

	mask = BIT(5);
	if ((mask & conf) == 0)
		printf("Type of access: LSB\n");
	else {
		mask = BIT(4);
		if ((mask & conf) == 0)
			printf("Type of access: MSB\n");
		else
			printf("Type of access: LSB followed by MSB\n");
	}

	mask = BIT(3);
	unsigned char m2 = BIT(2);
	unsigned char m3 = BIT(1);
	if ((mask & conf) == 0) {
		if ((m2 & conf) == 0) {
			if ((m3 & conf) == 0)
				printf("Operation mode: 0\n");
			else
				printf("Operation mode: 1\n");
		} else {
			if ((m3 & conf) == 0)
				printf("Operation mode: 2\n");
			else
				printf("Operation mode: 3\n");
		}
	}
	if ((mask & conf) == 1) {
		if ((m2 & conf) == 0) {
			if ((m3 & conf) == 0)
				printf("Operation Mode: 4\n");
			else
				printf("Operation Mode: 5\n");
		} else {
			if ((m3 & conf) == 0)
				printf("Operation mode: 2\n");
			else
				printf("Operation mode: 3\n");
		}
	}

	mask = TIMER_BCD;
	if ((mask & conf) == 0)
		printf("Counting mode: BCD\n");
	else
		printf("Counting mode: Binary\n");

	return 0;
}

int timer_test_square(unsigned long freq) { //fazer esta
	if (freq <= 0) {
		printf("Invalid frequency\n");
		return 1;
	}
	//same as timer_set_square(0, freq);
	unsigned long timer = 0;

	timer_set_square(timer, freq);

	return 0;

}

int timer_test_int(unsigned long time) {

	int ipc_status = 0;
	message msg;
	int r;
	int irq_set = ~hook_id_t0;
	while (1) { // You may want to use a different condition

		timer_subscribe_int();
		//Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt

					timer_int_handler();
					if ((cont_int % 60) == 0) {
						printf("One second\n");
						time--;
					}
					if (time == 0) {
						timer_unsubscribe_int();
						printf("int: %u", cont_int);
						return 0;
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

	return 1;
}

int timer_test_config(unsigned long timer) {        //fazer esta
	unsigned char *st;
	st = malloc(sizeof(unsigned char));
	unsigned char conf;
	timer_get_conf(timer, st);
	timer_display_conf(conf);
	return 0;
}

int wait_time(unsigned long seconds) {
	int ipc_status = 0;
	message msg;
	int r;
	int irq_set = ~hook_id_t0;
	if (timer_subscribe_int() < 0)
		return -1;
	int end = 1;
	while (end == 1) { // You may want to use a different condition
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification

				if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt

					timer_int_handler();
					if ((cont_int % 60) == 0) {
						seconds--;
					}
					if (seconds == 0) {
						end = 0;
					}
				}
				break;
			default:
				break; //no other notifications expected: do nothing
			}
		}
	}
	timer_unsubscribe_int();
	return 0;
}



