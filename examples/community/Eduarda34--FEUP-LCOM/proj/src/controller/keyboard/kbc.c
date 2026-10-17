#include <lcom/lcf.h>
#include "kbc.h"
#include <stdio.h> // For printf
#include "i8042.h"
int (read_kbc_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}

int read_kbc_output_buffer(uint8_t port, uint8_t* out, uint8_t mouse) {
    uint8_t status;

            if (read_kbc_status(&status) != 0) {
                printf("Error: Status not available!\n");
                return 1;
            }

            if((status & FULL_OUT_BUFFER)==0){
                return 1;
            }

            if (mouse && !(status & FULL_MOUSE_BUFFER)) {
                printf("Error: Mouse output not found\n");
                return 1;
            }

            if (!mouse && (status & FULL_MOUSE_BUFFER)) {
                printf("Error: Mouse output found\n");
                return 1;
            }

            if (util_sys_inb(port, out) != 0) {
                printf("Error: Could not read output!\n");
                return 1;
            }

            if ((status & PARITY_ERROR) != 0) {
                printf("Error: Parity error!\n");
                return 1;
            }

            if ((status & TIMEOUT_ERROR) != 0) {
                printf("Error: Timeout error!\n");
                return 1;
            }
            
            return 0;
}

int (write_kbc_command)(uint8_t port, uint8_t commandByte){
    uint8_t status;
    uint8_t attempts = 10;
    while(attempts > 0){
        if (read_kbc_status(&status) != 0){
            printf("Error: Status not available!\n");
            return 1;
        }
        if((status & FULL_IN_BUFFER) == 0){
            if(sys_outb(port, commandByte)!= 0){
                printf("Error: commandByte!\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    return 1;
}
