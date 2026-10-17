#include "keyboard.h"

int kbd_hook_id = 1;
uint8_t scancode = 0;

int keyboard_subscribe_int(uint8_t *bit_no) {
    *bit_no = BIT(kbd_hook_id);

    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int keyboard_unsubscribe_int() {
    return sys_irqrmpolicy(&kbd_hook_id);
}

void keyboard_ih() {
    kbc_read_data(KBC_OUT_BUF, &scancode, false);
}

int keyboard_restore() {
    uint8_t cmd;

    if (kbc_write_cmd(KBC_IN_BUF_CMD, KBC_R_CMD_B) != 0) return 1;
    if (kbc_read_data(KBC_OUT_BUF, &cmd, false) != 0) return 1;

    cmd |= KBC_INT;

    if (kbc_write_cmd(KBC_IN_BUF_CMD, KBC_W_CMD_B) != 0) return 1;
    if (kbc_write_cmd(KBC_IN_BUF_ARG, cmd) != 0) return 1;

    return 0;
}
