#pragma once
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"

static int mouse_hook_id=3;

int mouse_subscribe_int(uint8_t *mouse_bitmask){
    *mouse_bitmask=BIT(mouse_hook_id);
    sys_irqsetpolicy(MOUSE_IRQ_LINE,(IRQ_REENABLE|IRQ_EXCLUSIVE),&mouse_hook_id);
    return 0;
}

int mouse_unsubscribe_int(){
    sys_irqrmpolicy(&mouse_hook_id);
    return 0;
}

int mouse_handle_packet(uint8_t bytes[],struct packet *pk,int size){
    int16_t x=bytes[1],y=bytes[2];
    
    pk->rb=(bytes[0]&BIT(1));
    pk->lb=(bytes[0]&BIT(0));
    pk->mb=(bytes[0]&BIT(2));

    if((bytes[0]&BIT(4))!=0){
        x-=256;
    }
    if((bytes[0]&BIT(5))!=0){
        y-=256;
    }
    pk->delta_x=x;
    pk->delta_y=y;
    pk->x_ov=bytes[0]&BIT(6);
    pk->y_ov=bytes[0]&BIT(7);
    for (int i=0;i<size;i++){
        pk->bytes[i]=bytes[i];
    }
    return 0;
}

void(mouse_ih)(){
    uint32_t temp;
    if(check_output_buffer()==1){
        sys_inb(OUTPUT_BUFFER,&temp);
        data=(uint8_t)temp;
    }
    else{
        printf("erro no check output buffer");
    }
}

int check_output_buffer(){
    uint32_t temp;
    sys_inb(STATUS_REGISTER,&temp);
    uint8_t newtemp;
    newtemp=(uint8_t)temp;
    if (newtemp&BIT(0)){
        if((newtemp&(BIT(6)|BIT(7)))==0) return 1; 
    }
    else{
        return 0;
    }
    return 1;
}   

int check_input_buffer(){
    uint32_t temp;
    sys_inb(STATUS_REGISTER,&temp);
    uint8_t newtemp;
    newtemp=(uint8_t)temp;
    if(newtemp&BIT(1)){
        if((newtemp&BIT(6))==0&&(newtemp&BIT(7))==0) return 0;
        
    }
    return 1;
}

int(write_command)(uint32_t command,uint32_t argument){
    if(check_input_buffer()==0){
        printf("input buffer not ok");
        return 1;
    }
    sys_outb(COMMAND_REGISTER,command);
    uint32_t temp;
    uint8_t response;
    if(command==KBC_READ_COMMAND){
        sys_inb(OUTPUT_BUFFER,&temp);
        argument=(uint8_t)temp;
        return 0;
    }
    else if(command==KBC_WRITE_COMMAND){
        if(check_input_buffer()==0){
            printf("input buffer not ok");
            return 1;
        }
        sys_outb(COMMAND_ARGUMENT,argument);
    }
    else if (command==MOUSE_WRITE_COMMAND){
        if(check_input_buffer()==0){
            printf("input buffer not ok");
            return 1;
        }
        sys_outb(COMMAND_ARGUMENT,argument);
        sys_inb(OUTPUT_BUFFER,&temp);
        response=(uint8_t)temp;
        mouse_handle_commands(response,argument);
        return 0;
    }
    return 1;
}

int mouse_handle_commands(uint8_t response,uint32_t argument){

    
    if(response==ACK){
        return 0;
    }
    else if(response==NACK){
         if (write_command(MOUSE_WRITE_COMMAND,argument)==1){
            printf("error when writting command to kbc");
            return 1;}
    }
    else if(response==ERROR){
        printf("error writting command to mouse: second consecutive invalid byte");
        return 1;
    }
    return 1;
}

void mouse_update_state(mouse_state *state,struct packet *pk){
    if(pk->delta_y>0&&pk->lb==1){
        *state=up_left_move;
    }
    else if(pk->delta_y>0&&pk->rb==1){
        *state=up_right_move;
    }
    else if(pk->delta_y<0&&pk->lb==1){
        *state=down_left_move;
    }
    else if(pk->delta_y<0&&pk->rb==1){
        *state=down_right_move;
    }
    else if(pk->delta_y>0){
        *state=up_move;
    }
    else if(pk->delta_y<=0){
        *state=down_move;
    }

}

