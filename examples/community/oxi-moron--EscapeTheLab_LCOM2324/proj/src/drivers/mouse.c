#include "mouse.h"

int hook_id4 = MOUSE_BIT_NO;
int currentByte = 0;
struct packet mousePacket;


int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id4);
    return sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id4);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id4);
}

void (mouse_ih)() {
    uint8_t res;
    uint8_t status;
    int n = MAX_ATTEMPTS;

    while (n != 0) {
        if (util_sys_inb(KBD_STATUS_REG, &status) != 0) return;
        if ((status & 0x01) == 1) {
            if (util_sys_inb(KBD_OUT_BUF, &res) != 0) return;
            if (currentByte == 0 && ((res & 0x08) == 0x08)) {
                mousePacket.bytes[currentByte] = res;
                mousePacket.rb = (res & 0x02) >> 1;
                mousePacket.mb = (res & 0x04) >> 2;
                mousePacket.lb = res & 0x01;
                mousePacket.delta_x = (((res & 0x10) >> 4) == 1) ? 0xFF00 : 0x0000;
                mousePacket.delta_y = (((res & 0x20) >> 5) == 1) ? 0xFF00 : 0x0000;
                mousePacket.x_ov = (res & 0x40) >> 6;
                mousePacket.y_ov = (res & 0x80) >> 7;
                currentByte++;
                return;
            } else if (currentByte != 0) {
                mousePacket.bytes[currentByte] = res;
                if (currentByte == 1) {
                    mousePacket.delta_x |= res;
                } else {
                    mousePacket.delta_y |= res;
                }
                currentByte++;
                return;
            }
        }
        n--;
        tickdelay(micros_to_ticks(WAIT_KBD));
    }
}

int writeCommand(uint8_t command) {
    uint8_t res;
    int n = MAX_ATTEMPTS;

    while (n != 0) {
        if (writeToKBC(KBD_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
        if (writeToKBC(KBD_ARG_REG, command) != 0) return 1;
        if (util_sys_inb(KBD_OUT_BUF, &res) != 0) return 1;
        if (res == ACK) return 0;
        if (res == ERROR) {
            printf("Error code: %x", res);
            return 1;
        }
        n--;
    }
    return 1;
}

int (writeToKBC)(uint8_t port, uint8_t command) {
    uint8_t status;
    int n = MAX_ATTEMPTS;

    while (n != 0) {
        if (util_sys_inb(KBD_STATUS_REG, &status) != 0) return 1;
        if ((status & 0x02) == 0) {
            if (sys_outb(port, command) != 0) return 1;
            return 0;
        }
        n--;
        tickdelay(micros_to_ticks(WAIT_KBD));
    }
    return 1;
}
