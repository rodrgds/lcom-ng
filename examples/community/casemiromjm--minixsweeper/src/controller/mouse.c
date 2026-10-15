#include "mouse.h"

uint8_t packet[3];  
struct packet mouse_packet; 
uint8_t byte_cnt = 0; 
u8_t current_byte;
static int mouse_hook_id = 2;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = (u8_t)mouse_hook_id;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
        return 1;
    }
    
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) {return 1;}

    return 0;  
}

void (mouse_ih)(void) {
    if (read_KBC_output(KBC_OUT_BUF, &current_byte, 1) != 0)
        printf("Error while reading a byte from mouse\n");
}

void (mouse_sync_packet)() {
    if (byte_cnt == 0 && (current_byte & BIT(3))) {
        packet[byte_cnt++] = current_byte;
    }
    else if (byte_cnt > 0 && byte_cnt < 3) {
        packet[byte_cnt++] = current_byte;
    }

    // After getting 3 bytes, the counter will be reset externally after parsing
}

void (mouse_parse_packet)() {
    for (int i = 0; i < 3; i++) {
        mouse_packet.bytes[i] = packet[i];
    }

    mouse_packet.lb = packet[0] & MOUSE_LB;
    mouse_packet.rb = packet[0] & MOUSE_RB;
    mouse_packet.mb = packet[0] & MOUSE_MB;
    mouse_packet.x_ov = packet[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = packet[0] & MOUSE_Y_OVERFLOW;

    mouse_packet.delta_x = (packet[0] & MOUSE_X_SIGNAL) ? (0xFF00 | packet[1]) : packet[1];
    mouse_packet.delta_y = (packet[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | packet[2]) : packet[2];
}

int (mouse_send_command)(uint8_t command) {
    u8_t attempts = MAX_ATTEMPTS;
    u8_t mouse_response;

    do {
        attempts--;

        if (write_KBC_command(KBC_CMD_REG, 0xD4) != 0) return 1;
        if (write_KBC_command(KBC_OUT_BUF, command) != 0) return 1;

        tickdelay(micros_to_ticks(DELAY_US));

        if (util_sys_inb(KBC_OUT_BUF, &mouse_response) != 0) return 1;
        if (mouse_response == KBC_ACK) return 0;

    } while (mouse_response != KBC_ACK && attempts > 0);

    return 1;
}

int (mouse_enable_int)() {
    if (sys_irqenable(&mouse_hook_id) != 0) return 1;
    return 0;
}

int (mouse_disable_int)() {
    if (sys_irqdisable(&mouse_hook_id) != 0) return 1;
    return 0;
}

