
#include "mouse.h"
int mouse_hook_id = 2;
uint8_t byte_counter = 0;
uint8_t packet_bytes[3];
struct packet mouse_packet;
int mouse_x = 10;
int mouse_y = 10;
uint8_t current_byte;
extern vbe_mode_info_t modeInfo;
int (mouse_subscribe_int)(){
    return sys_irqsetpolicy(IRQ_MOUSE,IRQ_REENABLE|IRQ_EXCLUSIVE,&mouse_hook_id);
}
int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}
void (mouse_ih)(){
    read_KBC_output(KBC_OUT_BUF,&current_byte,1);
}
void (mouse_sync_bytes)(){
    if((byte_counter == 0 ) && (MOUSE_3_BIT & current_byte)){
        packet_bytes[byte_counter] = current_byte;
        byte_counter++;
    }
    else if (byte_counter>0){
        packet_bytes[byte_counter] = current_byte;
        byte_counter++;
    }
}
void (mouse_bytes_to_packet)() {
    for (int i = 0; i < 3; i++) {
        mouse_packet.bytes[i] = packet_bytes[i];
    }

    mouse_packet.rb = packet_bytes[0] & MOUSE_RIGHT_BUTTON;
    mouse_packet.lb = packet_bytes[0] & MOUSE_LEFT_BUTTON;
    mouse_packet.mb = packet_bytes[0] & MOUSE_MIDDLE_BUTTON;
    mouse_packet.x_ov = packet_bytes[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = packet_bytes[0] & MOUSE_Y_OVERFLOW;

    if (mouse_packet.x_ov || mouse_packet.y_ov) return;

    mouse_packet.delta_x = (packet_bytes[0] & MOUSE_X_DISPLACEMENT) ? (0xFF00 | packet_bytes[1]) : packet_bytes[1];
    mouse_packet.delta_y = (packet_bytes[0] & MOUSE_Y_DISPLACEMENT) ? (0xFF00 | packet_bytes[2]) : packet_bytes[2];
    int new_x = mouse_x + mouse_packet.delta_x;
    int new_y = mouse_y - mouse_packet.delta_y;

    if (new_x < 0) new_x = 0;
    else if (new_x > modeInfo.XResolution) new_x = modeInfo.XResolution-1;

    if (new_y < 0) new_y = 0;
    else if (new_y > modeInfo.YResolution) new_y = modeInfo.YResolution-1;

    mouse_x = new_x;
    mouse_y = new_y;
}

int (mouse_write)(uint8_t command){
    uint8_t attemps = 10;
    uint8_t mouse_response;
    while(attemps){
        if(write_KBC_command(KBC_IN_BUFFER,WRITE_BYTE_MOUSE)!=0)return 1;
        if(write_KBC_command(KBC_WRITE_CMD_BYTE,command)!=0)return 1;
        tickdelay(micros_to_ticks(DELAY_US));
        if(util_sys_inb(KBC_OUT_BUF,&mouse_response)!=0)return 1;
        if(mouse_response == ACK)return 0;
        attemps--;
    }
    return 1;
}
