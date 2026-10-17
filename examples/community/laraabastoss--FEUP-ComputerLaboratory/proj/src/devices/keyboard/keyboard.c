#include "keyboard.h"

int hook_id_kbd = 1;
uint8_t scancode;


int (keyboard_subscribe_interrupts)() {
    if (sys_irqsetpolicy(KEYBOARDIRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd )) return 1;
    return 0;
}

int (keyboard_unsubscribe_interrupts)() {
    if (sys_irqrmpolicy(&hook_id_kbd)) return 1;
    return 0;
}

void (keyboard_interrupt_handler)() {
    get_output_KBC(0x60, &scancode, 0);
}


