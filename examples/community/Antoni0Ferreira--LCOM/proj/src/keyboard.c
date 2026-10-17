#include "keyboard.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <lcom/utils.h>

#define DELAY_US 20000

int hook_keyboard_ID = 0x01;

uint8_t scanCode;

int (keyboard_subscribe_int)(uint8_t *bit_no){
    
    *bit_no = hook_keyboard_ID;
    if(sys_irqsetpolicy(1,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_keyboard_ID) != 0) return 1;
    return 0;
}

int (keyboard_unsubscribe_int)(){

    if(sys_irqrmpolicy(&hook_keyboard_ID) != 0) return 1;
    return 0;
}

void (kbc_ih)(){
    uint8_t data = read_from_kbc();
    if(data != 0) scanCode = data;
}

uint8_t (read_from_kbc)(){ 

    uint8_t statusRegister, data;
    while(1){
        util_sys_inb(KBC_ST_REG,&statusRegister);
        if(statusRegister & OBF){
            util_sys_inb(KBC_OUT_BUF,&data);
            if((statusRegister & (PAR_ERROR | TIME_ERROR)) == 0)
                return data;
            else
                return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

uint8_t (issue_command)(uint8_t command){
    uint8_t status;

    while(1){
        util_sys_inb(KBC_ST_REG,&status);
        if((status & IBF) == 0){
            sys_outb(KBC_CMD_REG,command);
            return 0;
        } 
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

uint8_t (issue_arg)(uint8_t arg){
    uint8_t status;
    while(1){
        util_sys_inb(KBC_ST_REG, &status);
        if((status & IBF) == 0){
            sys_outb(KBC_ARG_REG, arg);
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}
