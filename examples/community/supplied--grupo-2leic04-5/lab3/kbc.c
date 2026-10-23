#include "kbc.h"
static int hook = 1;
static uint8_t scancode;
static bool error;
int kbd_subscribe_int(uint8_t *bit_no) {
    if (!bit_no) return 1;
    *bit_no = (uint8_t)hook;
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}
int kbd_unsubscribe_int(void) { return sys_irqrmpolicy(&hook); }
void kbc_ih(void) {
    uint32_t status, data;
    error = sys_inb(0x64, &status) != 0 || !(status & BIT(0)) ||
            (status & (BIT(5) | BIT(6) | BIT(7))) != 0 ||
            sys_inb(0x60, &data) != 0;
    if (!error) scancode = (uint8_t)data;
}
bool kbc_has_error(void) { return error; }
uint8_t kbc_get_scancode(void) { return scancode; }
