#include "kbd.h"

int kbd_hook_id = 1;
uint8_t scancode;

int (kbd_subscribe_int)(uint8_t *bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(kbd_hook_id);
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK) return 1;
    return 0;
}

int (kbd_unsubscribe_int)(){
    if (sys_irqrmpolicy(&kbd_hook_id) != OK) return 1;
    return 0;
}

void (kbc_ih)(void){
    if (KBC_read_output(KBD_OUT_BUF, &scancode) != 0){
        printf("Error reading output\n");
    }
}
