#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int counter = 0;
static int hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // Timer validation: (Before using it)
  if(timer != 0 && timer != 1 && timer != 2) return INVALID_TIMER;

  // Too High or Too Low can't make it
  if(freq > TIMER_FREQ || freq < 19) return INVALID_FREQ;

  uint16_t cnt_value = TIMER_FREQ / freq;
  uint8_t lsb = 0, msb = 0;

  // Get LSB and MSB from Counting Value
  if(util_get_LSB(cnt_value, &lsb) != OK) return NOT_OK;
  if(util_get_MSB(cnt_value, &msb) != OK) return NOT_OK;
  
  // Construct the Control Word
  uint8_t ctrl_word = 0;
  if(timer_get_conf(timer, &ctrl_word) != OK) return GET_CONF_FAIL;
  ctrl_word &= 0x0F;
  ctrl_word |= TIMER_LSB_MSB;

  int port = 0;

  switch (timer)
  {
  case 0:
    ctrl_word |= TIMER_SEL0;
    port = TIMER_0;
    break;
  case 1:
    ctrl_word |= TIMER_SEL1;
    port = TIMER_1;
    break;
  case 2:
    ctrl_word |= TIMER_SEL2;    
    port = TIMER_2;
    break;
  }

  if(sys_outb(TIMER_CTRL,ctrl_word)!= OK) return OUTB_FAIL;
  if(sys_outb(port,lsb)!= OK) return OUTB_FAIL;
  if(sys_outb(port,msb)!= OK) return OUTB_FAIL;

  return OK;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  *bit_no = hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id) != OK) return SET_POLICY_FAIL;
  
  return OK;
}

int (timer_unsubscribe_int)() {

  if(sys_irqdisable(&hook_id) != OK) return DISABLE_FAIL;
  if(sys_irqrmpolicy(&hook_id) != OK) return RM_POLICY_FAIL;
  
  return OK;
}

void (timer_int_handler)() {

  counter += 1;

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  // Construct the Read Back Command
  uint8_t rb_cm = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  if(sys_outb(TIMER_CTRL,rb_cm) != OK) return OUTB_FAIL;

  if(timer == 0){
    return util_sys_inb(TIMER_0, st);
  }
  else if(timer == 1){
    return util_sys_inb(TIMER_1, st);
  }
  else if(timer == 2){
    return util_sys_inb(TIMER_2, st);
  }
  
  return INVALID_TIMER;

}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  // Timer validation: (Before using it)
  if(timer != 0 && timer != 1 && timer != 2) return INVALID_TIMER;

  union timer_status_field_val values;

  // Example: st = 11 11 011 0 --> __11__ = LSB flw by MSB ; 011 = Mode 3 ; 0 = Binary
  
  // Initializing some variables, that were not allowed inside the switch statement
  uint8_t in_mode = st & 0x30;
  uint8_t count_mode = st & 0x0E;
  bool bcd = st & 0x01;
  
  switch(field){
  case tsf_all:
    //Loading Union
    values.byte = st;
    break;
  case tsf_initial:  
    // INIT MODE
    in_mode = (in_mode >> 4);  
    enum timer_init i_mode = in_mode;
    //Loading Union
    values.in_mode = i_mode;
    break;
  case tsf_mode:
    //COUNT MODE    
    count_mode = (count_mode >> 1); 
    if(count_mode > 5) count_mode &= 0x03;
    //Loading Union
    values.count_mode = count_mode; 
    break;
  case tsf_base:  
    //BCD    
    //Loading Union
    values.bcd = bcd;
    break;
  default:
    return INVALID_FIELD;
    }

  return timer_print_config(timer,field, values);
  
}
