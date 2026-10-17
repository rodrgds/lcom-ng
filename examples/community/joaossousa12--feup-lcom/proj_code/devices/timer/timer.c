#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int counter = 0;

#define FREQ  30

int (timer_set_frequency)(uint8_t timer, uint32_t freq)
{
  uint8_t controlWord;
  if (timer_get_conf(0, &controlWord) != 0)
    return 1;


  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 


  uint32_t initialValue = TIMER_FREQ / FREQ;
  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  if (sys_outb(TIMER_CTRL, controlWord) != 0) 
    return 1;

  if (sys_outb(0x40, LSB) != 0) 
    return 1;
  if (sys_outb(0x40, MSB) != 0) 
    return 1;

  return 0;
}

int (timer_subscribe_interrupts)() {
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_interrupts)() {
  if (sys_irqrmpolicy(&hook_id) != 0) 
    return 1;
  return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) 
    return 1;
    
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, RBC) != 0) 
    return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) 
    return 1;

  return 0;
}
