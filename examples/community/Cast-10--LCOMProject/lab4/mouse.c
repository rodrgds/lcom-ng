#include "mouse.h"
#include "i8042.h"
#define MAX_ATTEMPTS 5
static uint8_t packet_bytes[3];
static uint8_t packet_index = 0;
static struct packet pp;
static int hook_id_mouse = MOUSE_IRQ;
static bool pckt_complete = false;

static int mouse_send_cmd(uint8_t cmd, uint8_t* ack){
    if (kbc_send_command(WRITE_TO_MOUSE) != 0) return 1;
    if (kbc_write_data(cmd) != 0) return 1;
    if (kbc_read_response(ack) != 0) return 1;

    return 0;
}

int mouse_send_command(uint8_t cmd){
    uint8_t ack;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS){
        
        if (mouse_send_cmd(cmd,&ack) != 0) return 1;

        if (ack == ACK) return 0;

        tickdelay(micros_to_ticks(20000));
       
        attempts++;
    }

    return 1;
}


// This functions is only used if one single command is intended to be sent. If more than one disable first send them and enable again
int mouse_send_command_with_sync(uint8_t cmd) { 
    if (cmd != DISABLE_DATA_REPORTING) {
        if (mouse_send_command(DISABLE_DATA_REPORTING) != 0) return 1;
    }

    if (mouse_send_command(cmd) != 0) return 1;

    if (cmd != ENABLE_DATA_REPORTING && cmd != DISABLE_DATA_REPORTING) {
        if (mouse_send_command(ENABLE_DATA_REPORTING) != 0) return 1;
    }

    return 0;
}


int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_mouse;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0) return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id_mouse) != 0) return 1;
    return 0;
}


void (mouse_ih)(){

    uint8_t status;
    uint8_t byte;

    if (util_sys_inb(STATUS_REGISTER,&status) != 0) return;

    if (!(status & OUTPUT_BUFFER_FULL)) return;

    if (status & (PARITY_ERROR | TIMEOUT_ERROR)) return;

    if (!(status & (AUX))) return;

    if (util_sys_inb(OUTPUT_BUFFER, &byte) != 0) return;

    if (process_packet(byte) != 0) return;

}

int process_packet(uint8_t byte) {

    if (packet_index == 0) {
        if (!(byte & SYNCRONIZED)) return 1;
    }

    packet_bytes[packet_index] = byte;
    packet_index++;

    if (packet_index == 3) {
        pp.bytes[0] = packet_bytes[0];
        pp.bytes[1] = packet_bytes[1];
        pp.bytes[2] = packet_bytes[2];

        pp.lb = (packet_bytes[0] & LEFT_BUTTON) != 0;
        pp.rb = (packet_bytes[0] & RIGHT_BUTTON) != 0;
        pp.mb = (packet_bytes[0] & MIDDLE_BUTTON) != 0;

        pp.x_ov = (packet_bytes[0] & X_OVERFLOW) != 0;
        pp.y_ov = (packet_bytes[0] & Y_OVERFLOW) != 0;

        if (packet_bytes[0] & X_SIGN)
            pp.delta_x = (int16_t)(0xFF00 | packet_bytes[1]);
        else
            pp.delta_x = packet_bytes[1];

        if (packet_bytes[0] & Y_SIGN)
            pp.delta_y = (int16_t)(0xFF00 | packet_bytes[2]);
        else
            pp.delta_y = packet_bytes[2];

        packet_index = 0;
        pckt_complete = true;
    }

    return  0;
}

bool packet_complete(){
    return pckt_complete;
}

void reset_packet(){
    packet_index = 0;
    pckt_complete = false;
    packet_bytes[0] = 0;
    packet_bytes[1] = 0;
    packet_bytes[2] = 0;
}

struct packet *get_packet() {
    return &pp;
}
