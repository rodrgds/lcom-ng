#include <lcom/lcf.h>

#include "i8042.h"
#include "kbd.h"

uint8_t scancode = 0;
int KBD_hook_id = 0;

//getters =============================================================
uint8_t (get_scancode)(){
    return scancode;
}

int (get_KBD_hook_id)(){
    return KBD_hook_id;
}

//====================================================================


void (kbc_ih)() {
    if(kbd_read_scancode() != 0){
        scancode = 0;
        printf("Error in the scancode reading \n");
    }

}

int (kbd_read_status)(uint8_t *status){

    if(util_sys_inb(KBC_ST_REG, status) != 0){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int (kbd_read_scancode)(){
    uint8_t value = 0;
    uint8_t status = 0; 
    int tries = 20; //avoids the program to block

    while( tries ) {

        if(kbd_read_status(&status) != 0){
            return EXIT_FAILURE;
        }

        /* loop while 8042 input buffer is not empty */
        if(status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)){
            return EXIT_FAILURE;
        }

        if(status & KBC_OBF) {
            if(util_sys_inb(KBC_OUT_BUF, &value) != 0){
                return EXIT_FAILURE;
            }

            scancode = value;
            return EXIT_SUCCESS;
        }

        tickdelay(micros_to_ticks(DELAY_US));// e.g. tickdelay()
        tries--; 
    }

    return EXIT_FAILURE;
    
}

int (kbd_subscribe_int)(uint8_t *bit_no){
    *bit_no = BIT(1); 
    KBD_hook_id = *bit_no;

    if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &KBD_hook_id) != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int (kbd_unsubscribe_int)(){
    if(sys_irqrmpolicy(&KBD_hook_id) != 0){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//=================
void set_scancode(uint8_t fake_scancode) {
    scancode = fake_scancode;
}
