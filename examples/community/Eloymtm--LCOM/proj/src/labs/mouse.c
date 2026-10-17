#include "mouse.h"


uint8_t outByte=0;
int hook_id_mouse = 2;

uint8_t expected_byte = 1;
uint8_t packet[3];
uint8_t byte_count = 0;

struct packet pp;
int (mouse_subscribe_int)(uint8_t *bit_no){
    *bit_no = BIT(hook_id_mouse);
    return sys_irqsetpolicy(12,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id_mouse);

}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id_mouse);
}


void (mouse_ih)(){
    read_output_buffer(&outByte,true);
}

void synchronize_driver(){
    switch (byte_count)
    {
    case 0:
        if(outByte & BIT(3)){
            packet[byte_count++]=outByte;
        }
        break;
    case 1:
    break;
    case 2:
    packet[byte_count++] = outByte;
    break;
    
    }
}

void createPacket(){
    pp.bytes[0] = packet[0];
    pp.bytes[1] = packet[1];
    pp.bytes[2] = packet[2];

    pp.rb = packet[0] & BIT(1); //right button pressionado
    pp.mb = packet[0] & BIT(2); //mid button pressionado
    pp.lb = packet[0] & BIT(0); //left button pressionado


    pp.x_ov = packet[0] & BIT(6); //overflow em x
    pp.y_ov = packet[0] & BIT(7); //overflow em y

    pp.delta_x = packet[1]; //deslocamento em x
    if (packet[0] & BIT(4)) {
        pp.delta_x |= 0xFF00; 
    }

    pp.delta_y = packet[2]; //deslocamento em 7
    if (packet[0] & BIT(5)) {
        pp.delta_y |= 0xFF00;
    }
}

 int (mouse_commu)(uint8_t command){

    uint8_t mouseFeedback;
    uint8_t attemps =10;

    while(attemps > 0 && mouseFeedback != 0xFA){
    if(write_command(0x64,0xD4)!=0) return 1;
    if(write_command(0x60,command)!=0)return 1;
    tickdelay(micros_to_ticks(20000));
    if(util_sys_inb(0x60,&mouseFeedback)!=0)return 1;
    attemps--;
}
if(attemps==0){
    return 1;
}
return 0;
}
