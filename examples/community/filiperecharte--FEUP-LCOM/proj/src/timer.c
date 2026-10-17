#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = TIMER0_IRQ;
int timer_counter = 0;


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
	uint8_t current_configuration;

	if(timer_get_conf(timer, &current_configuration) != 0)
		return 1;


	uint8_t new_configuration = (current_configuration & 0x0F) | TIMER_LSB_MSB; // 4 LSB are preserved, bits 4 and 5 are set to 1


	if (freq > TIMER_FREQ)
		return 1;

	uint16_t divisor = TIMER_FREQ / freq; // We achieve freq by then calculating freq = TIMER_FREQ / divisor

	uint8_t lsb, msb;
	
	util_get_LSB(divisor, &lsb);
	util_get_MSB(divisor, &msb);

	switch (timer)
	{
	case 0:
		new_configuration = new_configuration | TIMER_SEL0;
		if(sys_outb(TIMER_CTRL, new_configuration) != 0)
			return 1;
			
		if(sys_outb(TIMER_0, lsb) != 0)
			return 1;
			
		if(sys_outb(TIMER_0, msb) != 0)
			return 1;
			
		break;
	
	case 1:
		new_configuration = new_configuration | TIMER_SEL1;
		if (sys_outb(TIMER_CTRL, new_configuration) != 0)
			return 1;
			
		if(sys_outb(TIMER_1, lsb) != 0)
			return 1;
			
		if(sys_outb(TIMER_1, msb) != 0)
			return 1;
			
		break;
	
	case 2:
		new_configuration = new_configuration | TIMER_SEL2;	
		if(sys_outb(TIMER_CTRL, new_configuration) != 0)
			return 1;
			
		if(sys_outb(TIMER_2, lsb) != 0)
			return 1;
			
		if(sys_outb(TIMER_2, msb) != 0)
			return 1;
			
		break;
	
	default:
		return 1;
	}

	return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
	*bit_no = BIT(timer_hook_id);

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)!= 0)
		return 1;

	return 0;
}

int (timer_unsubscribe_int)() {
	if (sys_irqrmpolicy(&timer_hook_id)!= 0)
		return 1;

	return 0;
}

void (timer_int_handler)() {
	timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	uint8_t cntr_word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | (1 << (timer + 1)));

	if(sys_outb(TIMER_CTRL, cntr_word) != 0)
		return 1;

	if (util_sys_inb(TIMER_0 + timer, st) != 0)
		return 1;
	
	return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
	union timer_status_field_val val;

	if (field == tsf_all) {
		val.byte = st;
	}
	else if (field == tsf_initial) {
		enum timer_init init_value;

		if ((st & (BIT(5) | BIT(4))) == TIMER_LSB) {
			init_value = LSB_only;
		}
		else if ((st & (BIT(5) | BIT(4))) == TIMER_MSB) {
			init_value = MSB_only;
		}
		else if ((st & (BIT(5) | BIT(4))) == TIMER_LSB_MSB) {
			init_value = MSB_after_LSB;
		}
		else
			init_value = INVAL_val;

		val.in_mode = init_value;
	}

	else if(field == tsf_mode) {
		uint8_t count_mode = (st & (BIT(1) | BIT(2) | BIT(3)));
		count_mode = count_mode >> 1;
		val.count_mode = count_mode;
	}

	else if(field == tsf_base) {
		bool bcd = true;
		if ((st & BIT(0)) == 0)
			bcd = false;

		val.bcd = bcd;
	}

	timer_print_config(timer, field, val);


	return 0;
}
