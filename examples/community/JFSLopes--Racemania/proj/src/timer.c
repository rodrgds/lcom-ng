#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  	if (timer > 2){
    	printf("Invalid timer, must be between 0 and 2.\n");
    	return -1;
  	}

  	if (st == NULL){
    	printf("Null pointer.\n");
    	return -1;
  	}

  	/// No need for reading the count
  	uint32_t write = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  	int ret = sys_outb(TIMER_CTRL, write);

  	if (ret != 0){
    	printf("Error on sys_outb().\n");
    	return -1;
  	}
  
  	// Calculate address based on timer number
  	uint8_t timer_address = TIMER_PORT(timer);

  	// Read timer configuration
  	ret = util_sys_inb(timer_address, st);
  	if (ret != 0){
    	printf("Error on util_sys_inb() call.\n");
    	return -1;
  	}
  	return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  	if (timer > 2){
    	printf("Invalid timer, must be between 0 and 2.\n");
    	return -1;
  	}

  	union timer_status_field_val config;

	switch(field){
		case tsf_all:
			config.byte = st;
			break;

		case tsf_initial:{
			uint8_t aux = (st & TIMER_GET_LSB_MSB) >> 4;	/// Get bit 4 and 5 and then move them to the right
			if (aux == 0) config.in_mode = INVAL_val;
			else if (aux == 1) config.in_mode = LSB_only;
			else if (aux == 2) config.in_mode = MSB_only;
			else config.in_mode = MSB_after_LSB;
			break;
		}

		case tsf_mode:
			config.count_mode = (st & TIMER_GET_MODE) >> 1;
			config.count_mode = ENSURE_MSB_ZERO(config.count_mode); /// In both mode 2 and 3, makes sure the msb is 0
			break;

		case tsf_base:
			config.bcd = (st & TIMER_GET_BASE);
			break;

		default:
			printf("field had an invalid value.\n");
			return -1;

	}
	
	int ret = timer_print_config(timer, field, config);
	if (ret != 0){
		printf("Error in timer_display_conf().\n");
		return -1;
	}
	return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq){
	if (timer > 2){
		printf("Invalid timer, must be between 0 and 2.\n");
    	return -1;
	}

	if (freq < TIMER_COUNT_MIN || freq > TIMER_FREQ){
		printf("Invalid frequency value.\n");
		return -1;
	}
	
	// Prepare the control byte mask
	uint8_t rb_command;
	int ret = timer_get_conf(timer, &rb_command);
	if (ret != 0){
		printf("Error preparing the control byte.\ntimer_get_conf() failed.\n");
		return -1;
	}
	uint32_t cnt_word;
	switch (timer){
		case 0:
			cnt_word = TIMER_SEL0;
			break;
		case 1:
			cnt_word = TIMER_SEL1;
			break;
		case 2:
			cnt_word = TIMER_SEL2;
			break;
	}
	cnt_word = cnt_word | TIMER_LSB_MSB; // MSB after LSB
	uint8_t keep_conf = rb_command & (TIMER_GET_BASE | TIMER_GET_MODE); // Keep last 4 bits
	cnt_word = cnt_word | keep_conf;

	// Write the control word
	ret = sys_outb(TIMER_CTRL, cnt_word);
	if (ret != 0){
		printf("Error writting control word to control register.\nsys_outb() failed.\n");
		return -1;
	}
	// Calculate address based on timer number
  	uint8_t timer_address = TIMER_PORT(timer);

	// value must be casted
	uint16_t value = (uint16_t) (TIMER_FREQ / freq);
	uint8_t byte;
 	// Write lsb
	ret = util_get_LSB(value, &byte);
	if (ret != 0){
		printf("Erro getting the lsb.\n");
		return -1;
	}
	ret = sys_outb(timer_address, byte);
	if (ret != 0){
		printf("Erro writting the lsb.\n");
		return -1;
	}

	// Write msb 
	ret = util_get_MSB(value, &byte);
	if (ret != 0){
		printf("Erro getting the lsb.\n");
		return -1;
	}
	ret = sys_outb(timer_address, byte);
	if (ret != 0){
		printf("Erro writting the msb.\n");
		return -1;
	}
	return 0;
}

uint32_t counter = 0;
int timer_hook_id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL){
		printf("Invalid pointer in timer_subscribe_int().\n");
		return -1;
	}
	*bit_no = (uint8_t) timer_hook_id;
	int ret = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
	if (ret != 0){
		printf("Error subscribing the timer.\n");
		return -1;
	}
	return 0;
}

int (timer_unsubscribe_int)() {
  	int ret = sys_irqrmpolicy(&timer_hook_id);
	if (ret != 0){
		printf("Error unsubscribing.\n");
		return -1;
	}
	return 0;
}

void (timer_int_handler)() {
  	counter++;
}
