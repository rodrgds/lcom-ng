#include <lcom/lcf.h>
#include "mouse.h"

int mouse_hook_id = 3;
uint8_t byte_index = 0;       // [0..2]
uint8_t mouse_bytes[3];            // pacote
uint8_t current_byte;         // o byte mais recente lido
uint32_t count = 0; // count of completed packets
struct packet p;

void (mouse_ih)(){
  if (read_KBC_output(0x60,&current_byte,1)!=0) return;
}

void (mouse_sync_bytes)(){
    if (byte_index == 0 && (current_byte & BIT(3))) {
        // é o byte CONTROL, o bit 3 está ativo
        mouse_bytes[byte_index]= current_byte;
        byte_index++;
    }
    else if (byte_index > 0) {
        // recebe os deslocamentos em X e Y
        mouse_bytes[byte_index] = current_byte;
        byte_index++;
    }
    if (byte_index == 3){
        make_packet();
        byte_index = 0;
    }
}

int (write_to_mouse)(uint8_t command){
    uint8_t attemps = 10;
    uint8_t mouse_response;
    do {
        attemps--;
        if (write_KBC_command(0x64, 0xD4)) return 1;
        if (write_KBC_command(0x60, command)) return 1;
        tickdelay(micros_to_ticks(20000));
        if (read_KBC_output(0x60, &mouse_response, 1)) return 1;
        if (mouse_response==0xFA) return 0;
    } while (attemps);       

    return 1;
}

int (mouse_subscribe_interrupts)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_interrupts)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (make_packet)(){
    for (int i = 0; i<3; i++){
        p.bytes[i]=mouse_bytes[i];
    }
    p.lb = mouse_bytes[0] & BIT(0);
    p.mb = mouse_bytes[0] & BIT(2);
    p.rb = mouse_bytes[0] & BIT(1);
    p.x_ov = mouse_bytes[0] & BIT(6);
    p.y_ov = mouse_bytes[0] & BIT(7);
    p.delta_x = mouse_bytes[0] & BIT(4) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
    p.delta_y = mouse_bytes[0] & BIT(5) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
    count++;
}
