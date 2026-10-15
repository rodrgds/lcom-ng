#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;
int counter = 0; //counter for how many times the timer signaled an interruption

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || freq<19 || freq>TIMER_FREQ) return 1; //checks validity of timer and frequency

  uint8_t old_st, new_st;
  timer_get_conf(timer, &old_st);

  new_st = old_st & 0x0F; //want operating mode and counting base to stay the same
  new_st = new_st | TIMER_LSB_MSB; //want to put lsb and msb
  sys_outb(TIMER_CTRL,new_st); //tells timer control we will input new configuration

  uint16_t value = TIMER_FREQ / freq; //value we want to put in the timer
  uint8_t lsb, msb;
  util_get_LSB(value, &lsb);
  util_get_MSB(value, &msb);

  sys_outb(TIMER_0+timer, lsb);
  sys_outb(TIMER_0+timer, msb);
  //write lsb followed by msb in the timer

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; //check if pointer is valid
  *bit_no = BIT(timer_hook_id); //bit_no is now a mask

  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id); //subscribe to the interrupts on IRQ_0 and bit 0
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  counter ++; //the timer was called one more time
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  if (timer > 2 || st==NULL) return 1; //checks if timer is valid (0<=timer<=2) and if the pointer is valid

  uint8_t ctrl_word = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); //control word to say we want to read the configuration of the timer

  if(sys_outb(TIMER_CTRL, ctrl_word)) return 1; //writes the control word to the timer control
  if(util_sys_inb(TIMER_0+timer, st)) return 1; //reads the configuration of timer and puts it in st

  return 0; //everything went as expected
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf; //defined as union, changes type depending on field
  uint8_t aux;

  switch (field) {
    case tsf_all: //display the whole byte
      conf.byte = st; //conf is of "byte" type
      break;
    
    case tsf_initial: //display the initialization mode
      //initialization mode is in bits 4 and 5
      aux = (st >> 4) & 0x03; //now aux has the bits 4 and 5 as the bits 0 and 1

      if(aux==1) conf.in_mode = LSB_only; //only bit 4 was active
      else if (aux==2) conf.in_mode = MSB_only; //only bit 5 was active
      else if (aux==3) conf.in_mode = MSB_after_LSB; //both bits were active
      else conf.in_mode = INVAL_val; //something went wrong
      break;

    case tsf_mode: //display the counting mode
      //initialization mode is in bits 1, 2 and 3
      aux = (st >> 1) & 0x07; //now aux has the bits 1, 2 and 3 as the bits 0, 1 and 2

      if(aux==6) conf.count_mode = 2; //both 010 and 110 is mode 2
      else if(aux==7) conf.count_mode = 3; //both 011 and 111 is mode 3
      else conf.count_mode = aux; //the mode corresponds to the number in binary in bits 1, 2 and 3
      break;

    case tsf_base: //display the counting base
      //the counting base is in bit 0
      conf.bcd = st & TIMER_BCD; //0x01 (true) if the last bit of st is active (counting base is bcd)
      break;

    default:
      return 1; //field is not an accepted value
  }

  // prints some fields of a timer's configuration/status.
  return timer_print_config(timer, field, conf); 
}
