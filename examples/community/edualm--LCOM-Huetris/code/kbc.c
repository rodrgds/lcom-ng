#include "kbc.h"

#include <minix/syslib.h>
#include <minix/drivers.h>

int keyboard_read(void) {
    unsigned long status, read_data;
    
    unsigned int tries;
    
    for (tries = 0; tries < KBC_MAX_TRIES; tickdelay(micros_to_ticks(DELAY_US)), tries++) {
        if (sys_inb(KBC_REG_STATUS, &status))
            return KBC_ERROR_RET;
        
        if ((status & (KBC_BIT_OBF | KBC_BIT_AUX))) {
            if (sys_inb(KBC_REG_DATA, &read_data)) {
                return KBC_ERROR_RET;
            }
            
            if (!(status & (KBC_BIT_PAR_ERR | KBC_BIT_TO_ERR)))
                return read_data;
            
            continue;
        }
    }
    
    return KBC_ERROR_RET;
}

int keyboard_write(unsigned long port, unsigned char input) {
    unsigned long status, read_data;
    
    unsigned int tries;
    
    for (tries = 0; tries < KBC_MAX_TRIES; tickdelay(micros_to_ticks(DELAY_US)), tries++) {
        if (sys_inb(KBC_REG_STATUS, &status)) {
            return KBC_ERROR_RET;
        }
        
        if (status & KBC_BIT_IBF) {
            continue;
        }
        
        return sys_outb(port, input);
    }
    
    return KBC_ERROR_RET;
}
