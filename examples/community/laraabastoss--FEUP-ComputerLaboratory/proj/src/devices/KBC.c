#include "KBC.h"

int (write_command_KBC)(uint8_t reg, uint8_t command) {
    uint8_t status; 
    uint8_t tries = 10;
    while (tries) {
        if (util_sys_inb(0x64, &status)) return 1;
        if (!(status & BIT(1)) ){
            return (sys_outb(reg, command));
        }
        tries--;
        tickdelay(micros_to_ticks(20000));
    } 
    return 1;
}

int get_output_KBC(uint8_t reg, uint8_t *output, uint8_t mouse) {
    uint8_t status; 
    uint8_t tries = 10;
    while (tries) {
        if (util_sys_inb(0x64, &status)) return 1;
        if ((status & BIT(0))) {                      
            if (util_sys_inb(reg, output)) return 1;
            if ((mouse && !(status & BIT(5))) || (!mouse && (status & BIT(5)))) return 1; 
            if((status & BIT(7)) || (status & BIT(6)) ) return 1;
            return 0; 
        }
        tries--;
        tickdelay(micros_to_ticks(20000));
    }
    return 1;
}
