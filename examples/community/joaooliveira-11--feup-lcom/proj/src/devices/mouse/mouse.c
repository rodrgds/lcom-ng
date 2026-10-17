#include "mouse.h"

int mouse_hook_id = 2;
uint8_t mouse_current_byte;
uint8_t mouse_byte_index = 0;
uint8_t mouse_bytes[3];
mouse_t mouse_structure;
extern vbe_mode_info_t vbe_mode_info;


void (mouse_ih)(){
    if(read_KBC_mouseOUTPUT(0x60, &mouse_current_byte) != 0){
        printf("Error while reading a byte from mouse output\n");
    }
}

void (packet_bytes)(){
    if(mouse_byte_index == 0 && ( mouse_current_byte & BIT(3) ) ){   // bit 3 ativo é o byte de controlo, logo, começamos um packet
        mouse_bytes[mouse_byte_index] = mouse_current_byte;
        mouse_byte_index++;
    }
    else if(mouse_byte_index > 0){
        mouse_bytes[mouse_byte_index] = mouse_current_byte;
        mouse_byte_index++;
    }
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    if(bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    if(sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;
    return 0;
}

void (build_mouse_packet)(){

    for(int x = 0 ;  x < 3; x++){
        mouse_structure.bytes[x] = mouse_bytes[x];
    }
    
    mouse_structure.lb = mouse_bytes[0] & BIT(0);
    mouse_structure.rb = mouse_bytes[0] & BIT(1);
    mouse_structure.mb = mouse_bytes[0] & BIT(2);

    mouse_structure.x_ov = mouse_bytes[0] & BIT(6);
    mouse_structure.y_ov = mouse_bytes[0] & BIT(7);

    if(mouse_structure.x_ov || mouse_structure.y_ov) return;

    mouse_structure.delta_x = (mouse_bytes[0] & BIT(4)) ? (0XFF00 | mouse_bytes[1]) : mouse_bytes[1]; // se o descolamento for negativo inverter para ter o valor absoluto
    mouse_structure.delta_y = (mouse_bytes[0] & BIT(5)) ? (0XFF00 | mouse_bytes[2]) : mouse_bytes[2]; // se o descolamento for negativo inverter para ter o valor absoluto
    
    if(mouse_structure.xpos + mouse_structure.delta_x < 0 || mouse_structure.ypos - mouse_structure.delta_y < 0 || mouse_structure.xpos + mouse_structure.delta_x >= vbe_mode_info.XResolution || mouse_structure.ypos - mouse_structure.delta_y >= vbe_mode_info.YResolution) return;
    mouse_structure.xpos += mouse_structure.delta_x;
    mouse_structure.ypos -= mouse_structure.delta_y;
}

int (mouse_write_input)(uint8_t mouse_command){
    uint8_t attemps = 10;
    uint8_t mouse_answer = 0;

    while(attemps && mouse_answer != 0xFA){
        if(write_KBC_INPUT(0x64, 0xD4) != 0) return 1;
        if(write_KBC_INPUT(0x60, mouse_command) != 0) return 1;  // comando direto para o rato com inibição da interpretação

        if(util_sys_inb(0x60, &mouse_answer) != 0) return 1;
        if(mouse_answer == 0xFA) return 0;
        tickdelay(micros_to_ticks(20000));
        attemps--;
    }
    return 1;
}

int(mouse_unsubscribe_int)(){
    if(sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;
}
