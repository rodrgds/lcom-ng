#include <lcom/lcf.h>

#include <stdint.h>

#include "../helper/i8042.h"

uint8_t scancode = 0;
int hook_id = 1;

int(keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int read_controller(uint8_t port, uint8_t *output, uint8_t mouse) {

  uint8_t st;
  uint8_t attemps = 10;

  while (attemps) {

    if (util_sys_inb(ST_REG, &st) != 0) {
      return 1;
    }

    if (st & BIT(0)) {
      if (util_sys_inb(port, output) != 0) {
        return 1;
      }

      if (mouse && !(st & BIT(5))) {
        return 1;
      }
      if (!mouse && (st & BIT(5))) {
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attemps--;
  }
  return 1;
}



void(kbc_ih)() {
  read_controller(OUT_REG, &scancode, 0);
}

