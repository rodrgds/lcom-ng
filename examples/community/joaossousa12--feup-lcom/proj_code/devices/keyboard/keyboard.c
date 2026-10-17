#include "keyboard.h"


int keyboard_hook_id = 1;
uint8_t scancode=0;

int (keyboard_subscribe_interrupts)(){
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE|IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
    if(sys_irqrmpolicy(&keyboard_hook_id)) return 1;
    return 0;
}

void (kbc_ih)(){
    read_KBC_output(0x60,&scancode,0);
}

int (keyboard_restore)(){ //IMPORTANTE: 0x20: comando que avisa uma futura leitura; 0x60: avisa uma futura escrita; BIT(0): está ativo quando as interrupções estão ativas
    uint8_t configuration;
    if(write_KBC_command(0x64,0x20)) return 1;
    if(read_KBC_output(0x60,&configuration,0)) return 1;
    configuration = configuration | BIT(0);
    if(write_KBC_command(0x64,0x60)) return 1;
    if(write_KBC_command(0x60,configuration)) return 1;
    return 0;
}

