#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"
#include <lcom/utils.h>

int kbd_id = KBD_IRQ;
uint8_t code = 0x00;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = kbd_id;

  sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id);

  return 0;
}

int(kbd_unsubscribe_int)() {
  sys_irqrmpolicy(&kbd_id);
  return 0;
}

void(kbc_ih)() {
  uint8_t control;
  code = 0x00;

  util_sys_inb(STAT_REG, &control);

  if ((control & STAT_REG_OBF) != 0 || (control & (STAT_REG_PARITY | STAT_REG_TIMEOUT)) == 0) {
    if (util_sys_inb(OUT_BUF, &code) != 0) {
      code = 0x00;
      return;
    }
  }
}
