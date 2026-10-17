#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


int setHook = 0;

uint32_t timerCounter = 0;


int (timer_set_frequency)(uint8_t timer, uint32_t freq)
 {
   
  if(timer < 0 || 2 < timer) 
  {
    printf("Invalid timer\n");
    return 1;
  } 
  
  //The minimum value is due to limitations of representing a number in 16 bits
  //The maximum value is the frequency
  
  if (!((19 < freq) && (freq < TIMER_FREQ))) 
  {
    printf("Invalid input frequency!\n");
    return 1;
  }

  uint8_t st;
  uint8_t timerREF, timerSEL, freqLSB, freqMSB;
  
  timer_get_conf(timer, &st);  
  st = st & 0x0F;
  
  //Select the Count Register and the Control Word according to timer
  switch(timer)
	{
    case 0:
      timerSEL = TIMER_SEL0;
      timerREF = TIMER_0;
      break;

    case 1:
      timerSEL = TIMER_SEL1;
      timerREF = TIMER_1;
      break;
    
    case 2:
      timerSEL = TIMER_SEL2;
      timerREF = TIMER_2;
      break;
    
    default:
      return 1;
      break; 
	}

  sys_outb(TIMER_CTRL, timerSEL | TIMER_LSB_MSB | st);

  uint32_t freqClock = TIMER_FREQ / freq;

  util_get_LSB(freqClock, &freqLSB);
  util_get_MSB(freqClock, &freqMSB);

  sys_outb(timerREF, freqLSB);
  sys_outb(timerREF, freqMSB);

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) 
{
  *bit_no = setHook;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &setHook);
  return 0;
}

int (timer_unsubscribe_int)() 
{
  sys_irqrmpolicy(&setHook);
  return 0;
}

void (timer_int_handler)() 
{
  
  timerCounter = (timerCounter +1) % 60;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) 
{
  if(timer < 0 || 2 < timer) 
  {
    printf("Invalid timer\n");
    return 1;
  } 

  sys_outb(TIMER_CTRL, 0x00 | TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  switch(timer)
	{
    case 0:
      util_sys_inb(TIMER_0, st);
      return 0;

    case 1:
      util_sys_inb(TIMER_1, st);
      return 0;
    
    case 2:
      util_sys_inb(TIMER_2, st);
      return 0;

    default:
      return 1;
	}
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) 
{
  union timer_status_field_val result;

if(timer < 0 || 2 < timer) 
  {
    printf("Invalid timer\n");
    return 1;
  } 
  
  //Select the byte of control word or one field of that (i.e. operating mode)
  switch(field)
  {
    case (tsf_all):
      result.byte = st;
      break;

    //Initalization Mode of Control Word
    case tsf_initial:
      if((st & 0x30) == TIMER_LSB_MSB) result.in_mode = MSB_after_LSB ;
      if((st & 0x30) == TIMER_LSB) result.in_mode = LSB_only;
      if((st & 0x30) == TIMER_MSB) result.in_mode = MSB_only;
      if((st & 0x30) == 0x00) result.in_mode = INVAL_val;
      break;

    //Operating Mode of Control Word
    case tsf_mode:
      if ((st & 0x0E) == 0x00) result.count_mode = 0;
      if ((st & 0x0E) == 0x02) result.count_mode = 1;
      if ((st & 0x0E) == 0x04 || (st & 0x0E) == 0x0C ) result.count_mode = 2;
      if ((st & 0x0E) == 0x06 || (st & 0x0E) == 0x0E ) result.count_mode = 3;
      if ((st & 0x0E) == 0x08) result.count_mode = 4;
      if ((st & 0x0E) == 0x0a) result.count_mode = 5;
      break;

    //Counting base of Control Word
    case tsf_base:
      if (st & TIMER_BCD) result.bcd = true;
      else result.bcd = false;
      break;

    default:
      return 1;  
      break;
  }

  timer_print_config(timer, field, result);

  return 0;
}
