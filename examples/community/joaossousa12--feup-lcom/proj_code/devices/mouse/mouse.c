#include "mouse.h"

#define IRQ_MOUSE 12

int hook_id_mouse=6;
struct packet mouse_packet;

uint8_t byte; //byte mais recente
uint8_t bytes[3];
uint8_t rb;
uint8_t lb;
uint8_t mb;
uint8_t x_ov;
uint8_t y_ov;
uint16_t delta_x;
uint16_t delta_y;

int current_index = 0;
int x = 11;
int y = 11;


int (mouse_subscribe_int)(){
    if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;
    return 0;
}

int (mouse_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_mouse)) return 1;
    return 0;
}



void (mouse_ih)(){
    read_KBC_output_mouse(0x60,&byte,1);
}

void (mouse_sync)(){
    if(current_index==0 && (byte & BIT(3))){
        bytes[0] = byte; //byte de controlo
        current_index++;
    }
    else if (current_index>0){
        bytes[current_index]=byte;
        current_index++;
    }
}

int (to_packet) (){
    for(int i=0; i<3; i++){
        mouse_packet.bytes[i] = bytes[i];
    }

    mouse_packet.rb = bytes[0]&BIT(1);
    mouse_packet.lb = bytes[0]&BIT(0);
    mouse_packet.mb = bytes[0]&BIT(2);
    mouse_packet.x_ov = bytes[0]&BIT(6);
    mouse_packet.y_ov = bytes[0]&BIT(7);
    mouse_packet.delta_x = (bytes[0]&BIT(4)) ? (0xFF00 | bytes[1]) : bytes[1];
    mouse_packet.delta_y = (bytes[0]&BIT(5)) ? (0xFF00 | bytes[2]) : bytes[2];

    return 0;
}

int (mouse_write)(uint8_t command) {

    uint8_t attemps = 10;
    uint8_t mouse_response=0;

    do {
        attemps--;
        if (write_KBC_command_mouse(0x64, 0xD4)) return 1;
        if (write_KBC_command_mouse(0x60, command)) return 1;
        tickdelay(micros_to_ticks(20000));
        if (read_KBC_output_mouse(0x60, &mouse_response, 1)) return 1;
        if(mouse_response==0xFA) return 0;
    } while (mouse_response != 0xFA && attemps);

    return 1;
}


void (refresh_mouse_location)(){
    // refresh x
    if(mouse_packet.delta_x + x < 11 && !mouse_packet.x_ov)
        x = 11;
    else if(mouse_packet.delta_x + x > 769 && !mouse_packet.x_ov)
        x = 769;
    else if(!mouse_packet.x_ov)
        x += mouse_packet.delta_x;
    // refresh y
    if(y - mouse_packet.delta_y < 11 && !mouse_packet.y_ov)
        y = 11;
    else if(y - mouse_packet.delta_y > 569 && !mouse_packet.y_ov)
        y = 569;
    else if(!mouse_packet.y_ov)
        y -= mouse_packet.delta_y;    
}
