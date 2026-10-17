#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ) return 1;
    uint8_t conf;
    uint16_t counter = TIMER_FREQ / freq;
    if (timer_get_conf(timer, &conf) != 0) return 1;
    conf = conf & 0x0F;
    conf = conf | BIT(5) | BIT(4);
    switch(timer){
      case 0:
        break;
      case 1:
        conf = conf | BIT (6);
        break;
      case 2:
        conf = conf | BIT (7);
        break;
      default:
        break;
    }

    uint8_t LSB, MSB;
    util_get_MSB(counter, &MSB);
    util_get_LSB(counter, &LSB);
    if (sys_outb(0x43, conf) != 0) return 1;
    sys_outb(0x40 + timer, LSB);
    sys_outb(0x40 + timer, MSB);

    return 0;
  }

int (timer_subscribe_interrupts)() {
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;
  return 0;
}

int (timer_unsubscribe_interrupts)() {
   return (sys_irqrmpolicy(&hook_id));
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t controlword = BIT(6) | BIT(7) | BIT(5) | BIT (timer + 1);
  if (sys_outb(0x43, controlword) != 0) return 1;
  if (util_sys_inb(0x40 + timer, st)) return 1;
  return 0;
}

