#include <lcom/lcf.h>

#include "keyboard.h"
#include "macro_kb_mouse.h"

uint8_t status, scancode;
int kb_ret_val = 0;
static int kb_hook_id = KB_IRQ;

int (kb_subscribe_int)(uint8_t *bit_no) {
    *bit_no = kb_hook_id;
    return sys_irqsetpolicy(KB_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kb_hook_id);
}

int (kb_unsubscribe_int)() {
    return sys_irqrmpolicy(&kb_hook_id);
}

int (kbc_read)() {
    while (1) {
        if (util_sys_inb(KBC_ST_REG, &status)) return 1;
        
        if (status & KBC_OBF) {
            if (!(status & (KBC_PAR_TO_ERROR))) {
                util_sys_inb(KBC_OUT_BUF, &scancode);
                return 0; 
            }
            else return 1;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }
}

void (kbc_ih)() {
    if (kbc_read()) {
        kb_ret_val = 1;
        return;
    }

    kb_ret_val = 0;
}
