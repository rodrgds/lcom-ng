#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "mouse.h"
#include "../graphics/graphics.h"

static int hook_mouse = 2;
mouse_packet_t mouse_packet;

int(mouse_subscribe_int)(uint8_t* bit_no) {
    *bit_no = BIT(hook_mouse);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_mouse);
}

int(mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_mouse);
}

int (mouse_send_cmd)(uint8_t cmd) {
    uint8_t tries = 10, res = 0;

    while (res != MOUSE_ACK) {
        if (tries-- <= 0) {
            printf("Could not send command to mouse!\n");
            return 1;
        }

        if (!ibf_empty()) {
            tickdelay(WAIT_KBC);
            continue;
        }

        kbc_write_cmd(MOUSE_WRITE_DATA);

        if (!ibf_empty()) {
            tickdelay(WAIT_KBC);
            continue;
        }

        kbc_write_arg(cmd);
        /*
        if (!obf_full()) {
            tickdelay(WAIT_KBC);
            continue;
        }
        */
        if (kbc_read_data(&res)) {
            tickdelay(WAIT_KBC);
            continue;
        }
    }

    return 0;
}

void (m_read_byte)(uint8_t b) {
    mouse_packet.bytes[mouse_packet.byte++] = b;
    if (mouse_packet.byte == 3) {
        mouse_packet.byte = 0;
        mouse_packet.complete = true;
        mouse_build_packet();
        mouse_update_pos();
    }
    else {
        mouse_packet.complete = false;
    }
}

void (mouse_build_packet)() {
    mouse_packet.lb = mouse_packet.bytes[0] & MOUSE_LB;
    mouse_packet.rb = mouse_packet.bytes[0] & MOUSE_RB;
    mouse_packet.mb = mouse_packet.bytes[0] & MOUSE_MB;
    mouse_packet.delta_x = (mouse_packet.bytes[0] & MOUSE_MSBX) ? (mouse_packet.bytes[1] | 0xFF00) : (mouse_packet.bytes[1]);
    mouse_packet.delta_y = (mouse_packet.bytes[0] & MOUSE_MSBY) ? (mouse_packet.bytes[2] | 0xFF00) : (mouse_packet.bytes[2]);
    mouse_packet.x_ov = mouse_packet.bytes[0] & MOUSE_XOV;
    mouse_packet.y_ov = mouse_packet.bytes[0] & MOUSE_YOV;
}

void (mouse_update_pos)() {
    int16_t dx = mouse_packet.delta_x;
    int16_t dy = mouse_packet.delta_y;

    if (!mouse_packet.x_ov) {
        if (mouse_packet.x + dx < 0) {
            mouse_packet.x = 0;
        }
        else if (mouse_packet.x + dx >= get_hres()) {
            mouse_packet.x = get_hres() - 1;
        }
        else {
            mouse_packet.x += dx;
        }
    }

    if (!mouse_packet.y_ov) {
        if (mouse_packet.y - dy < 0) {
            mouse_packet.y = 0;
        }
        else if (mouse_packet.y - dy >= get_vres()) {
            mouse_packet.y = get_vres() - 1;
        }
        else {
            mouse_packet.y -= dy;
        }
    }
}

void (mouse_ih)() {
    uint8_t st, byte;

    if (kbc_get_status(&st)) {
        printf("Error reading status!\n");
        return;
    }

    if (st & (KBC_OUT_FULL | MOUSE_DATA)) {
        kbc_read_data(&byte);

        if (st & KBC_ERROR) {
            printf("Error reading data!\n");
            return;
        }

        if (mouse_packet.byte == 0 && !(byte & MOUSE_SYNC_BIT)) {
            printf("Mouse is not synced!\n");
            return;
        }

        m_read_byte(byte);
    }
}

int (mouse_enable_dr)() {
    return mouse_send_cmd(MOUSE_ENABLE_DR);
}

int (mouse_disable_dr)() {
    return mouse_send_cmd(MOUSE_DISABLE_DR);
}

int (mouse_stream_mode)() {
    return mouse_send_cmd(MOUSE_STREAM_MODE);
}

int (mouse_inside)(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    return (x <= mouse_packet.x && mouse_packet.x <= x + width) && (y <= mouse_packet.y && mouse_packet.y <= y + height);
}

int (mouse_over_sprite)(Sprite* sp) {
    return mouse_inside(sp->x, sp->y, sp->width, sp->height);
}

int (mouse_lclick_sprite)(Sprite* sp) {
    return mouse_over_sprite(sp) && mouse_packet.lb;
}

int (mouse_rclick_sprite)(Sprite* sp) {
    return mouse_over_sprite(sp) && mouse_packet.rb;
}
