#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;
int timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
	uint8_t control = 0x00;

	if (timer_get_conf(timer, &control) != 0) return 1;

	control &= 0x0F;

	switch (timer) {
		case 0: control |= TIMER_SEL0; break;
		case 1: control |= TIMER_SEL1; break;
		case 2: control |= TIMER_SEL2; break;

		default: break;
	}

	control |= TIMER_LSB_MSB;

	if (sys_outb(TIMER_CTRL, control) != 0) return 1;

	uint8_t lsb, msb;
	util_get_LSB(TIMER_FREQ/freq, &lsb);
	util_get_MSB(TIMER_FREQ/freq, &msb);

	if (sys_outb(TIMER_0 + timer, lsb) != 0) return 1;
	if (sys_outb(TIMER_0 + timer, msb) != 0) return 1;

	return 0; 
}

int (timer_subscribe_int)(uint8_t *bit_no) {
	*bit_no = BIT(timer_hook_id);
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1;

	return 0;
}

int (timer_unsubscribe_int)() {
	if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1;

	return 0;
}

void (timer_int_handler)() {
	timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	uint8_t rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL, rb) != 0) return 1;

	if (util_sys_inb(TIMER_0 + timer, st) != 0) return 1;

	return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
	union timer_status_field_val val;

	switch (field) {
		case tsf_all:
			val.byte = st; break;
		case tsf_initial:
			val.in_mode = (st & (BIT(5) | BIT(4))) >> 4; break;
		case tsf_mode:
			val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1; break;
		case tsf_base:
			val.bcd = st & 1;

		default: break;
	}

	if (timer_print_config(timer, field, val) != 0) return 1;

	return 0;
}
