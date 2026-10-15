#include "mouse.h"

int mouse_hook_id = 2;

struct packet mouse_packet;
uint8_t mouse_byte; 
uint8_t mouse_byte_index = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;
}

int (mouse_issue_cmd)(uint8_t cmd) {
    uint8_t status;

    if (KBC_write_cmd_byte(KBD_IN_BUF, MOUSE_WRITE_CMD) != 0) return 1;
    if (KBC_write_cmd_byte(KBD_OUT_BUF, cmd) != 0) return 1;

    if (util_sys_inb(KBD_OUT_BUF, &status) != 0) return 1;

    if (status == MOUSE_ACK) return 0;
    else if (status == MOUSE_NACK) return mouse_issue_cmd(cmd);

    return 1;
}

void (mouse_ih)() {
    if (KBC_read_output(KBD_OUT_BUF, &mouse_byte) != 0) KBC_read_output(KBD_OUT_BUF, &mouse_byte);
}

void (mouse_place_byte)() {
    if ((mouse_byte_index == 0 && (mouse_byte & BIT(3))) || (mouse_byte_index > 0)) {
        mouse_packet.bytes[mouse_byte_index] = mouse_byte;
        mouse_byte_index++;
    }
}

void (mouse_create_packet)() {
    mouse_packet.lb = mouse_packet.bytes[0] & BIT(0);
    mouse_packet.rb = mouse_packet.bytes[0] & BIT(1);
    mouse_packet.mb = mouse_packet.bytes[0] & BIT(2);
    mouse_packet.x_ov = mouse_packet.bytes[0] & BIT(6);
    mouse_packet.y_ov = mouse_packet.bytes[0] & BIT(7);
    mouse_packet.delta_x = mouse_packet.bytes[1];
    mouse_packet.delta_y = mouse_packet.bytes[2];
    if (mouse_packet.bytes[0] & BIT(4)) {
        mouse_packet.delta_x |= 0xFF00;
    }
    if (mouse_packet.bytes[0] & BIT(5)) {
        mouse_packet.delta_y |= 0xFF00;
    }
}
