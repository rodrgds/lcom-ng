#include "KBC.h"

int (KBC_get_status)(uint8_t *status) {
  return (util_sys_inb(KBD_STAT_REG, status) != 0);
}

int (KBC_write_cmd_byte)(uint8_t port, uint8_t cmd){
    uint8_t command;
    if (KBC_get_status(&command) != 0) return 1;

    if ((command & FULL_IN_BUF) != 0) return 1;

    if (sys_outb(port, cmd) != 0) return 1;

    return 0;
}

int (KBC_reset)(){
    uint8_t command;

    if (KBC_write_cmd_byte(KBD_CMD_REG, 0x20) != 0) return 1;
    util_sys_inb(KBD_OUT_BUF, &command);

    command = command | BIT(0);

    if (KBC_write_cmd_byte(KBD_IN_BUF, KBC_RESET) != 0) return 1;
    if (KBC_write_cmd_byte(KBD_OUT_BUF, command) != 0) return 1;

    return 0;
}

int (KBC_read_output)(uint8_t port, uint8_t *cmd) {
    uint8_t status;

    if (KBC_get_status(&status) != 0) printf("Error reading status\n");

    if ((status & FULL_OUT_BUF) != 0) {
        if (util_sys_inb(port, cmd) != 0) {
            return 1;
        }
        if (((status & PAR_ERR) != 0) || ((status & TO_ERR) != 0)) {
            return 1;
        }
        return 0;
    }
    return 1;
}
