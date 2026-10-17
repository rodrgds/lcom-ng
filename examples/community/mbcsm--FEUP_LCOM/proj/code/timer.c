#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  uint8_t controlWord;

  timer_get_conf(timer, &controlWord);

  controlWord = controlWord & 0x0F; // Mask to keep 4LSBs and remove 4MSBs
  freq = TIMER_FREQ / freq;

  if (timer == 0) {
	controlWord = TIMER_SEL0 | TIMER_LSB_MSB | controlWord; //Assemble the control word

	if (sys_outb(TIMER_CTRL, controlWord) != OK) //Write control word in the control register
	  return 1;

	if (sys_outb(TIMER_0, (uint8_t) freq) != OK)       //
	  return 1;                                        // Load the frequency value to the counter register
	if (sys_outb(TIMER_0, (uint8_t)(freq >> 8)) != OK) //
	  return 1;
  }
  else if (timer == 1) {
	controlWord = TIMER_SEL1 | TIMER_LSB_MSB | controlWord; //Assemble the control word

	if (sys_outb(TIMER_CTRL, controlWord) != OK) //Write control word in the control register
	  return 1;

	if (sys_outb(TIMER_1, (uint8_t) freq) != OK)       //
	  return 1;                                        // Load the frequency value to the counter register
	if (sys_outb(TIMER_1, (uint8_t)(freq >> 8)) != OK) //
	  return 1;
  }
  else if (timer == 2) {
	controlWord = TIMER_SEL2 | TIMER_LSB_MSB | controlWord; //Assemble the control word

	if (sys_outb(TIMER_CTRL, controlWord) != OK) //Write control word in the control register
	  return 1;

	if (sys_outb(TIMER_0, (uint8_t) freq) != OK)       //
	  return 1;                                        // Load the frequency value to the counter register
	if (sys_outb(TIMER_0, (uint8_t)(freq >> 8)) != OK) //
	  return 1;
  }
  else
	return 1;

  return 0;
}

int hook = 0;
int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook) != OK) {
	return -1;
  }

  //printf("subscibe successfull\n");

  return 0;
}

int(timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook) != OK) {
	return -1;
  }

  //printf("unsubscibe successfull\n");
  return 0;
}
int counterSeconds = 0, counter = 0;
void(timer_int_handler)() {
	counter++;
	if(counter % sys_hz() == 0){
		int second = counter / 60;
		counterSeconds = second;
	}
}


int(timer_get_conf)(uint8_t timer, uint8_t *st) {

	uint8_t byte = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
	uint32_t st32Temp = 0;

	if (sys_outb(TIMER_CTRL, byte) != OK) {
		//printf("timer config = Error");
		return 1;
	}

	switch (timer){
	case 0:
	if (sys_inb(TIMER_0, &st32Temp) != OK) {
		//printf("timer config = Error");
		return 1;
		}
	break;
	case 1:
	if (sys_inb(TIMER_1, &st32Temp) != OK) {
			//printf("timer config = Error");
			return 1;
		}
	break;
	case 2:
	if (sys_inb(TIMER_2, &st32Temp) != OK) {
			//printf("timer config = Error");
			return 1;
		}
	break;
	default:
	return 1;
	}

  *st = (uint8_t) st32Temp;
  //printf("Timer Get Conf: DONE\n");

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val val;

  if (field == all) // FIELD == ALL
	val.byte = st;
  else if (field == initial) // FIELD == INIT
  {
	if (TIMER_LSB_MSB & st)
	  val.in_mode = MSB_after_LSB;
	else if (TIMER_MSB & st)
	  val.in_mode = MSB_only;
	else if (TIMER_LSB & st)
	  val.in_mode = LSB_only;
	else
	  val.in_mode = INVAL_val;
  }
  else if (field == mode) { // FIELD == MODE
	st = (st >> 1) & 0x07;
	val.count_mode = st;
  }
  else if (field == base) // FIELD == BASE
  {
	if (TIMER_BCD & st)
	  val.bcd = true;
	else if (TIMER_BIN & st)
	  val.bcd = false;
  }
  else
	return 1;
  if (timer_print_config(timer, field, val) != OK)
	return 1;

  return 0;
}
