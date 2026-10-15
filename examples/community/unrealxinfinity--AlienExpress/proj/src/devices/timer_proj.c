#include "timer_proj.h"

int hook_id_timer = 0;
int timerCount = 0;

uint8_t (timer_subscribe)() {
  uint8_t bit_no = BIT(hook_id_timer);
  
  if(sys_irqsetpolicy(TIMER_IRQ_LINE, IRQ_REENABLE,&hook_id_timer)) return 0xFF;

  return bit_no;
}

int (timer_unsubscribe)() {
  return sys_irqrmpolicy(&hook_id_timer);
}

void (timer_int_handler_proj)() {
  timerCount++;
  counter_timer++;
}






int (timer_set_frequency_proj)(uint8_t timer, uint32_t freq) {

  uint32_t st = timer_get_config(timer);
  if(st == 0xFFFF) return 1;
  uint8_t status = (uint8_t)((st & 0x0F) | TIMER_LSB_MSB); 

  uint32_t temp = CONST_FREQ / freq;
  uint8_t msb = (temp >> 8) & 0xFF;
  uint8_t lsb = (0xFF & temp);
  status &= ~BIT(6);
  status &= ~BIT(7);
  if(sys_outb(TIMER_STATUS, SELECT_TIMER(timer) | status)) return 1;
  if(sys_outb(TIMER_0+timer, lsb)) return 1;
  if(sys_outb(TIMER_0+timer, msb)) return 1;
  return 0;
}


uint32_t timer_get_config(uint8_t timer) {

  if(sys_outb(TIMER_STATUS, TIMER_RB_CMD | TIMER_COUNT_MODE | SELECT_RB_TIMER(timer))) return 0xFFFF;
  uint32_t status;
  if(sys_inb(TIMER_0 + timer, &status)) return 0xFFFF;
  return status;
}
