#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer_count.h"

static int hook_id;
static unsigned int count = 0;

unsigned int timer_get_count() {
  return count;
}

void timer_reset_count() {
  count = 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq >= TIMER_FREQ) {
    printf("Invalid frequency\n");
    return 1;
  }

  uint8_t status;
  if(timer_get_conf(timer,&status)!=0) {
    printf("Failed to get timer configuration.\n");
    return 1;
  }

  uint8_t control_word = status & 0xF;
  control_word |= (BIT(4) | BIT(5) | (timer<<6));
  if(sys_outb(TIMER_CTRL,control_word)!=0) {
    printf("Failed to write control word.\n");
    return 1;
  }

  uint16_t final_freq = TIMER_FREQ / freq;
  uint8_t lsb,msb;
  util_get_LSB(final_freq,&lsb);
  util_get_MSB(final_freq,&msb);

  if(sys_outb(TIMER_0 + timer,lsb) !=0 ) {
    printf("Failed to write LSB.\n");
    return 1;
  }
  if(sys_outb(TIMER_0 + timer,msb) !=0 ) {
    printf("Failed to write MSB.\n");
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id = TIMER0_IRQ;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) {
    printf("Failed to subscribe timer interrupts.\n");
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) {
    printf("Failed to unsubscribe timer interrupts.\n");
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rbc = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL,rbc) != 0){
    printf("Failed to write read-back command.\n");
    return 1;
  }
  if(util_sys_inb(TIMER_0+timer, st) != 0){
    printf("Failed to read timer configuration.\n");
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val print_val;
  uint8_t mask;
  uint8_t mode;
  switch (field){
    case tsf_all:
      print_val.byte=st;
      break;

    case tsf_initial:
      switch (st & (BIT(4) | BIT(5))){
        case BIT(4):
          print_val.in_mode = LSB_only;
          break;
        case BIT(5):
          print_val.in_mode = MSB_only;
          break;
        case (BIT(4) | BIT(5)):
          print_val.in_mode = MSB_after_LSB;
          break;
        default:
          print_val.in_mode = INVAL_val;
          break;
      }
      break;

    case tsf_mode:
      mask = BIT(1) | BIT(2) | BIT(3);
      mode = (st & mask)>>1;
      if(mode > 5){
        print_val.count_mode = mode-4;
      }else{
        print_val.count_mode = mode;
      }
      break;

    case tsf_base:
      print_val.bcd= st & BIT(0); 
      break;

    default:
      return 1;
      break;      
  }
  if(timer_print_config(timer,field,print_val)!=0){
    printf("Failed to print timer configuration.\n");
    return 1;
  }
  return 0;
}
