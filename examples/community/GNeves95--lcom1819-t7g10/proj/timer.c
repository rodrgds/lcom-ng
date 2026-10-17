#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;

//int timer_print_config(uint8_t timer, enum timer_status_field field, union timer_status_field_val conf);

int (timer_set_frequency)(uint8_t timer, uint32_t freq){
	int endereco;
	uint32_t ctrl;

	if(timer == 0){
		endereco = TIMER_0;
		ctrl = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_SEL0;
	}

	else if(timer == 1){
		endereco = TIMER_1;
		ctrl = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_SEL1;
	}

	else if(timer == 2){
		endereco = TIMER_2;
		ctrl = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_SEL2;
	}

	else return 1;

	uint8_t lsb, msb;

	//-----NEW CODE------
	//util_get_LSB(uint16_t val, uint8_t *lsb)
	util_get_LSB((uint16_t) freq, &lsb);
	//int(util_get_MSB)(uint16_t UNUSED(val), uint8_t *UNUSED(msb))
	util_get_MSB((uint16_t) freq, &msb);
	
	/*---- OLD CODE------
	uint32_t new_freq= TIMER_FREQ / freq;
	uint8_t lsb = (uint32_t) new_freq;
	new_freq >>=8;
	uint8_t msb = (uint32_t) new_freq;
	*/


	if (sys_outb(TIMER_CTRL, ctrl) != 0) return 1;
	if (sys_outb(endereco, lsb) != 0) return 2;
	if (sys_outb(endereco, msb) != 0) return 3;

	return 0;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
	int ret = hook_id;
	*bit_no = hook_id;
	cont = 0;
	if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id) != 0) return -1;
	//if(sys_irqenable(&hook_id)!= 0) return -1;
	return BIT(ret);
}

int (timer_unsubscribe_int)() {
	//if(sys_irqdisable(&hook_id)!= 0) return -1;
	if(sys_irqrmpolicy(&hook_id)!= 0) return -1;
	return 0;
}

void (timer_int_handler)() {
	//printf("cont: %d\n", cont);
  cont++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	int temp = 0;//Created to save the choosen configuration

  //Reads the configuration of a timer & saves it in temp
	if(timer == 0)
		temp = TIMER_0;

	else if(timer == 1)
		temp = TIMER_1;

	else if(timer == 2)
		temp = TIMER_2;

  //Checks for input error
	else{
		printf("Wrong timer input! input = %d\n", timer);
		return 1;
	}

  //Checks if temp didn't get a new value
	if(temp == 0){
		printf("Error assigning timer addr\n");
		return 2;
	}

  //Defining the control port input
	uint32_t ctrl = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  //Verifies that no mistakes were made
	if(sys_outb(TIMER_CTRL, ctrl) != 0){
		printf("Error writing to control word!\n");
		return 3;
	}

	if(sys_inb(temp, &ctrl) != 0){
		printf("Error reading from timer config!\n");
		return 4;
	}
  
  //No mistakes being made the configuration is saved on st
	*st = ctrl;
	return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  //Create variable and save the bits of the configuration
	int output, ncount, access1, access0, mode2, mode1, mode0;

	union timer_status_field_val fv;

	enum timer_init init_mode;

	uint8_t cm;

	bool bincd;

	output = ((st & BIT(7))==0)? 0:1;
	ncount = ((st & BIT(6))==0)? 0:1;
	access1 = ((st & BIT(5))==0)? 0:1;
	access0 = ((st & BIT(4))==0)? 0:1;
	mode2 = ((st & BIT(3))==0)? 0:1;
	mode1 = ((st & BIT(2))==0)? 0:1;
	mode0 = ((st & BIT(1))==0)? 0:1;

	if(access0 && !access1)
		init_mode = LSB_only;
	else if(!access0 && access1)
		init_mode = MSB_only;
	else if(access0 && access1)
		init_mode = MSB_after_LSB;
	else {
		init_mode = INVAL_val;
  }

	if(!mode0 && !mode1 && !mode2)
		cm = 0;
	else if(mode0 && !mode1 && !mode2)
		cm = 1;
	else if(!mode0 && mode1)
		cm = 2;
	else if(mode0 && mode1)
		cm = 3;
	else if(!mode0 && !mode1 && mode2)
		cm = 4;
	else if(mode0 && !mode1 && mode2)
		cm = 5;

	if((st & BIT(0)) == 0) bincd = false;
  else bincd = true;

	switch(field){
		case all:
			fv.byte = st;
		break;
		case initial:
			fv.in_mode = init_mode;
		break;
		case mode:
			fv.count_mode = cm;
		break;
		case base:
			fv.bcd = bincd;
		break;
		default:
			printf("timer_display_conf: wrong field value!\n");
			return 1;
		break;
	}

  // Get the timer stiguration using the function timer_print_config
  timer_print_config(timer, field, fv);

  //if everything passes
  return 0;
}
