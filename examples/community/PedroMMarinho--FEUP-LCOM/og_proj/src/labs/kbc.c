#include <lcom/lcf.h>

#include "i8042.h"
#include <stdint.h>
#include "kbc.h"

uint8_t tries = 3;

int(kbc_issue_command)(uint8_t command, uint8_t *ret_arg) {
    if (ret_arg == NULL)
        return 1;
    if (kbc_insert_command(command))
        return 1;
    switch (command) {
        case KBC_READ_CMD_BYTE:
            if (read_outputBuffer(ret_arg))
                return 1;
            break;
        case KBC_WRITE_CMD_BYTE:
            if (kbc_write_args(ret_arg))
                return 1;
            break;
        case KBC_SELF_CHECK:
            if (read_outputBuffer(ret_arg))
                return 1;
            break;
        case KBC_CHECK_INTERFACE:
            if (read_outputBuffer(ret_arg))
                return 1;
            break;
        case KBC_ENABLE_INTERFACE:
            break;
        case KBC_DISABLE_INTERFACE:
            break;
        case KBC_WRITE_BYTE_TO_MOUSE:
            if (kbc_write_args(ret_arg))
                return 1;
            if (util_sys_inb(KBC_OUTPUT_BUFFER, ret_arg)) return 1;
            break;
        default:
            return 1;
    }
    return 0;
}

int(kbc_insert_command)(uint8_t command) {
    int wait_times = 3;
    uint8_t st;
    while (wait_times) {
        if (util_sys_inb(KBC_STATUS, &st))
            return 1;
        if ((st & KBC_STATUS_IN_BUF_FULL) == 0) {
            if (sys_outb(KBC_WRITE_COMMAND, command))
                return 1;
            return 0;
        }
        COMMAND_WAIT;
        wait_times--;
    }
    return 1;
}

int(kbc_write_args)(uint8_t *args) {
    int wait_times = 3;
    if (args == NULL)
        return 1;
    uint8_t st;
    while (wait_times) {
        if (util_sys_inb(KBC_STATUS, &st))
            return 1;
        if ((st & KBC_STATUS_IN_BUF_FULL) == 0) {
            if (sys_outb(KBC_COMMAND_ARGS, *args))
                return 1;
            return 0;
        }
        COMMAND_WAIT;
        wait_times--;
    }
    return 1;
}

int read_outputBuffer(uint8_t *data) {
    if (data == NULL)
        return 1;

    u_int8_t try = 0;
    while (try < tries) {
        uint8_t st = 0;
        if (util_sys_inb(KBC_STATUS, &st))
            return 1;
        if ((st & KBC_STATUS_OUT_BUF_FULL)) {
            if (util_sys_inb(KBC_OUTPUT_BUFFER, data))
                return 1;
            if (st & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) {
                // dont know if this is doable
                data = 0;
                return 1;
            };
            return 0;
        }
        try++;
        COMMAND_WAIT;
    }
    return 1;
}
