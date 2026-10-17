#include "timer.h"

int timer_hook_id = 0; //timer_hook_id for the timer0
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  

  if (freq > TIMER_FREQ || freq < 19){
    printf("Invalid frequency\n");
    return ERROR;
  }

  uint16_t div = TIMER_FREQ / freq;
  uint8_t div_lsb, div_msb;
  util_get_LSB(div, &div_lsb);
  util_get_MSB(div, &div_msb);

  uint8_t control_word;
  if(timer_get_conf(timer, &control_word)<0){
    printf("Error in timer_get_conf\n");
    return ERROR;
  }

  control_word = (control_word & 0x0F)  | TIMER_LSB_MSB;
  if(timer==0) control_word |= TIMER_SEL0;
  if(timer==1) control_word |= TIMER_SEL1;
  if(timer==2) control_word |= TIMER_SEL2;
  
  printf("control_word: %x\n", control_word);

  if(sys_outb(TIMER_CTRL, control_word)!=OK){
    printf("Error in sys_outb\n");
    return ERROR;
  }

  if (sys_outb(TIMER_0 + timer, div_lsb) != OK) {
    printf("Error in sys_outb for LSB\n");
    return ERROR;
  }
  if (sys_outb(TIMER_0 + timer, div_msb) != OK) {
    printf("Error in sys_outb for MSB\n");
    return ERROR;
  }


  return OK;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  *bit_no = BIT(timer_hook_id);

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)!=OK){
    printf("Error in %s \n",__func__);
    return ERROR;
  }

  return OK;
}

int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&timer_hook_id)!=OK){
    printf("Error in %s \n",__func__);
    return ERROR;
  }

  return OK;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  uint8_t temp;
  uint8_t rb_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, rb_command)!=OK){
    printf("Error in sys_outb\n");
    return ERROR;
  }

  util_sys_inb(TIMER_0 + timer, &temp);

  *st = temp;
  
  return OK;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val config;

  switch (field)
  {
  case tsf_all:
    config.byte = st;
    break;
  case tsf_initial:
    config.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
    break;
  case tsf_mode:
    config.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
    printf("count_mode: %x\n", config.count_mode);
    break;
  case tsf_base:  
    config.bcd = st & BIT(0);
    break;
  default:
    printf("Invalid field\n");
    return ERROR;
    break;
  } 

  timer_print_config(timer, field, config);

  return OK;
}

int timer_get_counter(int *result) {
  *result = counter;
  return 1;
}
