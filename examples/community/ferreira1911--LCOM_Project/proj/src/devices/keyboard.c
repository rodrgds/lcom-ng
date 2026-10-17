#include <lcom/lcf.h>

#include "keyboard.h"
#include "kbc.h"

int kbc_hook_id = 0;
uint8_t scancode;
bool scancode_ready = false; 

int (kbd_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    kbc_hook_id = *bit_no;
    
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook_id) != 0) return 1;

    return 0;
}

int (kbd_unsubscribe_int)() {
    if (sys_irqrmpolicy(&kbc_hook_id) != 0) return 1;

    return 0;
}

void (kbc_ih)(void) {
    uint8_t scancode_temp;

    if(read_kbc_cmd(&scancode_temp) != 0) return;

    scancode = scancode_temp;
    scancode_ready = true;
}


