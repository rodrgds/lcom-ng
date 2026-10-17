#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
int hook_id=0;
int count =0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  uint16_t counterValue = TIMER_FREQ / freq;

  uint8_t MSB,LSB;

  if(util_get_LSB(counterValue,&LSB)!=0)return 1;
  if(util_get_MSB(counterValue,&MSB)!=0)return 1;

  uint8_t controlWord;

  if(timer_get_conf(timer,&controlWord)!=0)return 1;
  
  controlWord = controlWord & 0x0F;
  controlWord = controlWord | BIT(4) | BIT(5);


  uint8_t timerPort;
  switch (timer)
  {
  case 0:
    timerPort= 0x40;
    break;
  case 1:
    timerPort = 0x41;
    controlWord = controlWord | BIT (6);
    break;
  case 2:
    timerPort = 0x42;
    controlWord = controlWord | BIT (7);
    break;
  default:
    return 1;
  }

 
  if(sys_outb(0x43, controlWord)!=0) return 1;
  if(sys_outb(timerPort,LSB)!=0)return 1;
  if(sys_outb(timerPort,MSB)!=0)return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
   *bit_no = BIT(hook_id);

    if(sys_irqsetpolicy(0,IRQ_REENABLE,&hook_id)!=0)return 1;


  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id)!=0)return 1;

  return 0;
}

void (timer_int_handler)() {
  count++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t RBC = BIT(7) | BIT(6) | BIT (5) | BIT(timer +1); // criar o RBC command

  if(sys_outb(0x43,RBC)!=0) return 1; // colocar na porta 0X43

  if(util_sys_inb(0x40 + timer,st)!=0) return 1; // ler a controlWord

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val val;



  switch (field) // colocar na variavel val o valor desejado para ser printado
  {
  case tsf_all:
    val.byte = st;
    break;
  
  case tsf_initial:
    st = (st >> 4) & 0x03;
    val.in_mode = st;
   break;

  case tsf_mode:
    st= (st >> 1) & 0x07;
    if(st == 6 ) st=2;
    else if(st== 7) st=3;
    val.count_mode = st;
    break;

  case tsf_base:
    st = st & 0x01;
    val.bcd = st;

    break;

  default:
    break;
  }


  if(timer_print_config(timer,field,val)!=0 )return 1;
  return 0;
}
