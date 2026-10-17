#include "kbc.h"

int (kbc_get_status)(uint8_t *st) {
    if (util_sys_inb(KBC_STAT_REG, st) != 0) return 1;

    return 0; 
}

int (kbc_read_data)(uint8_t port, uint8_t *output, bool mouse) {
    uint8_t status;
    uint8_t attempts = 5;

    while (attempts) {
        if (kbc_get_status(&status) != 0) return 1;

        if (status & KBC_OBF) {
            if (util_sys_inb(port, output) != 0) return 1;

            if ((status & KBC_PARITY) || (status & KBC_TIMEOUT)) return 1;

            if (mouse && !(status & KBC_AUX)) return 1;

            if (!mouse && (status & KBC_AUX)) return 1;

            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;
    }

    return 1;
}

int(kbc_write_cmd)(uint8_t port, uint8_t cmd) {
    uint8_t status;
    uint8_t attempts = 5;

    while (attempts) {
        if (kbc_get_status(&status) != 0) return 1;

        if ((KBC_IBF & status) == 0) {
            if (sys_outb(port, cmd) != 0) return 1;
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;
    } 

    return 1;
}
