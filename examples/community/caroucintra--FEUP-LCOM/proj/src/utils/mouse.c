#ifndef _LCOM_MOUSE_C_
#define _LCOM_MOUSE_C_
#include "mouse.h"

#include <lcom/lcf.h>

int byte_no = 0;
bool pack_done = false;
uint8_t lineXLen = 0;
uint8_t lineYLen = 0;
uint16_t x = 30;
uint16_t y = 200;

int (mouse_subscribe_int)(uint8_t *bit_no){
  mouse_hookid = 2;
  *bit_no = mouse_hookid;

  if (sys_irqsetpolicy(MOUSE_IRQ,  IRQ_REENABLE|IRQ_EXCLUSIVE , &mouse_hookid)) return -1;

  return 0;
}


int (mouse_unsubscribe_int)(){

  return sys_irqrmpolicy(&mouse_hookid);

}


int (write_kbc_mouse_reg)(uint8_t port, uint8_t command){
    int i = 0;
    uint8_t st;
    while (i < 5){
        if (util_sys_inb(KBC_CMD_REG, &st)) return 1;//MUDAR PARA PORT

        if (!(st & TO_ERR) && !(st & PAR_ERR) && !(st & IBF)) {
                sys_outb(port, command);
                return 0;
            }

        i++;
        tickdelay(micros_to_ticks(25));
    }
    return -1;
}

int (get_answer)(uint8_t * answer){
    int i = 0;
    uint8_t st;
    while (i < 5){
        printf("%d\n",i);
        if (util_sys_inb(KBC_CMD_REG, &st)) return 1;//MUDAR PARA PORT
        //if ((st & OBF)) {
                util_sys_inb(KBC_IN_BUF, answer);

                if ((st & TO_ERR) || (st & PAR_ERR)){
                    continue;
                }

                return 0;
            //}

        i++;
        tickdelay(micros_to_ticks(25));
    }
    return -1;
}


int enable_data_reporting(){
    uint8_t answer;

    int i = 0;

    do{
        write_kbc_mouse_reg(KBC_CMD_REG, BYTE_TO_MOUSE); 

        
        if (write_kbc_mouse_reg(KBC_OUT_BUF, ENABLE_DATA_REPORTING)){
            printf("erro\n");
            return -1;
        };
        get_answer(&answer);
        i++;
        

    }while(answer != ACK & i<5);

    return 0;
}


int disable_data_reporting(){
    uint8_t answer;

    int i = 0;

    do{
        write_kbc_mouse_reg(KBC_CMD_REG, BYTE_TO_MOUSE); 

        
        if (write_kbc_mouse_reg(KBC_OUT_BUF, DISABLE_DATA_REPORTING)){
            printf("erro\n");
            return -1;
        };
        get_answer(&answer);
        i++;
        

    }while(answer != ACK & i<1);

    return 0;
}


void (mouse_ih)(){
    uint8_t st;
    if (util_sys_inb(STAT_REG, &st)) return;
    if (st & TO_ERR) return ;
    if (st & PAR_ERR) return;

    if (st & OBF){
        if (st & AUX){
            handle_byte();
        }
    }

}

int (handle_byte)(){
    uint8_t outp;
    if (util_sys_inb(KBC_OUT_BUF, &outp)) return 1;

    bytes[byte_no] = outp;

    byte_no ++;

    if (byte_no == 3){
        return handle_mouse_package();
    }
    
    return 0;
}

int handle_mouse_package(){
    byte_no = 0;

    packet.bytes[0] = bytes[0]; 
    packet.bytes[1] = bytes[1]; 
    packet.bytes[2] = bytes[2]; 
    packet.rb = ( bytes[0] & RIGHT);
    packet.lb = ( bytes[0] & LEFT);
    packet.mb = ( bytes[0] & MIDDLE);
    packet.x_ov = ( bytes[0] & X_OVF);
    packet.y_ov = ( bytes[0] & Y_OVF);

    if ( bytes[0] & X_SIGN){
        packet.delta_x = ( 0xff00 | bytes[1]);
    }
    else{
        packet.delta_x = bytes[1];
    }
    if ( bytes[0] & Y_SIGN){
        packet.delta_y = ( 0xff00 | bytes[2]);
    }
    else{
        packet.delta_y = bytes[2];
    }

    printf("X %d Y %d\n", packet.delta_x, packet.delta_y);
    pack_done = true;
    return 1;

}   

    


#endif /* _LCOM_MOUSE_C */
