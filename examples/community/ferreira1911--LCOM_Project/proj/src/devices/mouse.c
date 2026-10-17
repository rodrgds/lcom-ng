#include <lcom/lcf.h>

#include "mouse.h"
#include "kbc.h"

uint8_t mouse_byte;
uint8_t byte_index = 0;
uint8_t packet[3];
int mouse_hook_id = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    mouse_hook_id = *bit_no;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0)
        return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != OK)
        return 1;
    return 0;
}

int (write_mouse_cmd)(uint8_t cmd) {
    uint8_t response;
    uint8_t status;

    for (int retries = 10; retries > 0; retries--) {
        if (write_kbc_cmd(KBC_WRITE_MOUSE, KBC_COMMAND_REGISTER) != 0) return 1;
        if (write_kbc_cmd(cmd, KBC_INPUT_BUFFER) != 0) return 1;

        tickdelay(micros_to_ticks(KBC_DELAY));

        if (util_sys_inb(KBC_STATUS_REGISTER, &status) != 0) return 1;
        if (status & KBC_STATUS_OB_FULL) {
            if (util_sys_inb(KBC_OUTPUT_BUFFER, &response) != 0) return 1;
            if (response == MOUSE_ACK) return 0;
        }
    }
    return 1;
}

void (mouse_ih)() {
    read_mouse_data(&mouse_byte);
}

int (read_mouse_data)(uint8_t *byte) {
    uint8_t response;
    
    for (int retries = 10; retries > 0; retries--) {

        if(util_sys_inb(KBC_STATUS_REGISTER, &response) != 0) return 1;
        
        if((response & KBC_STATUS_OB_FULL)  && (response & KBC_STATUS_D)){
            if(response & KBC_STATUS_PARITY_ERR) return 1;
            if(response & KBC_STATUS_TIMEOUT_ERR) return 1;
            if (util_sys_inb(KBC_OUTPUT_BUFFER, byte) != 0) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(KBC_DELAY));
    }
    return 1;
}

void (sync_bytes)(){
    if (byte_index == 0 && !(mouse_byte & MOUSE_CONTROL_BYTE)) return;

    packet[byte_index] = mouse_byte;
    byte_index++;
}

void (parse_packet)(struct packet *pp) {
    pp->bytes[0] = packet[0];
    pp->bytes[1] = packet[1];
    pp->bytes[2] = packet[2];

    pp->lb = packet[0] & MOUSE_LEFT_BUTTON;
    pp->rb = packet[0] & MOUSE_RIGHT_BUTTON;
    pp->mb = packet[0] & MOUSE_MIDDLE_BUTTON; 

    pp->x_ov = packet[0] & MOUSE_OVERFLOW_X;
    pp->y_ov = packet[0] & MOUSE_OVERFLOW_Y;

    if (packet[0] & MOUSE_SIGNAL_X)
        pp->delta_x = (int16_t)(0xFF00 | packet[1]);
    else {
        pp->delta_x = packet[1];
    }

    if (packet[0] & MOUSE_SIGNAL_Y)
        pp->delta_y = (int16_t)(0xFF00 | packet[2]);
    else {
        pp->delta_y = packet[2];
    }
}

bool mouse_packet_ready() {
    return byte_index == 3;
}

void get_mouse_packet(struct packet *pp) {
    parse_packet(pp);
}

void reset_mouse_packet() {
    byte_index = 0;
}

