#include <keyboard.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"

int hook_idK=2;
uint8_t statusCode=0x0;
uint8_t command;

int (keyboard_subscribe)(uint8_t *bit_no, int hook) {
    hook_idK=hook;
    *bit_no=BIT(hook_idK);
    return sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_idK);
}

int (keyboard_unsubscribe)() {
    return sys_irqrmpolicy(&hook_idK);
}
int size=0;
void (kbc_ih)(){
    if(done) size = 0;
    uint8_t byte;
    if (util_sys_inb(STATUS_REG, &statusCode)) return;
    if((statusCode & PARITY_BIT) || (statusCode & TOB)){
        printf("error");
        return;
    }
        if ((statusCode & OBF) == 0 || (statusCode & MOUSE_AUX) != 0) {
        return;
    }
    if (util_sys_inb(OUT_BUF, &byte)) return;
    scanCode[size]=byte;
    if(TWO_BYTE == byte){
      done=0;
      size++;
    }
    else{
      done=1;
      size++;
    }
}


