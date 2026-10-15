#include "mouse.h"

int hook_id_mouse = 3;

uint8_t bytes_index = 0;
uint32_t bytes[3];


uint8_t mouse_subscribe(){
    uint8_t bit_no = BIT(hook_id_mouse);
    if(sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse))return 0xFF;
    return bit_no;
}
int mouse_unsubscribe(){
    return sys_irqrmpolicy(&hook_id_mouse);
}
int mouse_write(uint8_t command){
    uint32_t mouse_status;
    do{
        if(mouse_write_output(KBC_STATUS, WRITE_MOUSE_CMD) == ERROR)return 1;
        if(mouse_write_output(OUT_BUFFER, command) == ERROR)return 1;
        if(sys_inb(OUT_BUFFER, &mouse_status))return 1;
    }while(mouse_status != ACK);
    return 0;
}


int mouse_read_output(int port){
    uint32_t status;
    uint32_t command;

    if(sys_inb(KBC_STATUS, &status)) return ERROR;

    if ((status & OUT_BUFFER_BIT)) {
        if((status & PARITY_ERROR) || (status & TIME_OUT_ERROR))return ERROR;

        sys_inb(port, &command);

        if(!(status & BIT(5))) return 0xFFFF;
        return command;

    }

    return ERROR;
}


int mouse_write_output(int port, unsigned long command){
    uint32_t status;
    if(sys_inb(KBC_STATUS, &status))return ERROR;

    while((status & IN_BUFFER_BIT)){};

    if((status & PARITY_ERROR) || (status & TIME_OUT_ERROR))return ERROR;

    if(sys_outb(port, command))return ERROR;
    return 0;
    
}

int mouse_control(){
    uint32_t scancode = mouse_read_output(OUT_BUFFER);
    if(scancode == ERROR)return 0xFF;

    if(bytes_index == 0 && (scancode & MOUSE_CONTROL_BIT)) bytes[bytes_index] = scancode;
    else bytes[bytes_index] = scancode;
    bytes_index++;

    if(bytes_index == 3) {
        fill_mouse_packet(scancode);
        move_mouse();
    }

    return 0;
}

void fill_mouse_packet(uint32_t scancode){
    for(int i = 0; i<3; i++){
        mouse_packet.bytes[i] = bytes[i];
    }
    uint32_t control_byte = bytes[0];
    mouse_packet.lb = control_byte & CLICK_LEFT;
    mouse_packet.rb = control_byte & CLICK_RIGHT;
    mouse_packet.mb = control_byte & CLICK_MIDDLE;
    mouse_packet.delta_x = (control_byte & DELTA_X) ? (SET_NEGATIVE | bytes[1]) : bytes[1];
    mouse_packet.delta_y = (control_byte & DELTA_Y) ? (SET_NEGATIVE | bytes[2]) : bytes[2];
    mouse_packet.x_ov = control_byte & X_OVERFLOW;
    mouse_packet.y_ov = control_byte & Y_OVERFLOW;

    bytes_index = 0;
}



void move_mouse(){
    if(mouse_x+mouse_packet.delta_x < 0 && !mouse_packet.x_ov){
        mouse_x = 0;
    }
    else if(mouse_x+mouse_packet.delta_x > 1140 && !mouse_packet.x_ov){
        mouse_x = 1140;
    }
    else if(!mouse_packet.x_ov){
        mouse_x+=mouse_packet.delta_x;
    }

    if(mouse_y-mouse_packet.delta_y < 0 && !mouse_packet.y_ov){
        mouse_y = 0;
    }
    else if(mouse_y-mouse_packet.delta_y > 850 && !mouse_packet.y_ov){
        mouse_y = 850;
    }
    else if(!mouse_packet.y_ov){
        mouse_y-=mouse_packet.delta_y;
    }
}
bool mouse_position(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf){
    if(mouse_x >= xi && mouse_x <= xf && mouse_y <= yf && mouse_y >= yi){
        mouse_hover = true;
        return true;
    }
    return false;
}

