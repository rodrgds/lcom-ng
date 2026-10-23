#include "mouse.h"
static int hook = 12;
static uint8_t bytes[3];
static unsigned count;
static bool ready;
static int reporting(uint8_t command) {
    uint32_t status, ack;
    if (sys_inb(0x64, &status) || (status & (BIT(0) | BIT(1))) ||
        sys_outb(0x64, 0xd4) || sys_outb(0x60, command)) return 1;
    if (sys_inb(0x64, &status) || !(status & BIT(0)) ||
        sys_inb(0x60, &ack) || ack != 0xfa) return 1;
    return 0;
}
int my_mouse_enable_data_reporting(void) { return reporting(0xf4); }
int my_mouse_disable_data_reporting(void) { return reporting(0xf5); }
int mouse_subscribe_int(uint8_t *bit_no) {
    if (!bit_no) return 1;
    *bit_no = (uint8_t)hook;
    return sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}
int mouse_unsubscribe_int(void) { return sys_irqrmpolicy(&hook); }
void mouse_ih(void) {
    uint32_t status, data;
    if (sys_inb(0x64, &status) || !(status & BIT(0)) || !(status & BIT(5)) ||
        (status & (BIT(6) | BIT(7))) || sys_inb(0x60, &data)) return;
    uint8_t byte = (uint8_t)data;
    if (!count && !(byte & BIT(3))) return;
    bytes[count++] = byte;
    if (count == 3) { count = 0; ready = true; }
}
bool mouse_packet_ready(void) { return ready; }
void mouse_parse_packet(struct packet *packet) {
    if (!packet) return;
    for (unsigned i = 0; i < 3; ++i) packet->bytes[i] = bytes[i];
    packet->lb = !!(bytes[0] & BIT(0));
    packet->rb = !!(bytes[0] & BIT(1));
    packet->mb = !!(bytes[0] & BIT(2));
    packet->x_ov = !!(bytes[0] & BIT(6));
    packet->y_ov = !!(bytes[0] & BIT(7));
    packet->delta_x = (int16_t)(bytes[1] | ((bytes[0] & BIT(4)) ? 0xff00 : 0));
    packet->delta_y = (int16_t)(bytes[2] | ((bytes[0] & BIT(5)) ? 0xff00 : 0));
}
void mouse_clear_packet_flag(void) { ready = false; }
