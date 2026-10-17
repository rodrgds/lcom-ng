#include <lcom/lcf.h>

#include <stdint.h>

#include "../inc/i8042.h"
#include "../inc/kbc.h"

int kbd_id = 1;

int kbd_subscribe_int(uint8_t *bit_no){
    if(bit_no == NULL) return 1;
    *bit_no = BIT(kbd_id);
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_id);
}
int kbd_unsubscribe_int(){
    return sys_irqrmpolicy(&kbd_id);
}

bool error=false;
uint8_t scancode=0x00;

void (kbc_ih)(){
    if(kbc_read(&error, &scancode) != 0) printf("Error while reading scancode! \n");
}
