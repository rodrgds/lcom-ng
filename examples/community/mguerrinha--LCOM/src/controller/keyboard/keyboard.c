#include "keyboard.h"

int kb_hook_id = 1;
uint8_t scancode = 0;
bool two_byte = false;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(kb_hook_id);
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_hook_id)) return 1;
  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&kb_hook_id)) return 1;
  return 0;
}

void (kbc_ih)() {
  read_KBC_output(KBC_OUT_CMD, &scancode, 0);
}
