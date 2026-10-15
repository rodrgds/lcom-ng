#include "keyboard.h"
#include "../i8042.h"
#include "../KBC.h"

uint8_t scancode;
int hook_id = 2;

// Subscribes the KBC interrupts
int (keyboard_subscribe_int)(uint8_t *irq_set) {
    if (irq_set == NULL) return 1;
    *irq_set = BIT(hook_id);
    if (sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) return 1;
    return 0;
}

// Unsubscribes the KBC interrupts
int (keyboard_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id) != 0) return 1;
    return 0;
}

// Input handler
void (kbc_ih)() {
    KBC_get_byte(&scancode);
}
