#include "keyboard.h"

extern uint32_t counter_KBC;
uint8_t scancode = 0;
int hook_id_kbd = 1;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id_kbd);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
}

int (keyboard_unsubscribe_interrupts)() {
    return sys_irqrmpolicy(&hook_id_kbd);
}



void (kbc_ih)() {
    uint8_t status;

    if (util_sys_inb(KBC_STATUS_REG, &status) != 0)
        return;

        if ((status & FULL_OUT_BUFFER) &&
        !(status & (PARITY_ERROR | TIMEOUT_ERROR)) &&
        !(status & BIT(5))) {

        util_sys_inb(KBC_OUT_CMD, &scancode);
    }
}





int (keyboard_restore)() {
    uint8_t commandByte;

    if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD) != 0) return 1;          
    if (read_KBC_output(KBC_OUT_CMD, &commandByte, 0) != 0) return 1; 

    commandByte |= ENABLE_INT;  

    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;    
    if (write_KBC_command(KBC_WRITE_CMD, commandByte) != 0) return 1;

    return 0;
}
