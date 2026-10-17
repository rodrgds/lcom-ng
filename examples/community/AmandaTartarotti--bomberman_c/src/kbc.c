#include <lcom/lcf.h>
#include <stdint.h>


#include "../inc/i8042.h"

int kbc_read(bool *error, uint8_t *scancode){
    uint8_t status;
    if(error == NULL) return 1;
    if(scancode == NULL) return 1;

    if(util_sys_inb(STAT_IN, &status) != 0) return 1;
    if(!(status & BIT(0))) return 1;
    if(status & BIT(7)){ *error = true; return 1;}
    if(status & BIT(6)){*error = true; return 1;}
    
    if(status & BIT(0)){
        if(util_sys_inb(OUT_BUF, scancode) != 0) {*error = true; return 1;}
        return 0;
    }
    return 1;
}
