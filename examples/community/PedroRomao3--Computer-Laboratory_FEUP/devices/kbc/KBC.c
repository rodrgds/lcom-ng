#include "KBC.h"

int (fetch_kbc_status)(uint8_t *kbc_status){
    if(kbc_status == NULL) return 1;
    return util_sys_inb(KBC_STATUS_REG, kbc_status);
}

int (execute_KBC_command)(uint8_t port, uint8_t command){
    int num_attempts = NUMBER_ATTEMPS;
    uint8_t kbc_status;

    while(num_attempts){
        if(fetch_kbc_status(&kbc_status)){
            printf("Error: Status read error.\n");
            return 1;
        }
        if((kbc_status & FULL_IN_BUFFER) == 0){
            if(sys_outb(port, command)){
                //uint8_t bin=command;printBinary(bin);printf("Error: cannot write command to port: %hhu\n", port);
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        num_attempts--;
    }
    return 1;
}

int check_for_status_errors(uint8_t kbc_status){
    if(kbc_status & TIMEOUT_ERROR){
        printf("Error: timeout error, status: %hhu\n", kbc_status);
        return 1;
    } 
    if(kbc_status & PARITY_ERROR){
        printf("Error: parity error, status: %hhu\n", kbc_status);
        return 1;
    }
    return 0;
}

int verify_kbc_output(uint8_t kbc_status, uint8_t is_mouse){
    if(!is_mouse && (kbc_status & BIT(5))){
        printf("Error: read mouse, status: %hhu\n", kbc_status);
        return 1;
    }
    if(is_mouse && !(kbc_status & BIT(5))){
        printf("Error: read keyboard, status: %hhu\n", kbc_status);
        return 1;
    }
    return 0;
}

int fetch_output(uint8_t port, uint8_t *output){
    if(util_sys_inb(port, output)){
        printf("Error: Output read error from port: %hhu\n", port);
        return 1;
    }
    return 0;
}

int (get_KBC_out)(uint8_t port, uint8_t *output, uint8_t is_mouse){
    int attempts = NUMBER_ATTEMPS;
    uint8_t kbc_status;

    while(attempts){
        if(fetch_kbc_status(&kbc_status)){
            printf("Error: Status read error.\n");
            return 1;
        }
        if(kbc_status & ENABLE_INT){
            if(fetch_output(port, output)) return 1;
            if(check_for_status_errors(kbc_status)) return 1;
            if(verify_kbc_output(kbc_status, is_mouse)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    printf("Failed to read output after 10 attempts over 20ms\n");
    return 1; 
}
