#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "./timer.h"

#include <stdint.h>

#include "i8254.h"

// ---------------------------------- TIMER FREQUENCY SETTING ----------------------------------
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(timer < 0 || timer > 2) return 1; //invalid timer
  
  uint8_t st;
  if (timer_get_conf(timer, &st)) return 1;

  //configure the control word
  st &= 0x0F; //keep only the 4 least significant bits
  st |= TIMER_LSB_MSB; //set the access mode to lsb followed by msb
  st |= timer << 6; //set the timer selection bits

  //calculate the divisor and write it to the timer control register
  uint16_t div = TIMER_FREQ / freq;
  if(sys_outb(TIMER_CTRL, st)) return 1;

  //split the divisor into lsb and msb (in that order) 
  uint8_t lsb, msb; 
  if(util_get_LSB(div, &lsb)) return 1;
  if(util_get_MSB(div, &msb)) return 1;

  //write the lsb and msb to the timer ports
  if(sys_outb(TIMER_0 + timer, lsb)) return 1;
  if(sys_outb(TIMER_0 + timer, msb)) return 1;

  return 0;
}
// ---------------------------------- INTERRUPT HANDLING ----------------------------------
int hook_id = 0;
int (timer_subscribe_int)(uint8_t *bit_no) {
    /* 
      subscribe timer interrupt 
      configure GIH to set bit_no to 1 in the kernel message's interrupt bitset
    */

    if(bit_no == NULL) return 1;
    hook_id = TIMER0_IRQ; 
    *bit_no = hook_id;

    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;
    
  return 0;
}

int (timer_unsubscribe_int)() {
  /* unsubscribe timer interrupt */
  if(sys_irqrmpolicy(&hook_id)) return 1;

  return 0;
}

int int_counter = 0;
void (timer_int_handler)() {
  /* increment a global counter for tracking the nr of interrupts */
  int_counter++;
}

// ----------------------------------- TIMER CONFIGURATION ----------------------------------

//Read the configuration of the specified timer
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  /* Verifications */
  if (timer < 0 || timer > 2) return 1;
  if(st == NULL) return 1;
  
  /* build the read-back command */
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  /* send the read-back command to the control register */
  if(sys_outb(TIMER_CTRL, rb_cmd)) return 1;
  
  /* read the status byte from the selected timer port */
  if(util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 0;
}


//Display a timer configuration, depending on the selected field (status, initial mode, mode and counting base)
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  /* Verifications */
  if (timer < 0 || timer > 2) return 1;

  union timer_status_field_val val;

  switch(field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      val.in_mode = INVAL_val;
      if (st & TIMER_LSB_MSB) val.in_mode = MSB_after_LSB;
      else if (st & TIMER_LSB) val.in_mode = LSB_only;
      else if (st & TIMER_MSB) val.in_mode = MSB_only;
      break;
    case tsf_mode:
      val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      if (val.count_mode == 6) val.count_mode = 2;
      else if (val.count_mode == 7) val.count_mode = 3;
      break;
    case tsf_base:
      val.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }


  return timer_print_config(timer, field, val);
}

// ----------------------------------- GETTERS & SETTERS ----------------------------------

int get_int_counter() {
  return int_counter;
}

int set_int_counter(int new_value) {
  int_counter = new_value;
  return 0;
}

// --------------------------- PROJECT TIMER FUNCTIONS ---------------------------

void timer_ih_proj(void) {
    timer_int_handler();
}

uint32_t timer_get_ticks(void) {
    return (uint32_t) int_counter;
}

bool timer_elapsed(int ms) {
    int ticks_needed = (ms * 60) / 1000;
    if (ticks_needed == 0 && ms > 0) ticks_needed = 1;
    return int_counter >= ticks_needed;
}
