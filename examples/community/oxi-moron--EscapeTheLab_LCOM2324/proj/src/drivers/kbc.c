#include "kbc.h"

int kbd_hook_id = KBD_BIT_NO;
uint8_t scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(kbd_hook_id);
    return sys_irqsetpolicy(KBD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbd_hook_id);
}

void (kbc_ih)() {
    util_sys_inb(KBD_OUT_BUF, &scancode);
}
