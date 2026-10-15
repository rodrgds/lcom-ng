#include "keyboard.h"

uint8_t scancode = 0;
int hook = 1;
bool has_e0 = false;

// subscribes keyboard interrupts using the hook_id
int(keyboard_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    *bit_no = BIT(hook);
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

// unsubscribes from keyboard interrupts
int(keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook);
}


// keyboard interrupt handler. reads the scancode from the KBC output buffer. if the read operation fails, the scancode is set to 0.
void(kbc_ih)() {
    if(read_KBC_output(0x60, &scancode, 0) != 0) {
        scancode = 0;
        return;
    }
}

// function that restores the keyboard's configuration to enable interrupt generation
int(keyboard_restore)() {
    uint8_t commandbyte;

    if(write_KBC_command(0x64, 0x20) != 0) return 1;
    if(read_KBC_output(0x60, &commandbyte, 0) != 0) return 1;
    
    commandbyte |= BIT(0); // Enable interrupts

    if(write_KBC_command(0x64, 0x60) != 0) return 1;
    if(write_KBC_command(0x60, commandbyte) != 0) return 1;

    return 0;
}
