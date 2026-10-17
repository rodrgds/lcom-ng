#include <lcom/lcf.h>
#include <lcom/lab4.h>

#include <stdint.h>
#include "mouse.h"
#include "i8042.h"

int mouse_hook_id = MOUSE_IRQ;
uint8_t mouseBytes[4];
uint8_t mouseIndexArray = 0;
uint8_t stat;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = mouse_hook_id;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) {
        printf("sys_irqsetpolicy failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) {
        printf("sys_irqrmpolicy failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}

void (mouse_ih)() {
    uint8_t byte;

    if (util_sys_inb(KBC_CMD_REG, &stat) != OK) mouseIndexArray = 0;

    else if (util_sys_inb(KBC_OUT_BUF, &byte) != OK) mouseIndexArray = 0;

    else if (stat & (KBC_PAR_ERR | KBC_TO_ERR)) mouseIndexArray = 0;

    else if ((mouseIndexArray == 0) && ((byte & VALIDATION_BIT) == 0)) {
        mouseIndexArray = 0;
    }

    else {
        mouseBytes[mouseIndexArray] = byte;
        mouseIndexArray++;
    }
}

bool (mouse_packet_is_done)() {
    if (mouseIndexArray == 4) {
        mouseIndexArray = 0;
        return true;
    }
    return false;
}

void (mouse_get_packet)(struct mousePacket *pp) {
    pp->bytes[0] = mouseBytes[0];
    pp->bytes[1] = mouseBytes[1];
    pp->bytes[2] = mouseBytes[2];
    pp->rb = mouseBytes[0] & RIGHT_BUTTON;
    pp->mb = mouseBytes[0] & MIDDLE_BUTTON;
    pp->lb = mouseBytes[0] & LEFT_BUTTON;
    pp->delta_x = (mouseBytes[0] & MSB_X_DELTA) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1];
    pp->delta_y = (mouseBytes[0] & MSB_Y_DELTA) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2];
    pp->x_ov = mouseBytes[0] & X_OVF;
    pp->y_ov = mouseBytes[0] & Y_OVF;
    pp->delta_scroll = mouseBytes[3];
}

int (kbc_write_command)(uint8_t cmd) {
    int tries = MAX_TRIES;

    while (tries) {
        if (util_sys_inb(KBC_CMD_REG, &stat) != OK) {
            printf("util_sys_inb failed inside %s\n", __func__);
            return EXIT_FAILURE;
        }

        if ((stat & KBC_IBF) == OK) {
            if (sys_outb(KBC_CMD_REG, cmd) != OK) {
                printf("sys_outb failed inside %s\n", __func__);
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }

        tries--;
        tickdelay(micros_to_ticks(DELAY));
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

int (write_byte_to_mouse)(uint8_t byte) {
    int tries = MAX_TRIES;
    uint8_t ack;

    while (tries) {
        if (kbc_write_command(WRITE_BYTE_MOUSE) != OK) {
            printf("kbc_write_command failed inside %s", __func__);
            return EXIT_FAILURE;
        }

        tickdelay(micros_to_ticks(DELAY));

        if (util_sys_inb(KBC_CMD_REG, &stat) != OK) {
            printf("util_sys_inb failed inside %s", __func__);
            return EXIT_FAILURE;
        }

        if ((stat & KBC_IBF) == OK) {
            if (sys_outb(KBC_OUT_BUF, byte) != OK) {
                printf("sys_outb failed inside %s", __func__);
                return EXIT_FAILURE;
            }

            tickdelay(micros_to_ticks(DELAY));

            if (util_sys_inb(KBC_OUT_BUF, &ack) != OK) {
                printf("util_sys_inb failed inside %s", __func__);
                return EXIT_FAILURE;
            }

            if (ack == ACK) return EXIT_SUCCESS;
        }

        tries--;
        tickdelay(micros_to_ticks(DELAY));
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

int (mouse_enable_data_reporting_mine)() {
    int tries = MAX_TRIES;

    while (tries) {
        if (write_byte_to_mouse(MOUSE_ENABLE_DATA_REPORT) == OK) {
            return EXIT_SUCCESS;
        }

        else {
            tries--;
            tickdelay(micros_to_ticks(DELAY));
        }
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

int (mouse_disable_data_reporting)() {
    int tries = MAX_TRIES;

    while (tries) {
        if (write_byte_to_mouse(MOUSE_DISABLE_DATA_REPORT) == OK) {
            return EXIT_SUCCESS;
        }

        else {
            tries--;
            tickdelay(micros_to_ticks(DELAY));
        }
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

int (mouse_set_sample_rate)(uint8_t rate) {
    int tries = MAX_TRIES;

    while (tries) {
        tries--;

        if (write_byte_to_mouse(MOUSE_SET_SAMPLE_RATE) != 0) continue;

        tickdelay(micros_to_ticks(DELAY));

        if (write_byte_to_mouse(rate) == OK) return EXIT_SUCCESS;
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

// Get Device ID replies with an ACK followed by a separate device ID byte.
int (write_get_device_id_to_mouse)(uint8_t* id) {
    int tries = MAX_TRIES;

    while (tries) {
        if (kbc_write_command(WRITE_BYTE_MOUSE) != OK) {
            printf("kbc_write_command failed inside %s\n", __func__);
            return EXIT_FAILURE;
        }

        tickdelay(micros_to_ticks(DELAY));

        if (util_sys_inb(KBC_CMD_REG, &stat) != OK) {
            printf("util_sys_inb failed inside %s\n", __func__);
            return EXIT_FAILURE;
        }


        if ((stat & KBC_IBF) == OK) {
            if (sys_outb(KBC_OUT_BUF, MOUSE_GET_DEVICE_ID) != OK) {
                printf("sys_outb failed inside %s\n", __func__);
                return EXIT_FAILURE;
            }

            tickdelay(micros_to_ticks(DELAY));

            uint8_t response;
            if (util_sys_inb(KBC_OUT_BUF, &response) == OK && response == ACK) {
                tickdelay(micros_to_ticks(DELAY));
                if (util_sys_inb(KBC_OUT_BUF, id) == OK) return EXIT_SUCCESS;
            }
        }

        tries--;
        tickdelay(micros_to_ticks(DELAY));
    }

    printf("%s failed all tries\n", __func__);
    return EXIT_FAILURE;
}

int (get_mouse_id)(uint8_t* id) {
    int tries = MAX_TRIES;

    while (tries) {
        tries--;

        if (write_get_device_id_to_mouse(id) != OK) continue;

        tickdelay(micros_to_ticks(DELAY));

        tries = 1;
        break;
    }

    if (!tries) {
        printf("%s failed all tries\n", __func__);
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}

int (mouse_enable_scrolling)(){
    uint8_t id = 0;

    if (mouse_set_sample_rate(200) != OK) {
        printf("mouse_set_sample_rate failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    if (mouse_set_sample_rate(100) != OK) {
        printf("mouse_set_sample_rate failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    if (mouse_set_sample_rate(80) != OK) {
        printf("mouse_set_sample_rate failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    if (get_mouse_id(&id) != OK) {
        printf("get_mouse_id failed inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    if (id != INTELLIMOUSE) {
        printf("%s failed\n", __func__);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
