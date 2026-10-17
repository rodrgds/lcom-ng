#include "keyboard.h"

uint8_t scancode = 0;
int keyboard_hook_id = 1;

int(keyboard_subscribe_int)(uint8_t *mask){
    if(mask == NULL) return 1;
    *mask = BIT(keyboard_hook_id);
    if(sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) return 1;
    return 0;
}

void(kbc_ih)(){
    if(read_KBC_keyboardOUTPUT(0x60, &scancode) != 0) printf("error while reading scancode");
}

int(keyboard_unsubscribe_int)(){
    if(sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;
    return 0;
}
