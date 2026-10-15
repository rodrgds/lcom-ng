#include "KBC.h"


int (read_KBC_status)(uint8_t* status){
    if(status==NULL) return 1; //check if the pointer is valid

    return util_sys_inb(KBC_ST_REG, status);
}

int (read_KBC_output)(uint8_t *output, bool mouse){
    if(output==NULL) return 1; //check if the pointer is valid

    uint8_t status;
    int attempts_left = MAX_ATTEMPS;

    while (attempts_left>0){
        if(read_KBC_status(&status)) return 1; //error checking status

        if((status & OUTPUT_BUF_FULL)){ //if the output buffer is full, we try to read the scancode
            //need to read output buffer even if there is an error
            if(util_sys_inb(KBC_OUT_BUF, output)) return 1; //if there is an error reading the output
            
            if(mouse && !(status & MOUSE_DATA)) continue; //if we are looking for mouse data and it is not, try again
            if(!mouse && (status & MOUSE_DATA)) continue; //if we are looking for kb data and it is mouse, try again

            if((status & INVALID_DATA) || (status & TIMEOUT_ERROR)) return 1; //error identifiable by status word
            return 0; //we read the scancode
        }

        //the output is not ready to be read
        tickdelay(micros_to_ticks(DELAY)); //we wait the delay time
        attempts_left--; //minus one attempt
    }

    return 1; //tried 10 times and could not read the output
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte){
    uint8_t status;
    int attempts_left = MAX_ATTEMPS;

    while(attempts_left>0){
        if(read_KBC_status(&status)) return 1; //error checking status

        if((status & INPUT_BUF_FULL)==0){ //the input buffer is empty, so we can write
            if(sys_outb(port,commandByte)) return 1; //error writing command

            return 0; //we were able to write the command
        }

        tickdelay(micros_to_ticks(DELAY)); //we wait the delay time
        attempts_left--; //minus one attempt
    }

    return 1; //tried 10 times and could not read the output
}
