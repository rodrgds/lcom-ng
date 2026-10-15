#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"


// read the status register
int(read_KBC_status) (uint8_t *status) {
  if(util_sys_inb(STATUS_REG, status) != 0) return 1;
  return 0;
}



/* READ KBC OUTPUT
we are going to read the output buffer of the KBC (for eg, to read which characters were pressed)
things to take into account:

1) kbc is slow,  our "driver" should not expect to get a response imm after issuing a command
   solution -> give enough time for the KBC to respond, retry a few times on time-out - we'll do 10 - and finally give up 
   (go here for more details: https://pages.up.pt/~up722898/aulas/lcom2425/lab3/lab3_04.html#sec4.1)

2) the info given by the i8042 is only reliable if it is completely on the buffer, 
   so we can only read it when the output buffer is full !!!!
   solution -> we now can read directly the status of the device
   status has 8 bits and gives us relevant info, including if the output buffer is full 

td;lr what are we going to do?
create a finite loop and wait a few miliseconds between fails 
(normally 10 attemps w/ 20ms delay is enough to have success)
remembering that the output buffer must be full 
*/  

int(read_KBC_output)(uint8_t port, uint8_t *output) {

    uint8_t status;
    int attempts = 10;

    while(attempts > 0) {

        //we retrieve the status of the KBC
        if (read_KBC_status(&status) != 0) {
            printf("Error reading status.\n"); 
            return 1;}
    
        //we check if the output buffer is full
        if ((status & OBF) && !(status & AUX)) {    //see slide 22
            
            //we read the output buffer
            if(util_sys_inb(port, output) != 0) {
                printf("Error reading output buffer.\n");
                return 1;
            }

            //we check for errors (parity and timeout)
            if ((status & PARITY) != 0) {
                printf("Parity error.\n");
                return 1;
            }

            if ((status & TIMEOUT) != 0) {
                printf("Timeout error.\n");
                return 1;
            }

            //sucess! we read the output without errors
            return 0;
        }
        
    
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;  
    }

    //if we surpass the number of attempts, we return an error
    return 1;
}




/* WRITE KBC COMMAND
we are going to write to the KBC (for eg, to enable the keyboard)
we need to have similar precautions when writing to the KBC

-> we still have to wait for the KBC to be ready to receive the command
-> but now we need to check if the input buffer is full before writing to it
*/

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
