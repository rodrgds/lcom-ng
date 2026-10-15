
#include <lcom/lcf.h>

#include <stdint.h>

#include "KBC.h"

int (get_KBC_status)(uint8_t *status){
    return util_sys_inb(KBC_ST_REG, status);
}

int (check_KBC_status)(uint8_t status){
    return (status & (KBC_PARITY_ERR | KBC_TIMEOUT_ERR));
}

int (write_command)(int port, uint8_t command){
   
    uint8_t status;
    int attempts = 10;

    while (attempts--){
        
        if (get_KBC_status(&status) ) 
            return 1;
        
        if (check_KBC_status(status) ) 
            return 1;
        
        if ((status & KBC_IBF) == 0)
            return sys_outb(port, command);
        
        WAIT_KBC;
    }
    return 1;
}

int (read_output)(int port, uint8_t *output, bool keyboard){

    uint8_t status;
    int attempts = 10;

    while (attempts--){
        
        if (get_KBC_status(&status) ) 
            return 1;
        
        if (check_KBC_status(status) ) 
            return 1;
        
        if (status & KBC_OBF){

            if (util_sys_inb(port, output) ) return 1;

            if (keyboard ^ (status & KBC_AUX)) 
                return 0;
            else 
                return 1;
        }
    
        WAIT_KBC;
    }
    return 1;
}
