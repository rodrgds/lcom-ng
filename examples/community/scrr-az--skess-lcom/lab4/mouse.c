#include <lcom/lcf.h>
#include <stdint.h>
#include "mouse.h"
#include "KBC.h"

int mouse_hook_id = 2;
struct packet mouse_packet;
int byte_index = 0;
int x_msb;
int y_msb;

int (mouse_subscribe_int)(uint8_t *bit_no){
    if(bit_no==NULL) return 1; //sees if the pointer is valid

    *bit_no = BIT(mouse_hook_id); //bit_no is now a mask for the mouse
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id); //subscribes the interruptions
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id); //unsubscribes the interrupts from mouse
}

int (mouse_write_command)(uint8_t command){ //we do not write commands to the mouse through the kbc, so we need a function for that
    int attempts_left = MAX_ATTEMPS;
    uint8_t acknowledgment;

    while (attempts_left>0){
        if(write_KBC_command(KBC_CMD_REG, MOUSE_WRITE)) return 1; //warn kbc you are going to write to the mouse
        if(write_KBC_command(KBC_OUT_BUF, command)) return 1; //write to the output buffer the command you want

        tickdelay(micros_to_ticks(DELAY)); //wait a bit so the mouse has time to acknowledge the command

        if(util_sys_inb(KBC_OUT_BUF, &acknowledgment)) return 1; //read the acknowledgment from the mouse
        if(acknowledgment == ACK) return 0; //it was successful

        attempts_left--;
    }

    return 1; //was not successful in any of the attempts
}

void (mouse_ih)(){
    uint8_t mouse_byte;

    if(read_KBC_output(&mouse_byte, true)) return; //if there is an error reading the output

    if(byte_index==0 && (mouse_byte & BYTE_1_MOUSE)){ //if we are looking for byte 1 and we got it
        mouse_packet.lb = mouse_byte & LEFT_MOUSE_BUTTON;
        mouse_packet.rb = mouse_byte & RIGHT_MOUSE_BUTTON;
        mouse_packet.mb = mouse_byte & MIDDLE_MOUSE_BUTTON;
        x_msb = mouse_byte & X_DELTA_SIGN;
        y_msb = mouse_byte & Y_DELTA_SIGN;
        mouse_packet.x_ov = mouse_byte & X_OVERFLOW;
        mouse_packet.y_ov = mouse_byte & Y_OVERFLOW;
        mouse_packet.bytes[0]=mouse_byte;
    }
    else if (byte_index==1){ //we are looking at the second byte (delta x)
        if(x_msb) mouse_packet.delta_x = (0xFF00 | mouse_byte);
        else mouse_packet.delta_x = mouse_byte;
        mouse_packet.bytes[1]=mouse_byte;

    }
    else if (byte_index==2){ //we are looking at the third byte (delta y)
        if(y_msb) mouse_packet.delta_y = (0xFF00 | mouse_byte);
        else mouse_packet.delta_y = mouse_byte;
        mouse_packet.bytes[2]=mouse_byte;

    }

    byte_index++; //one more byte analysed
}
