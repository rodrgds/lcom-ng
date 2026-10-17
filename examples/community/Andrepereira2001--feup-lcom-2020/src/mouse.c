#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>

#include <minix/sysutil.h>

extern uint8_t mouse_data;
int mouse_hook_id = 0;


int (mouse_subscribe_int)(uint8_t *bit_no){
    mouse_hook_id = MOUSE_IRQ;
    *bit_no = mouse_hook_id;
    if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&mouse_hook_id) != 0){
        printf("sys_irqsetpolicy error!\n");
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)(){
    if( sys_irqrmpolicy(&mouse_hook_id) != 0){
        printf("sys_irqrmpolicy error!\n");
        return 1;
    } 
    return 0;
}

void (mouse_ih)(){
    uint8_t stat;
    util_sys_inb(KBC_STATUS_REG, &stat);
    if((stat & (ST_PARITY | ST_TIMEOUT))==0) 
    {
        if((stat & ST_OBF) == 1)
        {
            util_sys_inb(OUT_BUFFER, &mouse_data);
        }
        else{ 
            tickdelay(micros_to_ticks(DELAY));
        }
    }
    else { 
        tickdelay(micros_to_ticks(DELAY));
    }
}

void (get_packet)(struct packet *pp, uint8_t bytes[]){
    uint16_t del_x, del_y;

    pp->bytes[0]=bytes[0];
    pp->bytes[1]=bytes[1];
    pp->bytes[2]=bytes[2];
    pp->rb=(bytes[0] & RIGHT_BUTTON) >>1;
    pp->lb=(bytes[0] & LEFT_BUTTON);
    pp->mb=(bytes[0] & MOUSE_BUTTON) >>2;
    pp->x_ov=(bytes[0] & X_OVERFLOW) >>6;
    pp->y_ov=(bytes[0] & Y_OVERFLOW) >>7;
    
    //deltax vai ter como lsb os bytes[1]
    //deltax vai ter como msb os bytes todos iguais ao bit 4 -> MSB_X_DELTA
    if(((bytes[0] & MSB_X_DELTA) >>4)==1) del_x=0xFF00;
    else del_x=0x0000;
    del_x=(del_x + bytes[1]);
    pp->delta_x=del_x;

    if(((bytes[0] & MSB_Y_DELTA) >>5)==1) del_y=0xFF00;
    else del_y=0x0000;
    del_y=(del_y + bytes[2]);
    pp->delta_y=del_y;
}

int (check_in_buffer)(){
    int counter = 0;
    uint8_t stat;
     while(counter < 10) {
        if(util_sys_inb(KBC_STATUS_REG, &stat) !=0 ){
            printf("ERROR in utils_sys_inb()\n");
            return 1;
        }
        if((stat & ST_IBF) == 0 ) {
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
        counter++;
    }
    printf("Input buffer never full error");
    return 1;
}

int (mouse_read_ack)(uint8_t *ack){
    unsigned counter = 0;
    uint8_t stat;
    while(counter < 20) {
        if(util_sys_inb(KBC_STATUS_REG, &stat) != 0){
            printf("ERROR in utils_sys_inb()\n");
            return 1;
        }
        if((stat & ST_OBF) != 0 ) {
            if(util_sys_inb(OUT_BUFFER, ack) == 0){
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
    printf("Error while trying to read ack byte\n");
    return 1;
}

int (mouse_write_command)(uint8_t cmd){
    uint8_t ack;
        do{
            if(check_in_buffer() == 0){
                sys_outb(KBC_COMAND_REG, WRITE_B_TO_MOUSE);
            }
            else{
                printf("Write byte to mouse error!\n");
               return 1; 
            } 
            if(check_in_buffer() == 0){
                sys_outb(IN_BUFFER, cmd);
            }
            else{
                printf("Write command error!\n");
               return 1; 
            } 
            mouse_read_ack(&ack);
            if(ack == ACK)
                return 0;

        }while(ack == NACK);
    return 1;
}

int (mouse_enable_data_reporting_me)(){
    if (mouse_write_command(EN_DATA_REPORT) == 1){
        printf("Error while trying to write command - enable report\n");
        return 1;
    }
    return 0;
}

int (mouse_disable_data_reporting)(){
    if (mouse_write_command(DIS_DATA_REPORT) == 1){
        printf("Error while trying to write command - disable report\n");
        return 1;
    }
    return 0;
}
