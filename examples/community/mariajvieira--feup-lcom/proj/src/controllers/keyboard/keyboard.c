#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"


uint8_t scancode = 0;
int kbd_hook_id = 1;


void (kbc_ih)() {
  uint8_t status;
  util_sys_inb(STAT_REG, &status);
  if (!(status & OBF) || (status & (PAR_ERR | TO_ERR)))
    return;
  util_sys_inb(OUT_BUF, &scancode);
}

int (kbd_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;
  kbd_hook_id = *bit_no;

  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)!=0) return 1;
  return 0;
}

int (kbd_unsubscribe)() {
    return sys_irqrmpolicy(&kbd_hook_id);
}


