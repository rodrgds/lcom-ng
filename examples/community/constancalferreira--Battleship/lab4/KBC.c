#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"


// read the status register
int(read_KBC_status) (uint8_t *status) {
  if(util_sys_inb(STATUS_REG, status) != 0) return 1;
  return 0;
}



/* READ KBC OUTPUT*/
//new -> boolean to see if the reading is from mouse


int(read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse) {

    uint8_t status;
    int attempts = 10;

    while(attempts > 0) {

        //we retrieve the status of the KBC
        if (read_KBC_status(&status) != 0) {
            printf("Error reading status.\n"); 
            return 1;
        }
    
        //we check if the output buffer is full
        if ((status & OBF)) {    
            
            //we check for errors (parity and timeout)
            if ((status & PARITY) != 0) {
                printf("Parity error.\n");
                return 1;
            }

            if ((status & TIMEOUT) != 0) {
                printf("Timeout error.\n");
                return 1;
            }

            if (!mouse && (status & AUX)) {
                printf("Not expecting mouse input response but received it.\n");
                return 1;
            }

            if (mouse && !(status & AUX)) {
                printf("Expecting mouse input response but didn't receive it.\n");
                return 1;
            }

            //we read the output buffer
            if(util_sys_inb(port, output) != 0) {
                printf("Error reading output buffer.\n");
                return 1;
            }

            //sucess! we read the output without errors
            return 0;
        }
        
    
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;  
    }

    //if we surpass the number of attempts, we return an error
    printf("Output buffer not full after 10 attempts.\n");
    return 1;
}




/* WRITE KBC COMMAND*/

int (write_KBC_cmd)(uint8_t port, uint8_t commandByte) {

    uint8_t status;
    int attempts = 10;

    while(attempts > 0) {

        //we retrieve the status of the KBC
        if (read_KBC_status(&status) != 0) {
            printf("Error reading status.\n"); 
            return 1;}
    
        //we check if the input buffer is not full
        if ((status & IBF) == 0) {
            
            //we write the command to the KBC
            if(sys_outb(port, commandByte) != 0) {
                printf("Error writing command.\n");
                return 1;
            }

            //sucess! we wrote the command without errors
            return 0;
        }
        
    
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;  
    }

    //if we surpass the number of attempts, we return an error
    return 1;
}
