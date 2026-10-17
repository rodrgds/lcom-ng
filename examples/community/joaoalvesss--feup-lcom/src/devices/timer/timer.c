#include "timer.h"

static int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
     if((freq > TIMER_FREQ) || (freq < 19)) return 1;
     if((timer < 0) || (timer > 2)) return 1;
     
     uint8_t ctrlWord;
     if(timer_get_conf(timer, &ctrlWord) != 0)
          return 1;

     ctrlWord = ctrlWord & 0x0F;
     ctrlWord |= TIMER_LSB_MSB | (timer << 6);
     if(sys_outb(TIMER_CTRL, ctrlWord) != 0) return 1;

     uint32_t frequency = TIMER_FREQ / freq;
     uint8_t LSB, MSB;
     util_get_LSB(frequency, &LSB);
     util_get_MSB(frequency, &MSB);

     if(sys_outb(TIMER_0 + timer, LSB) != 0) return 1;
     if(sys_outb(TIMER_0 + timer, MSB) != 0) return 1;

     return 0;
}

int (timer_subscribe_interrupts)() {
     if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) return 1;
     return 0;
}

int (timer_unsubscribe_interrupts)() {
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;

  return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t rbcommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, rbcommand))return 1; 

  util_sys_inb(TIMER_0 + timer, st);
  return 0;
}


