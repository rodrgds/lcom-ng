#include <lcom/lcf.h>

#include "mouse.h"
#include "macro_kb_mouse.h"

static int mouse_hook_id = MS_IRQ;
int ret_val;
uint8_t read_byte;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = mouse_hook_id;
    return sys_irqsetpolicy(MS_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_read_data)(uint8_t *byte) {
    return util_sys_inb(KBC_OUT_BUF, byte);
}

void (mouse_ih)() {
    if (mouse_read_data(&read_byte)) {
        ret_val = 1;
    } else ret_val = 0;
}

struct packet (mouse_parse_packet)(uint8_t packet[3]) {
    struct packet pp;

    pp.bytes[0] = packet[0]; 
    pp.bytes[1] = packet[1];
    pp.bytes[2] = packet[2];

    pp.lb = packet[0] & BIT(0);
    pp.rb = packet[0] & BIT(1);
    pp.mb = packet[0] & BIT(2);

    if (packet[0] & MS_X_MSB) pp.delta_x = 0xff00;
    else pp.delta_x = 0x0000;
    pp.delta_x |= packet[1];

    if (packet[0] & MS_Y_MSB) pp.delta_y = 0xff00;
    else pp.delta_y = 0x0000;
    pp.delta_y |= packet[2];

    pp.x_ov = packet[0] & MS_X_OVF;
    pp.y_ov = packet[0] & MS_Y_OVF;

    return pp;
}

int (mouse_issue_write_cmd)(uint8_t reg, uint8_t cmd) {
    uint8_t status;
    while (1) {
        if (util_sys_inb(KBC_ST_REG, &status)) return 1;
        if ((status & KBC_IBF) == 0) {
            return sys_outb(reg, cmd);
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (mouse_data_reporting)(uint8_t cmd) {
    uint8_t ack_byte;

    while (1) {
        if (mouse_issue_write_cmd(KBC_ST_REG, MS_WRITE_CMD_BYTE)) return 1;

        if (mouse_issue_write_cmd(KBC_IN_BUF, cmd)) return 1;

        if (mouse_read_data(&ack_byte)) return 1;

        switch (ack_byte) {
            case MS_ACK:
                return 0;
                break;
            case MS_ERROR:
                return 1;
                break;
            default:
                continue;
        }
    }
}
