#include <lcom/lcf.h>
#include "keyboard.h"

#include <stdint.h>

#include <minix/sysutil.h>

extern uint8_t kbc_data;
int kbc_hook_id = 0;


int (kbc_subscribe_int)(uint8_t *bit_no){
    kbc_hook_id = KBC_IRQ;
    *bit_no = kbc_hook_id;
    if(sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&kbc_hook_id) != 0){
        printf("sys_irqsetpolicy error!\n");
        return 1;
    }
    return 0;
}

int (kbc_unsubscribe_int)(){
    if( sys_irqrmpolicy(&kbc_hook_id) != 0){
        printf("sys_irqrmpolicy error!\n");
        return 1;
    } 
    return 0;
}

void (kbc_ih)(){
    uint8_t stat;
    util_sys_inb(KBC_STATUS_REG, &stat);

    if((stat & (ST_PARITY | ST_TIMEOUT))==0) 
    {
        if((stat & ST_OBF) == 1)
        {
            util_sys_inb(OUT_BUFFER, &kbc_data);

        }
    }
}

int (kbc_write_command)(uint8_t cmd,uint8_t command){
    unsigned counter = 0;
    uint8_t stat;
    while(counter < 10) {
        if(util_sys_inb(KBC_STATUS_REG, &stat) !=0 ){
            printf("ERROR in utils_sys_inb()\n");
            return 1;
        }
        if((stat & ST_IBF) == 0 ) {
            if(sys_outb(KBC_COMAND_REG, cmd) == 0){
                sys_outb(IN_BUFFER, command);
                return 0;
            }
            else {
                printf("ERROR in utils_sys_inb()\n");
                return 1;
            }
        }
        tickdelay(micros_to_ticks(DELAY));
        counter++;
    }
    printf("Error while trying to write command\n");
    return 1;
}

int (kbc_read_command)(uint8_t cmd,uint8_t * command){
    unsigned counter = 0;
    uint8_t stat;
    while(counter < 10) {
        if(util_sys_inb(KBC_STATUS_REG, &stat)!=0){
            printf("ERROR in utils_sys_inb()\n");
            return 1;
        }
        if((stat & ST_IBF) == 0) {
            if(sys_outb(KBC_COMAND_REG, cmd ) == 0){
                if(util_sys_inb(OUT_BUFFER, command) != 0){
                    printf("Error in util_sys_inb()\n");
                    return 1;
                }
                return 0;
            }
            else {
                printf("ERROR in utils_sys_inb()\n");
                return 1;
            }
        }
        tickdelay(micros_to_ticks(DELAY));
        counter++;
    }
    printf("Error while trying to read command\n");
    return 1;
}

int (kbc_get_value)(){
    unsigned counter = 0;
    uint8_t stat;
    while( counter < 10 ) {
        if( util_sys_inb(KBC_STATUS_REG, &stat) != 0){
            printf("ERROR in utils_sys_inb()\n");
            return 1;
        }
        if((stat & ST_OBF) == 1 && (stat & ST_AUX) == 0) { // ass. it returns OK 
            if ( (stat & (ST_PARITY | ST_TIMEOUT)) == 0 )
            {
                if( util_sys_inb(OUT_BUFFER, &kbc_data) !=0){
                    printf("ERROR in utils_sys_inb()\n");
                    return 1;   
                }
                return 0;
            }

        }
        tickdelay(micros_to_ticks(DELAY)); // e.g. tickdelay()
        counter++;
    }
    return 1;
}
