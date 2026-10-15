#include <lcom/lcf.h>
#include "keyboard.h"

#include <stdint.h>

#include "i8042.h"

static int hook_id = 1;
uint32_t count = 0;
uint8_t scan_code = 0;

void (kbc_ih)(){
    uint8_t st;
    
    for(int count = 0; count < 3; count++){

        if(util_sys_inb(STATUS_REG, &st) != 0){
            printf("Error: Status not available!\n");
            return;
        }

        if((st & BIT(0)) != 0){

            if(util_sys_inb(OUT_BUF, &scan_code) != 0){
                return;
            }

            if(check_status(st) == OK) {

                return;
            }
            else {
                printf("Error: Status not OK!\n");
                return;
            }

            return;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }
}

/* int (check_status) (uint8_t st){
    if(st & PARITY)
        return PARITY_ERR;

    if(st & TIME_OUT) 
        return TIME_OUT_ERR;

    if(st & AUX)
        return AUX_DATA;

    return OK;
} */



int (keyboard_subscribe_int) (uint8_t *bit_n){

    *bit_n = hook_id;

    return sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id);
}

int (keyboard_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}
