#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"


uint8_t scancd=0;
int kbd_hook_id = 1;

int(keyboard_subscribe)(uint8_t* bit_no){
    if(bit_no==NULL){return 1;}
    *bit_no = BIT(kbd_hook_id);
    return sys_irqsetpolicy(IRQ_KEYBOARD,IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id );
}

int(keyboard_unsubscribe)(){
    return sys_irqrmpolicy(&kbd_hook_id);
}

void(kbc_ih)(){
    if(read_KBC_out(KBC_OUT_CMD, &scancd, false)!=0){
        printf("ERROR Couldn't read the scancode! \n");
    }
}

