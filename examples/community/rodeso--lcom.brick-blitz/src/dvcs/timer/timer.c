#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "pit.h"

int globalCounter = 0;
int hook_id=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  printf("%s is running!\n", __func__);

  if (freq > TIMER_FREQ || timer<0 || timer>2) {return 1;} //checking if given timer exists and if given frequency makes sense


  uint8_t cw;
  int 
  rea = timer_get_conf(timer,&cw); //gets and reads the timers control world
  if (rea==1) {return 1;}

  cw = (cw & 0xF) | TIMER_LSB_MSB; //Maintains the 4 LSBs and cleans the rest, and then inputs the operating mode

  uint32_t squareWaveFreq = TIMER_FREQ / freq; //defines the squareWaveFreq
  uint8_t lsb;
  util_get_LSB(squareWaveFreq, &lsb);
  uint8_t msb;
  util_get_MSB(squareWaveFreq, &msb);

  uint8_t port;
  switch(timer) { //inputs the provided timer into the control world  
    case 0:
      port=TIMER_0;
      cw=(cw | TIMER_SEL0);
      break;
    case 1:
      port=TIMER_1;
      cw=(cw | TIMER_SEL1);
      break;
    case 2:
      port=TIMER_2;
      cw=(cw | TIMER_SEL2);
      break;
  }

  int wri = sys_outb(TIMER_CTRL, cw); //writes updated control word, in the control register
  if (wri==1) {return 1;}

  int lb = sys_outb(port,lsb); //Loads the timer initial value, by writing to the timers port
  if(lb==1) {return 1;}
  int mb = sys_outb(port,msb);
  if(mb==1) {return 1;}


  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) { //subscribes/activates interrupts for the timer

  printf("%s is running!\n", __func__);

  if(bit_no==NULL) {return 1;}

  hook_id= TIMER0_IRQ;
  *bit_no=hook_id;

  int pol= sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id);
  if (pol==1) {return 1;}

  return 0;
}

int (timer_unsubscribe_int)() { //unsubscribes interrupts for the timer

  printf("%s is running!\n", __func__);

  int pol = sys_irqrmpolicy(&hook_id);
  if (pol==1) {return 1;}

  return 0;
}

void (timer_int_handler)() {
  
  printf("%s is running!\n", __func__);
  globalCounter++;

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  int se=1;
  int res=1;

  uint8_t cw = TIMER_RB_CMD|TIMER_RB_SEL(timer)|TIMER_RB_COUNT_;  //readback command creation
  se = sys_outb(TIMER_CTRL,cw);  //sending readback command

  res = util_sys_inb(0x40+timer,st); //receiving the status byte (st)


  /*printf("%s is not yet implemented!\n", __func__);*/
  return (res | se);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  int res = 1;
  
  printf("%s is running!\n", __func__);
  
  union timer_status_field_val values;

  switch(field) {

    case tsf_all:  //Display status byte, in hexadecimal
      values.byte=st;
      res = timer_print_config(timer,field,values);
      if (res==1) return 1;
      break;

    case tsf_initial:  //Display the initialization mode, only
      st= (st>>4) & 0x03;
      switch (st) {
        case 0:
          return 1;
        case 1:
          values.in_mode=LSB_only;
          break;
        case 2:
          values.in_mode=MSB_only;
          break;
        case 3:
          values.in_mode=MSB_after_LSB;
          break;
      }
      res= timer_print_config(timer,field,values);
      if (res==1) return 1;
      break;

    case tsf_mode:  //Display the counting (counters) mode, only
      st=(st>>1) & 0x07;
      values.count_mode=st;
      res= timer_print_config(timer,field,values);
      if (res==1) return 1;
      break;

    case tsf_base:  //Display the counting base, only
      st=st&0x01;    
      values.bcd=st;
      res= timer_print_config(timer,field,values);
      if (res==1) return 1;
      break;

    default:
        printf("field doesn't exist!");
        return 1;
  }


  return 0;
}
