#include "keyboard.h"


int (kbc_subscribe_int)(uint8_t *bit_no){
    
    kb_hook_id = *bit_no;
    
    return sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE|IRQ_EXCLUSIVE), &kb_hook_id);
}

int (kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&kb_hook_id);
}

void (kbc_ih)(){
    uint8_t st;

    for (int counter = 0; counter < 3; counter++){
        
        if ((util_sys_inb(KBC_ST_REG, &st)) != 0){
            found_err = 1;
            return;
        }

        if (st & KBC_ST_OBF){
            
            if (st & KBC_ERR){
                found_err = 1;
                return;
            }
            
            if ((util_sys_inb(KBC_OUT_BUF, &kb_data)) != 0){
                found_err = 1;
            }

            return;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }

    data_read_to = 1;
}

int (kbc_issue_cmd)(uint8_t kbc_cmd){

    uint8_t st;

    for (int i = 0; i < 3; i++) {
        
        if ((util_sys_inb(KBC_ST_REG, &st)) != 0) return 1;

        if( (st & KBC_ST_IBF) == 0 ){ 
            
            if ((sys_outb(KBC_CMD_REG, kbc_cmd)) != 0) return 1;
            
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1;
}
