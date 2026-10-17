#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int counter = 0;
int t_hook_id = 4;

/* READ CONFIG, BUILD CONTROL WORD BASED ON CONFIG, WRITE CONTROL WORD IN CONTROL REG, WRITE FREQUENCY VALUE VALUE IN CHOSEN VALUE*/
/*Only works with freq >= 19*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t var;
  
  if(timer_get_conf(timer,&var) != 0) {
    printf("Error in timer_get_conf function in timer_set_frequency function");
    return 1;
  }

  var = var & 0x0f; //To keep the last 4 bits unchanged

  uint16_t actual_frequency = TIMER_FREQ / freq;

  uint8_t freq_lsb, freq_msb;

  util_get_LSB(actual_frequency, &freq_lsb);
  util_get_MSB(actual_frequency, &freq_msb);

  uint8_t chosen_timer;
  switch(timer) {
    case 0:
      chosen_timer = TIMER_SEL0;
      break;
    case 1: 
      chosen_timer = TIMER_SEL1;
      break;
    case 2: 
      chosen_timer = TIMER_SEL2;
      break;
    default: 
      printf("Wrong Timer!");
      return 1;
      break;
  }
  
  uint8_t read_back_command =  chosen_timer | TIMER_LSB_MSB | var;

  if(sys_outb(TIMER_CTRL,read_back_command) != 0) {
    printf("fail in frq");
    return 1;
  }

  if(sys_outb(TIMER_0+timer,freq_lsb) != 0) {
    printf("Error in set frequency LSB");
    return 1;
  }

  if(sys_outb(TIMER_0+timer, freq_msb) != 0) {
    printf("Error in set frequency MSB");
    return 1;
  } 

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  *bit_no = BIT(t_hook_id); //To get the bit number of the hook_id

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&t_hook_id) != 0) {
    printf("Error in sys_irqsetpolicy");
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&t_hook_id) != 0) {
    printf("Error in sys_irqrmpolicy");
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t read_back = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  if(sys_outb(TIMER_CTRL,read_back) != 0) {
    printf("Error in sys_outb\n");
    return 1;
  }

  if(util_sys_inb(TIMER_0+timer,st) != 0) {
    printf("Error in util_sys_inb\n");
    return 1;
  }
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  union timer_status_field_val val;
  uint8_t aux = 0x00;
  switch (field)
  {
  case tsf_initial:
    aux = st >> 4;
    aux = aux & (BIT(1) | BIT(0)); 
    if(aux == 1) {
      val.in_mode = LSB_only;
    } else if (aux == 2) {
      val.in_mode = MSB_only;
    } else if (aux == 3) {
      val.in_mode = MSB_after_LSB;
    } else {
      val.in_mode = INVAL_val;
    }break;
  case tsf_mode:
    aux = st >> 1;
    aux = aux  & (BIT(2) | BIT(1) | BIT(0)); 
    if(aux == 6) aux = 2;
    else if(aux == 7) aux = 3;
    val.count_mode = aux;
    break;
  case tsf_base:
    if((st & BIT(0))) 
      val.bcd = true;
    else
      val.bcd = false; break;
  case tsf_all: 
    val.byte = st; break;
    break;  
  default:
    break;
  }

  if (timer_print_config(timer, field, val) != 0) {
    printf("Error in timer_print_config");
    return 1;
  }

  return 0;
}
