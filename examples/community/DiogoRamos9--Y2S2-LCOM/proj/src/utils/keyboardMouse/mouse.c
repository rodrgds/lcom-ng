#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.c"
#include "mouse.h"
#include "../utils.c"

int mouse_hook_id = 3;
uint8_t scancode;
int byte_index = 0;
struct packet pp;
bool activated_buttons[3];

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

void (mouse_ih)() {
    read_output(KBC_OUT_BUF, &scancode);
}

void handle_mouse() {
    if (byte_index == 0) {
        if (scancode & BIT(3)) {
            pp.bytes[0] = scancode;
            byte_index++;
        }
    }
    else if (byte_index == 1) {
        pp.bytes[1] = scancode;
        byte_index++;
    }
    else if (byte_index == 2) {
        pp.bytes[2] = scancode;
        byte_index = 0;
        if (pp.bytes[0] & MSB_X_DELTA) pp.delta_x = (pp.bytes[1] | 0xFF00);
        else pp.delta_x = pp.bytes[1];
        if (pp.bytes[0] & MSB_Y_DELTA) pp.delta_y = (pp.bytes[2] | 0xFF00);
        else pp.delta_y = pp.bytes[2];
        pp.lb = pp.bytes[0] & MOUSE_LB;
        pp.rb = pp.bytes[0] & MOUSE_RB;
        pp.mb = pp.bytes[0] & MOUSE_MB;
        if (pp.lb) activated_buttons[0] = true;
        if (pp.rb) activated_buttons[1] = true;
        if (pp.mb) activated_buttons[2] = true;
    }

}

int (mouse_write)(uint8_t command) {
    int attempts = 10;
    uint8_t ack_byte;
    while (attempts) {
        if (write_command(KBC_CMD_REG, MOUSE_WRITE_BYTE) != 0) return 1;
        if (write_command(KBC_IN_BUF, command) != 0) return 1;
        if (util_sys_inb(KBC_OUT_BUF, &ack_byte) != 0) return 1;
        if (ack_byte == ACK) return 0;
        attempts--;
    }
    return 1;
}

int (mouse_disable_data_reporting)() {
    return mouse_write(DISABLE_DATA_REPORT); 
}
