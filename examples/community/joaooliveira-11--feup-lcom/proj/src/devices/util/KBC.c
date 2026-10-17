#include "KBC.h"

int(reading_KBC_STATE)(uint8_t * state){
    if(util_sys_inb(0x64, state) != 0) return 1;
    return 0;
}

int(write_KBC_INPUT)(uint8_t port, uint8_t kbc_command){
    uint8_t KBC_state;
    uint8_t write_attemps = 10;

    for(uint8_t i = 0; i <= write_attemps; i++){
        if(reading_KBC_STATE(&KBC_state) != 0) return 1;
        if(!(KBC_state & BIT(1))){
            if(sys_outb(port, kbc_command) != 0) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
    } 
    return 1;
}

int(read_KBC_keyboardOUTPUT)(uint8_t port, uint8_t * kbc_output){
    uint8_t KBC_state;
    uint8_t read_attemps = 10;

    for(uint8_t i = 0; i <= read_attemps; i++){

        if(reading_KBC_STATE(&KBC_state) != 0) return 1;

        if(KBC_state & BIT(0)){
            if(util_sys_inb(port, kbc_output) != 0) return 1;
            if(KBC_state & (BIT(6) | BIT(7) | BIT(5))) return 1;
            return 0;
        }

        tickdelay(micros_to_ticks(20000));
    }
    return 1;
}

int(read_KBC_mouseOUTPUT)(uint8_t port, uint8_t * kbc_output){
    uint8_t KBC_state;
    uint8_t read_attemps = 10;

    for(uint8_t i = 0; i <= read_attemps; i++){

        if(reading_KBC_STATE(&KBC_state) != 0) return 1;

        if(KBC_state & BIT(0)){
            if(util_sys_inb(port, kbc_output) != 0) return 1;
            if(( KBC_state & (BIT(6) | BIT(7)) ) || (!(KBC_state & BIT(5))) ) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
    }
    return 1;
}

