#include "keyboard.h"

int hook_id_keyboard = 1;
uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
    if(bit_no == NULL) return 1;

    *bit_no=BIT(hook_id_keyboard);

    return sys_irqsetpolicy(IRQ_KEYBOARD, (IRQ_REENABLE|IRQ_EXCLUSIVE), &hook_id_keyboard);
}

int (keyboard_unsubscribe_interrupts)(){
    return sys_irqrmpolicy(&hook_id_keyboard);
}

void (kbc_ih)(){
    read_KBC_output(KBC_OUTBUFFER, &scancode, 0);
}
