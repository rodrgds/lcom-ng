#include "keyboard.h"

static lcom_irq_t keyboard_irq;
static uint8_t scancode;
static bool keyboard_error;
static bool keyboard_subscribed;

uint8_t get_scancode(void) {
    return scancode;
}

bool get_kbd_error(void) {
    return keyboard_error;
}

int kbd_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    if (lcom_irq_subscribe(KBC_IRQ, 0, &keyboard_irq) != LCOM_OK) return 1;
    keyboard_subscribed = true;
    *bit_no = keyboard_irq.bit_no;
    return 0;
}

int kbd_unsubscribe_int(void) {
    if (!keyboard_subscribed) return 0;
    keyboard_subscribed = false;
    return lcom_irq_unsubscribe(&keyboard_irq) == LCOM_OK ? 0 : 1;
}

void kbc_ih(void) {
    uint8_t status = 0;
    keyboard_error = false;
    if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK ||
        (status & KBC_ST_OBF) == 0 ||
        (status & KBC_ST_AUX) != 0) {
        keyboard_error = true;
        return;
    }
    if (lcom_port_read8(KBC_OUT_BUF, &scancode) != LCOM_OK ||
        (status & KBC_ST_ERR) != 0) {
        keyboard_error = true;
    }
}
