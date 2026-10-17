#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"
                                
int kbd_hook_id;
int mouse_hook_id; 
uint8_t scan_codes[2];
int print = 0;
int num = 1;
extern uint8_t mouse_packet;
uint8_t scancode;
bool first_byte = false;
bool rightKey = false;
bool leftKey = false;

int(kbc_subscribe_int)(uint8_t *bit_no){
    kbd_hook_id = *bit_no;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(kbc_unsubscribe_int)(){
    return sys_irqrmpolicy(&kbd_hook_id);
}

int(mouse_subscribe_int)(uint8_t *bit_no){
    mouse_hook_id = *bit_no;
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

int(kbc_get_status)(uint8_t *st){
    return util_sys_inb(KBC_CMD_REG, st);
}

int( kbc_read_out_buffer)(uint8_t *data, uint8_t *st ){ 
    
    int err = 5;
    while(err != 0){
        if(kbc_get_status(st)){
            return 1;
        }
        if((*st & OUT_BFF_FULL) && check_status(*st) == 0 && ((*st & KBC_AUX_MOUSE) == 0) ){
            return util_sys_inb(KBC_OUT_BUF, data);
            
        }
        err--;
        if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
    }
    return 1;
}

int(mouse_read_out_buffer)(uint8_t *data, uint8_t *st ){ 
    
    int err = 5;
    while(err != 0){
        if(kbc_get_status(st)){
            return 1;
        }
        if((*st & OUT_BFF_FULL) && (check_status(*st) == 0)  && ((*st & KBC_AUX_MOUSE) != 0)){
            return util_sys_inb(KBC_OUT_BUF, data);
            
        }
        err--;
        if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
    }
    return 1;
}

int(kbc_get_scancode)(uint8_t* data){ // substituir pelo kbc_ih ou vice-versa
    uint8_t st;

    if(kbc_read_out_buffer(&scancode,&st) != 0) return 1;

    bool twobytes = false;

    if(scancode == TWO_BYTE){
        twobytes = true;
        data[0] = scancode; 
        return 0;
        }

    if(twobytes){
        data[1] = scancode;
        twobytes = false;
    }

    else{

        data[0] = scancode;
    }
 
    return 0;
}


int(check_status)(uint8_t st){
    return (st & (KBD_PAR_ERR | KBD_TO_ERR) );
}   

             
int(kbc_send_cmd)(uint8_t port, uint8_t cmd){
    int i = 5;
    uint8_t st = 0;
    while(i != 0){
        if(kbc_get_status(&st)){
            return 1;
        }
        if(st & KBD_ST_IBF){
            i--;
            if(tickdelay(micros_to_ticks(DELAY_US))) return 1;
            continue;
        }
        break;
        
    }
    if(i == 0) return 1; //Timeout

    return sys_outb(port, cmd);
}


// Mouse --

int(mouse_write_cmd)(uint8_t cmd) {
  uint8_t flag = 0;

  while (flag != KBC_ACK){
    kbc_send_cmd(KBC_CMD_REG, 0xD4);
    kbc_send_cmd(KBC_OUT_BUF, cmd);
    util_sys_inb(IN_BUF_ARGS, &flag);
  }
  return 0;
}

int (mouse_get_data)(struct packet* pp){
    uint8_t st;
    uint8_t packet_byte;
    
    if(kbc_get_status(&st) != 0) return 1;

    if(mouse_read_out_buffer(&packet_byte,&st) != 0) return 1;
    

    if(!(st & BIT(5))){
        
        return 1;
    }

    if(packet_byte & BIT(3)){
        first_byte = true; 
    }

    if(first_byte){
        pp->bytes[mouse_packet] = packet_byte;
        mouse_packet++;
    }

    if(mouse_packet == 3){
        
        first_byte = false;
    }
  

    return 0;    
}

int (mouse_parse_packet)(struct packet *pp){
  pp->lb = pp->bytes[0] & PACKET_LEFT_BOTTON;
  pp->mb = pp->bytes[0] & PACKET_MIDDLE_BOTTON;
  pp->rb = pp->bytes[0] & PACKET_RIGHT_BOTTON;
  pp->delta_x = (pp->bytes[0] & PACKET_X_NEGATIVE) ? (pp->bytes[1] | 0xFF00) : pp->bytes[1];
  pp->delta_y = (pp->bytes[0] & PACKET_Y_NEGATIVE) ? (pp->bytes[2] | 0xFF00) : pp->bytes[2];
  pp->x_ov = pp->bytes[0] & PACKET_X_OVERFLOW;
  pp->y_ov = pp->bytes[0] & PACKET_Y_OVERFLOW;
  return 0;
}

bool leftKeyPressed(){
    return leftKey;
}

bool rightKeyPressed(){
    return rightKey;
}

void pressLeftKey(bool is_pressed){
    leftKey = is_pressed;
}

void pressRightKey(bool is_pressed){
    rightKey = is_pressed;
}

