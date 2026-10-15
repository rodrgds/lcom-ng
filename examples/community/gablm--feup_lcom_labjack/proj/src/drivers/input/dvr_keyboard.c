#include "input.h"

int hook_id_keyboard = 1;
uint8_t scancode = 0;

int kbd_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(hook_id_keyboard);

  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard);
}

int kbd_unsubscribe_int() {
  if (hook_id_keyboard < 0)
    return 1;

  return sys_irqrmpolicy(&hook_id_keyboard);
}

void (kbc_ih)() {
  kbc_read_output(OUT_BUF, &scancode, false);
}
