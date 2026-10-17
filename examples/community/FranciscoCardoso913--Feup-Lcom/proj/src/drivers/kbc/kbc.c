#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "kbc.h"
#include "keyboard.h"
#include "mouse.h"

int (kbc_get_status)(uint8_t* st) {
    return util_sys_inb(KBC_ST_REG, st);
}

int (kbc_read_data)(uint8_t* data) {
    return util_sys_inb(KBC_OUT, data);
}

int (kbc_write)(uint8_t port, uint8_t cmd) {
    uint8_t st, wait = 5;

    while (wait--) {
        if (kbc_get_status(&st) || st & KBC_IN_FULL) {
            tickdelay(WAIT_KBC);
            continue;
        }

        sys_outb(port, cmd);
        return 0;
    }
    return 1;
}

int (kbc_write_cmd)(uint8_t cmd) {
    return kbc_write(KBC_IN, cmd);
}

int (kbc_write_arg)(uint8_t arg) {
    return kbc_write(KBC_IN_ARGS, arg);
}

int (buffer_full)(uint8_t flag) {
    uint8_t status, tries = 10;

    while (tries--) {
        if (kbc_get_status(&status)) {
            continue;
        }
        if (status & flag) {
            return 1;
        }
    }
    return 0;
}

int (ibf_empty)() {
    return !buffer_full(KBC_IN_FULL);
}

int (obf_full)() {
    return buffer_full(KBC_OUT_FULL);
}

int (kbc_init)(uint8_t* irq_keyboard, uint8_t* irq_mouse) {
    if (keyboard_subscribe_int(irq_keyboard) != OK) {
        printf("Error subscribing keyboard interrupts.\n");
        return 1;
    }

    if (mouse_enable_dr() != OK) {
        printf("Error enabling data reporting.\n");
        return 1;
    }

    if (mouse_subscribe_int(irq_mouse) != OK) {
        printf("Error subscribing mouse interrupts.\n");
        return 1;
    }

    return 0;
}

int (kbc_disable)() {
    if (mouse_disable_dr() != OK) {
        printf("Error disabling data reporting.\n");
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        printf("Error unsubscribing mouse interrupts.\n");
        return 1;
    }

    if (keyboard_unsubscribe_int()) {
        printf("Error unsubscribing keyboard interrupts.\n");
        return 1;
    }

    return 0;
}
