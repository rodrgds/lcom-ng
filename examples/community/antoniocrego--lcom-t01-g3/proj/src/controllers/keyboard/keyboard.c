#include <lcom/lcf.h>
#include "keyboard.h"

int keyboard_hook_id = 1;

int (keyboard_subscribe_interrupts)(uint8_t* bit_no){
    if (bit_no==NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(1,IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
    return sys_irqrmpolicy(&keyboard_hook_id);
}
