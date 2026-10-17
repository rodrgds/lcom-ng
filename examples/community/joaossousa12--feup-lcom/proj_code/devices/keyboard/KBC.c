#include "KBC.h"
#include <stdio.h>

int (read_KBC_status)(uint8_t *status){
    return util_sys_inb(0x64,status);
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte){
    uint8_t attempts = 10;
    uint8_t status;
    while(attempts){
        if(read_KBC_status(&status)){
            return 1;
        }
        if(!(status & BIT(1))){
            if(sys_outb(port,commandByte)) return 1;
            return 0;
        }
        printf("Slowing down!");
        tickdelay(micros_to_ticks(20000));
        printf("%d", attempts);
        attempts--;
    }
    return 1;
}

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse){
    uint8_t attempts=10;
    uint8_t status;
    while(attempts){
        if(read_KBC_status(&status)) return 1;
        if(status & BIT(0)){
            if(util_sys_inb(port,output)) return 1;
            if(status & (BIT(6)|BIT(7))) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    return 1;
}
