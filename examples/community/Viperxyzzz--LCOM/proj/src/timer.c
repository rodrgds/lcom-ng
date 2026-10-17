#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


static unsigned long timer_counter = 0;
static int hook_id = TIMER0_IRQ;




int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //frequency using the formula
  uint16_t f  = TIMER_FREQ / freq;


  //----------------------------//
  uint8_t st = 0,lsbF,msbF;
  uint8_t rbcmd = 0;

  //----------------------------//

  //reading first so we can save the 4LSB 
  timer_get_conf(timer,&st);
  
  //saving the first 4LSB 
  rbcmd = st & 0x0F; 


  //initialization mode
  rbcmd |= TIMER_LSB_MSB;
  

  //selecting the correct control word for the timer
  switch (timer)
  {
    case 0: 
      rbcmd |= TIMER_SEL0;
      break;
    case 1:
      rbcmd |= TIMER_SEL1;
      break;
    case 2:
      rbcmd |= TIMER_SEL2;
      break;
    default:
      return 1;
  }
  


  //writing the control word
  if (sys_outb(TIMER_CTRL,rbcmd))
  {
    return 1;
  }
  

  //getting the frequency we want to write
  //splitted in two bytes
  if (util_get_LSB(f,&lsbF) || util_get_MSB(f,&msbF))
  {
    return 1;
  }

  //writing the frequency lsbF first and msbF afterwards
  if (sys_outb(TIMER_0 + timer,lsbF) || sys_outb(TIMER_0 + timer, msbF))
  {
    return 1;
  }


  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  //bit_no is a return argument which contains the bit mask created from hook_id
  hook_id = TIMER0_IRQ;
  *bit_no = TIMER0_IRQ;
  //subscribes notifications from timer0 based on hook_id passed as 3rd argument
  //from now on the IRQ line 0 will be masked
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK){
    printf("sys_irqsetpolicy failed.\n");
    return 1;
  }
  return 0;
}



int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != OK) {

    printf("sys_irqrmpolicy of timer_unsubscribe_int failed.\n");
    return 1;
  }
  return 0;
}



void (timer_int_handler)() {
  timer_counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint32_t rbcmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  sys_outb(TIMER_CTRL,rbcmd);


  if (util_sys_inb(TIMER_0 + timer,st) == 0)
  {
    return 0;
  }
  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  
  union timer_status_field_val conf;


  switch(field)
  {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_base:
      conf.bcd = st & BIT(0);
      break;
    case tsf_initial:
      conf.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
      break;
    case tsf_mode:
      conf.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      if (conf.count_mode & BIT(1))
      {
        conf.count_mode = conf.count_mode & BIT(2);
      }
      break;
    default:
      break;
  }


  timer_print_config(timer,field,conf);



  return 0;
}
