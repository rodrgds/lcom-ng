#include "KBC.h"


int (read_status)(uint8_t* status){
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_KBC_out)(uint8_t port, uint8_t *out, bool mouse){
    if(out==NULL)return 1;
    uint8_t status;
    uint8_t attemps=10;

    while(attemps){
        if(read_status(&status)!=0){
            printf("Error in status availability! \n");
            return 1;
        }

        if((status & FULL_OUT_BUFFER) !=0){

            if(util_sys_inb(port,out)!=0){
                printf("Could not read output! ERROR \n");
                return 1;
            }
            
            if((status & PARITY_ERROR) !=0){
                printf("Parity error!\n");
                return 1;
            
            }
            if((status & TIMEOUT_ERROR)!=0){
                printf("Timeout ERROR!\n");
                return 1;
            } 

            if((mouse && !(status & MOUSE_DATA))!=0){
                printf("No mouse data!\n");
                return 1;
            }

            if((!mouse && (status & MOUSE_DATA))!=0){
                printf("No keyboard data!\n");
                return 1;
            }
            
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attemps--;
    }
    
    return 1;
}

int(write_KBC)(uint8_t port, uint8_t command){
    uint8_t status;
    uint8_t attemps = 10;

    while (attemps){
        if(read_status(&status)!=0){
            printf("Error in status availability! \n");
            return 1; 
        }
        if((status & FULL_IN_BUFFER)==0){
            if(sys_outb(port,command)!=0){
                printf("ERROR: coundn't write command!\n");
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attemps--;
    }
    return 1;
}
