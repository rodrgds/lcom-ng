#include <lcom/lcf.h> 
#include "kbc.h"

int write_kbc_cmd(uint8_t cmd, uint8_t port){
    uint8_t response;

    for (int retries = 10; retries > 0; retries--) {

        if(util_sys_inb(KBC_STATUS_REGISTER, &response) != 0) return 1;

        if((response & KBC_STATUS_IB_FULL) == 0){
            if(sys_outb(port, cmd) != 0) return 1;
            return 0;
        };
        tickdelay(micros_to_ticks(KBC_DELAY));
    }
    return 1;
}

int read_kbc_cmd(uint8_t *byte){
    
    uint8_t response;

    for (int retries = 10; retries > 0; retries--) {
        
        if(util_sys_inb(KBC_STATUS_REGISTER, &response) != 0) return 1;
        
        if((response & KBC_STATUS_OB_FULL) != 0){
            if(response & KBC_STATUS_PARITY_ERR) return 1;
            if(response & KBC_STATUS_TIMEOUT_ERR) return 1;
            if(util_sys_inb(KBC_OUTPUT_BUFFER, byte) != 0) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(KBC_DELAY));
    }
    return 1;
}

