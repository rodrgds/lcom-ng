#include "../include/kbd.h"

int hook_id_kbd = 1;
uint8_t scancode;

void (kbc_ih)(void) {
  uint8_t st;

  if (util_sys_inb(STATREG, &st)) return;

  if (st & PAR_ERROR || st & TIMEOUT) {
    util_sys_inb(OBF, &scancode);
    scancode = 0X00;
    return;
  }
  if (util_sys_inb(OBF, &scancode)) return;
}

int (kbd_subscribe)(uint8_t *bit_no) {
  *bit_no = hook_id_kbd; 

  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != OK) return 1;

  return 0;
}

int (kbd_unsubscribe)() {
  
  if (sys_irqrmpolicy(&hook_id_kbd)) return 1;
  
  return 0;
}
