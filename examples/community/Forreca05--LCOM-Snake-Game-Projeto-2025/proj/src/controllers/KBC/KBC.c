#include "KBC.h"
#include "i8042.h"
#include <stdio.h>

int (KBC_read_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (KBC_read_output)(uint8_t* output) {
    return util_sys_inb(KBC_OUT_CMD, output);
}

int (KBC_write_command)(uint8_t port, uint8_t command) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;
    
    while (attempts) {
        if (KBC_read_status(&status) != 0) {
            printf("ERROR: couldn't read status\n");
            return 1;
        }
        if ((status & FULL_IN_BUFFER) == 0) {
            sys_outb(port, command);
            return 0;
        } 
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }

    return 1;
}

int (KBC_get_byte)(uint8_t *output){
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts) {
        if (KBC_read_status(&status) != 0) {
            printf("ERROR: couldn't read status\n");
            return 1;
        }

        if (status & FULL_OUT_BUFFER) {
            // Mouse byte is read here
            if (KBC_read_output(output) != 0){
                printf("ERROR: couldn't read output\n");
                return 1;
            }

            if (status & PARITY_ERROR) {
                printf("ERROR: parity error\n");
                return 1;
            }

            if (status & TIMEOUT_ERROR) {
                printf("ERROR: timeout error\n");
                return 1; 
            }

            if ((status & NOT_INHIBITED) == 0) {
                printf("ERROR: keyboard inhibited\n");
                return 1;
            }
            
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    return 1;
}
