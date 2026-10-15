#include <lcom/lcf.h>

#include "kbc.h"

uint8_t size = 0;
uint8_t bytes[2] = {0, 0};
bool is_scode_complete = false;

static int hook_id;

void (kbc_ih)() {
    uint8_t scancode;

    if(is_scode_complete) {
        is_scode_complete = false;
        size = 0;
    }

    if (read_kbc_out_buf(&scancode)) {
        is_scode_complete = false;
        size = 0;
        return ;
    }

    if (scancode == FIRST_B_SCODE) {
        size++;
        bytes[0] = FIRST_B_SCODE;
    } else {
        bytes[size] = scancode;
        size++;
        is_scode_complete = true;
    }
}

int (kbc_subscribe_int)(uint8_t *bit_no) {
    hook_id = 1;
    *bit_no = hook_id;

    if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) {
        return 1;
    }

    return 0;
}

int (kbc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id)) {
        return 1;
    }

    return 0;
}

int (read_kbc_config)(uint8_t *st) {
    if (util_sys_inb(KBC_ST_REG, st)){
        return 1;
    }
    
    return 0;
}

int (read_kbc_out_buf)(uint8_t *sc) {
    uint8_t st;
    uint8_t timeout = 0;

    while (timeout < 3) {

        if (read_kbc_config(&st)) {
            timeout++;
            continue;
        }

        if ((st & KBC_ST_OBF) && !(st & KBC_ST_AUX)) {
            if (util_sys_inb(KBC_OUT_BUF, sc)) {
                return 1;
            }

            if (st & (KBC_PAR_ERR | KBC_TO_ERR)) {
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(WAIT_KBC));
        timeout++;
    }

    return 1;
}

int (write_cmd_byte)(int port, uint8_t cmd) {
    uint8_t st;
    uint8_t timeout = 0;

    while (timeout < 3) {
        if (read_kbc_config(&st)) {
            timeout++;
            continue;
        }

        if ((st & KBC_ST_IBF) == 0) {
            if (sys_outb(port, cmd)) {
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(WAIT_KBC));
        timeout++;
    }

    return 1;
}

int (kbc_cmd)(uint8_t cmdb, uint8_t *cmd) {

    switch (cmdb) {
        case READ_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, READ_CMDB)) {
                return 1;
            }
            if (read_kbc_out_buf(cmd)) {
                return 1;
            }
            break;
        case WRITE_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, WRITE_CMDB)) {
                return 1;
            }
            if (write_cmd_byte(KBC_ARG_REG, *cmd)) {
                return 1;
            }
            break;
        case CHKC_KBC_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, CHKC_KBC_CMDB)) {
                return 1;
            }
            if (read_kbc_out_buf(cmd)) {
                return 1;
            }
            break;
        case CHCK_KBD_ITFC_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, CHCK_KBD_ITFC_CMDB)) {
                return 1;
            }
            if (read_kbc_out_buf(cmd)) {
                return 1;
            }
            break;
        case DIS_KBD_ITFC_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, DIS_KBD_ITFC_CMDB)) {
                return 1;
            }
            cmd = NULL;
            break;
        case ENB_KBD_ITFC_CMDB:
            if (write_cmd_byte(KBC_CMD_REG, ENB_KBD_ITFC_CMDB)) {
                return 1;
            }
            cmd = NULL;
            break;

        default:
            printf("Invalid Command");
            return 1;
    }

    return 0;
}
