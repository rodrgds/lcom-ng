#include "mouse.h"

int mouse_hook_id = 2;
uint8_t packetBytes[3];
uint8_t byte;
uint8_t byte_index = 0;
struct packet packet;
int mouse_x = 0, mouse_y = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = BIT(mouse_hook_id);

    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)() {
    kbc_read_data(KBC_OUT_BUF, &byte, true);
}

int (mouse_write_cmd)(uint8_t cmd) {
    uint8_t status;
    uint8_t attempts = 5;

    do {
        attempts--;
        if (kbc_write_cmd(KBC_IN_BUF_CMD, MOUSE_WRITE_BYTE)) return 1;
        if (kbc_write_cmd(KBC_IN_BUF_ARG, cmd)) return 1;

        tickdelay(micros_to_ticks(DELAY_US));

        if (util_sys_inb(KBC_OUT_BUF, &status)) return 1;
        if (status == KBC_ACK) return 0; 
    } while (status != KBC_ACK && attempts);

    return 1;
}

void (mouse_sync_bytes)() {
    if (byte_index == 0 && (byte & BIT(3))) {
        packetBytes[byte_index] = byte;
        byte_index++;
    } else if (byte_index > 0) {
        packetBytes[byte_index] = byte;
        byte_index++;
    }
}

void (mouse_parse_packet)() {
    for (int i = 0; i < 3; i++) {
        packet.bytes[i] = packetBytes[i];
    }

    packet.lb = packetBytes[0] & MOUSE_LB;
    packet.mb = packetBytes[0] & MOUSE_MB;
    packet.rb = packetBytes[0] & MOUSE_RB;
    packet.x_ov = packetBytes[0] & MOUSE_X_OVERFLOW;
    packet.y_ov = packetBytes[0] & MOUSE_Y_OVERFLOW;

    packet.delta_x = (packetBytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | packetBytes[1]) : packetBytes[1];
    packet.delta_y = (packetBytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | packetBytes[2]) : packetBytes[2];
}

void (mouse_update_position)() {
    if (mouse_x + packet.delta_x < 0 && !packet.x_ov) {
        mouse_x = 0;
    } else if (mouse_x + packet.delta_x > get_hres() && !packet.x_ov) {
        mouse_x = get_hres();
    } else if (!packet.x_ov) {
        mouse_x += packet.delta_x;
    }

    if (mouse_y - packet.delta_y < 0 && !packet.y_ov) {
        mouse_y = 0;
    } else if (mouse_y - packet.delta_y > get_vres() && !packet.y_ov) {
        mouse_y = get_vres();
    } else if (!packet.y_ov) {
        mouse_y -= packet.delta_y;
    }
}

int (mouse_draw_cursor)() {
    if (draw_sprite(cursor, mouse_x, mouse_y)) return 1;

    return 0;
}

bool (mouse_over_button)(Sprite *button) {
    return mouse_x >= button->x && mouse_x <= button->x + button->width && mouse_y >= button->y && mouse_y <= button->y + button->height;
}

bool (mouse_clicked_button)(Sprite *button) {
    return mouse_over_button(button) && packet.lb;
}
