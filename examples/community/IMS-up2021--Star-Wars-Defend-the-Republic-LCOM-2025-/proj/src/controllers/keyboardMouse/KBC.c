#include "KBC.h"


int (read_KBC_status)(uint8_t* status){
    return util_sys_inb(KBC_STAT_REG, status);
}

int (kbc_write_byte)(uint8_t port, uint8_t byte_to_write) {
    uint8_t status;
    for (int i = 0; i < MAX_ATTEMPS; i++) {
        if (read_KBC_status(&status) != 0) return 1;

        if ((status & FULL_IN_BUF) == 0) { // FULL_IN_BUF é BIT(1)
            if (sys_outb(port, byte_to_write) != 0) return 1;

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));

    }
    return 1;
}

int (read_kbc_out)(uint8_t port, uint8_t *cmdByteOut, uint8_t mouse) {

    uint8_t status;
    uint8_t attemps = MAX_ATTEMPS;

    while (attemps){
        if(read_KBC_status(&status) != 0) {
            printf("Error: status read failed\n");
            return 1;
        }

        if((status & BIT(0)) != 0) {
            if(util_sys_inb(port, cmdByteOut) != 0) {
                printf("Error: output read failed\n");
                return 1;
            }
            if((status & ERR_PARITY) != 0) {
                printf("Error: Parity error\n");
                return 1;
            }
            if((status & ERR_TIMEOUT) != 0) {
                printf("Error: Timeout error\n");
                return 1;
            }
            if (mouse && !(status & BIT(5))) {              
                printf("Error: Mouse output not found\n");  
                return 1;
            } 
            if (!mouse && (status & BIT(5))) {                
                printf("Error: Keyboard output not found\n"); 
                return 1;
            } 
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attemps--;
    }
    return 1;    
}

