#include "mouse.h"
#include "kbd.h"
// #include "KBC.h"
#include "i8042.h"

#ifdef DEBUG
#  define DEBUG_MSG(str) printf(str)
#else
#  define DEBUG_MSG(str)
#endif

struct packet mouse_packet;
uint8_t byte_index  =  0 ;  // [0..2]
uint8_t mouse_bytes[3]; // packet of 3 bytes
uint8_t current_byte; // current byte being read

int mouse_hook_id = 2;

bool packet_parsed = false;

int (KBC_read_output_mouse)(uint8_t port, uint8_t* output){
    
    uint8_t status;

    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

    if ( status & KBC_OUTPUTB_FULL){
        if (util_sys_inb(port, output) != 0) {
            DEBUG_MSG("Could not read output buffer\n");
            return 1;
        }   
        if ((status & PARITY_ERR) || (status & TIMEOUT_ERR)){
            DEBUG_MSG("Parity or Timeout error\n");
            return 1;
        }
        if(!(status & BIT(5))){ //tentar ler o mouse e verificar se o bit de auxiliar esta a 1
            DEBUG_MSG("Mouse data not available"); //output nao é do rato
            return 1;
        }
        return 0;

    }

    return 1; // se ultrapassar o numero de tentativas
}

void (mouse_ih)(){
    if(KBC_read_output_mouse(KBD_OUT_CMD, &current_byte)!=0) printf("Error in mouse - Could not read current byte from packet");

    if (byte_index == 0){
        if(current_byte & BIT(3)){
            mouse_bytes[0] = current_byte;
            byte_index = 1;
            return;
        }
    }

    mouse_bytes[byte_index] = current_byte;
    byte_index++;
}

int (mouse_subscribe_int)(uint8_t *bit){
    *bit = BIT(mouse_hook_id);

    if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)!=0) return 1;

    return 0;
}

int (mouse_unsubscribe_int)(){
    if(sys_irqrmpolicy(&mouse_hook_id)!=0) return 1;
    return 0;
}


void (mouse_bytes_to_packet)(struct packet *mouse_packet){
    for(int i=0; i<3; i++){
         mouse_packet->bytes[i] = mouse_bytes[i];
    }

    uint8_t control_byte = mouse_bytes[0];

    mouse_packet->lb = control_byte & BIT(0);
    mouse_packet->mb = control_byte & BIT(2);
    mouse_packet->rb = control_byte & BIT(1);
    mouse_packet->x_ov = control_byte & BIT(6);
    mouse_packet->y_ov = control_byte & BIT(7);

    if (control_byte & BIT(4)){
        mouse_packet->delta_x = 0xFF00 | mouse_bytes[1];
    }
    else{
        mouse_packet->delta_x = mouse_bytes[1];
    }

    if (control_byte & BIT(5)){
        mouse_packet->delta_y = 0xFF00 | mouse_bytes[2];
    }
    else{
        mouse_packet->delta_y = mouse_bytes[2];
    }
}

int (mouse_write)(uint8_t cmd){
    uint8_t attempts = 10;
    uint8_t mouse_response = 0xFE;

    while(mouse_response != 0xFE && attempts){
        if(KBC_write_cmd(0x64, 0xD4)!=0) return 1; //avisar que vamos escrever para o KBC o 0xD4
        if(KBC_write_cmd(0x60, cmd)!=0) return 1; //escrever o command no KBC (porta 0x64)
        tickdelay(micros_to_ticks(20000));
        if(KBC_read_output_mouse(0x64, &mouse_response)!=0) return 1; // ler o que o KBC nos respondeu
    }

    return 0;

}
