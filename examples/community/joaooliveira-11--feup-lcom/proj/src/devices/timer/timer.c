#include "timer.h"

int timer_hook_id = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq){
  if(timer != 0 ||  freq < 19 || freq > 1193182) return 1;

  uint16_t counterValue = 1193182 / freq;
  uint8_t lsb, msb;
  if(util_get_LSB(counterValue, &lsb) != 0) return 1;
  if(util_get_MSB(counterValue, &msb) != 0) return 1;

  uint8_t controlword;
  if(timer_get_conf(timer, &controlword) != 0) return 1;
  controlword = (controlword & 0x0F) | BIT(4) | BIT(5);
  
  if(sys_outb(0x43, controlword) != 0) return 1;
  if(sys_outb(0x40, lsb) != 0) return 1;
  if(sys_outb(0x40, msb) != 0) return 1;
  return 0;
}

int(timer_subscribe_int)(uint8_t *mask){
  if(mask == NULL) return 1;
  *mask = BIT(timer_hook_id);
  if(sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hook_id) != 0) return 1;
  return 0;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st){
  if(st == NULL) return 1;
  uint32_t controlword = BIT(7) | BIT(6) | BIT(5) | BIT(1);
  if(sys_outb(0x43, controlword) != 0) return 1;
  if(util_sys_inb(0x40, st) != 0) return 1;
  return 0;
}

int(timer_unsubscribe_int)(){
  if(sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
  return 0;
}
