#include "KBC.h"

/**
 * @brief Reads the status of the KBC
 * @param status Pointer to the variable where the status will be stored
 * @return 0 if successful, 1 otherwise
 */
int (KBC_read_status)(uint8_t* status) {
    if(status == NULL) return 1;
    return util_sys_inb(KBC_STATUS_REG, status);
}

/**
 * @brief Reads the output buffer of the KBC
 * @param port Port to read from
 * @param output Pointer to the variable where the output will be stored
 * @param mouse 1 if the output is from the mouse, 0 if it's from the keyboard
 * @return 0 if successful, 1 otherwise
 */
int (KBC_read_output)(uint8_t port, uint8_t *output, uint8_t mouse) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;
    
    while (attempts) {

        if (KBC_read_status(&status)) {
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & BIT(0))) {
            if(util_sys_inb(port, output)){
                printf("Error: Could not read output!\n");
                return 1;
            }
            if((status & BIT(7))){
                printf("Error: Parity error!\n");           
                return 1;
            }
            if((status & BIT(6))){
                printf("Error: Timeout error!\n");          
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
        attempts--;
    }
    return 1;
}

/**
 * @brief Writes a command to the KBC
 * @param port Port to write to
 * @param commandByte Command to be written
 * @return 0 if successful, 1 otherwise
 */
int (KBC_write_command)(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts) {

        if (KBC_read_status(&status)){
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & FULL_IN_BUFFER) == 0){

            if(sys_outb(port, commandByte)){
                printf("Error: Could not write commandByte!\n");
                return 1;
            }

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    
    return 1;
}
