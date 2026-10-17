#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "i8254.h"

int hook_id = TIMER0_IRQ;
uint32_t counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st;
  uint32_t val = (TIMER_FREQ / freq); 
  timer_get_conf(timer, &st);
  printf("timer get config %x \n", st);

  uint8_t LSB;
  uint8_t MSB;
 
  util_get_LSB(val,&LSB);
  util_get_MSB(val,&MSB);

   printf("LSB %x \n", LSB);
   printf("MSB %x \n", MSB);
  
  if (timer == 0){

    uint8_t st2 = ((st & (BIT(0) | BIT(1) | BIT(2) | BIT(3))));
    uint8_t st3 = (TIMER_SEL0 | TIMER_LSB_MSB) ;
    st = st3 | st2;

    if (sys_outb(TIMER_CTRL, st) != 0) 
        return 1;
  
    if (sys_outb(TIMER_0 + timer, LSB) != 0) 
        return 1;
    
    if (sys_outb(TIMER_0 + timer, MSB) != 0)
        return 1;

    return 0;
  }

if (timer == 1){

    uint8_t st2 = ((st & (BIT(0) | BIT(1) | BIT(2) | BIT(3))));
    uint8_t st3 = (TIMER_SEL1 | TIMER_LSB_MSB) ;
    st = st3 | st2;

    if (sys_outb(TIMER_CTRL, st) != 0) 
        return 1;
  
    if (sys_outb(TIMER_0 + timer, LSB) != 0) 
        return 1;
    
    if (sys_outb(TIMER_0 + timer, MSB) != 0)
        return 1;

    return 0;
  }


if (timer == 2){

    uint8_t st2 = ((st & (BIT(0) | BIT(1) | BIT(2) | BIT(3))));
    uint8_t st3 = (TIMER_SEL2 | TIMER_LSB_MSB) ;
    st = st3 | st2;

    if (sys_outb(TIMER_CTRL, st) != 0) 
        return 1;
  
    if (sys_outb(TIMER_0 + timer, LSB) != 0) 
        return 1;
    
    if (sys_outb(TIMER_0 + timer, MSB) != 0)
        return 1;

    return 0;
  }
else
  return 1;

}

int (timer_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0)
      return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0)
      return 1;

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t read_back_cmd= TIMER_RB_CMD| TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL, read_back_cmd)!=0)return 1;
  if(util_sys_inb(TIMER_0 + timer, st)!=0) return 1;
  //printf("get conf %x \n", *st);
  return OK;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val conf;
  int get_init_bytes = (st << 2)>>6; 
  int get_count_bytes = (st << 4)>>5;


  switch(field){

    case tsf_all:
      conf.byte=st;
      printf("status byte %x", st);
      break;



    case tsf_initial:
      
      switch (get_init_bytes)
      {
        case 1:
            conf.in_mode=TIMER_LSB;
          break;
        case 2:
          conf.in_mode=TIMER_MSB;
          break;
        case 3:
          conf.in_mode=TIMER_LSB_MSB;
        default:
          break;
      }
      break;



    case tsf_mode:
     switch (get_count_bytes)
      {
        if(get_count_bytes== 0)
          conf.count_mode=0;
  
        if(get_count_bytes== 1)
          conf.count_mode=1;
          break;
        if(get_count_bytes== 2 || get_count_bytes==6)
          conf.count_mode=2;
      
        if(get_count_bytes== 3 || get_count_bytes==7)
          conf.count_mode=3;
        if(get_count_bytes== 4)
          conf.count_mode=4;
        if(get_count_bytes==  5)
          conf.count_mode=5;
      }
      break;


    case tsf_base:

      if (conf.bcd == 0){
        conf.bcd = true;
      }

      else if (conf.bcd == 1){
        conf.bcd = false;
      }

      else{
        printf("Invalid tsf_base \n");
        return 1;
      }
        
      break;


    default:
      printf("Invalid field \n");
      return 1;
  
  

  }
  timer_print_config(timer, field, conf);
  return 0;
}
