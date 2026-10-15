#include <lcom/lcf.h>
#include "mouse.h"

struct packet pp;
uint8_t mouse_counter = 0;
int x = 10, y = 10;

static int hook_id;
static uint8_t byte;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    hook_id = 12;
    *bit_no = hook_id;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) {
        return 1;
    }

    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id)) {
        return 1;
    }

    return 0;
}

int (mouse_enable_stream_mode)() {
    if (mouse_cmd(WRITE_B_TO_MOUSE_CMDB, ENABLE_STREAM_MODE_CMDB)) {
        return 1;
    }

    return 0;
}

int (mouse_enable_dt_reporting)() {
    if (mouse_cmd(WRITE_B_TO_MOUSE_CMDB, ENABLE_DATA_REPORT_CMDB)) {
        return 1;
    }

    return 0;
}

int (mouse_disable_dt_reporting)() {
    if (mouse_cmd(WRITE_B_TO_MOUSE_CMDB, DISABLE_DATA_REPORT_CMDB)) {
        return 1;
    }

    return 0;
}


int (mouse_read_out_buf)(uint8_t *output) {
    uint8_t st;
    int timeout = 0;

    while (timeout < 10) {
        if (mouse_read_config(&st)) {
            timeout++;
            continue;
        }

        if ((st & MOUSE_ST_OBF)) { 
            if (util_sys_inb(MOUSE_OUT_BUF, output)) {
                return 1;
            }

            if (st & (MOUSE_PAR_ERR | MOUSE_TO_ERR)) {
                return 1;
            }

            return 0;
        }
        timeout++;
        tickdelay(micros_to_ticks(WAIT_MOUSE));
    }

    return 1;
}

int (mouse_read_config)(uint8_t *st) {
    if (util_sys_inb(MOUSE_ST_REG, st)) {
        return 1;
    }
    
    return 0;
}

int (mouse_write_cmdb)(int port, uint8_t cmdb) {
    uint8_t st;
    int timeout = 0;

    while (timeout < 10) {
        if (mouse_read_config(&st)) {
            timeout++;
            continue;
        }

        while((st & MOUSE_ST_IBF)) {};

        if (st & ( MOUSE_TO_ERR | MOUSE_PAR_ERR)) {
            return 1;
        }
    
        if (sys_outb(port, cmdb)) {
            timeout++;
            continue;
        }

        return 0;
        
    }

    return 1;
}

int (mouse_cmd)(uint8_t cmd, uint8_t cmdb) {
    uint8_t ack_byte = MOUSE_NACK;

    switch (cmd) {
        case WRITE_B_TO_MOUSE_CMDB:
            while (ack_byte != MOUSE_ACK) {
                if (mouse_write_cmdb(MOUSE_CMD_REG, cmd)) {
                    return 1;
                }

                if (mouse_write_cmdb(MOUSE_ARG_REG, cmdb)) {
                    return 1;
                }

                tickdelay(micros_to_ticks(WAIT_MOUSE));

                if (util_sys_inb(MOUSE_OUT_BUF, &ack_byte)) {
                    return 1;
                }
            }
            break;
    }

    return 0;
}

void (mouse_parse_packet)() {
    if ((mouse_counter == 0) && !(byte & MOUSE_BIT_SYNC)){
        return ;
    }

    pp.bytes[mouse_counter] = byte;

    switch (mouse_counter) {
        case 0:
            pp.rb = byte & MOUSE_RB;
            pp.lb = byte & MOUSE_LB;
            pp.mb = byte & MOUSE_MB;
            pp.x_ov = byte & MOUSE_X_OV;
            pp.y_ov = byte & MOUSE_Y_OV;

            pp.delta_x = (byte & MOUSE_X_MSB);
            pp.delta_y = (byte & MOUSE_Y_MSB);
            break;
        case 1:
            if (pp.delta_x) {
                pp.delta_x = 0xFF00;
                pp.delta_x |= byte;
            } else {
                pp.delta_x = byte;
            }
            break;
        case 2:
            if (pp.delta_y){
                pp.delta_y = 0xFF00;
                pp.delta_y |= byte; 
            } else {
                pp.delta_y = byte;
            }
            break;
    }

    mouse_counter++;
}

void (mouse_ih)() {
    if (mouse_read_out_buf(&byte)) {
        mouse_counter = 0;
    }
}

void (mouse_update_position)() {
    if (x + pp.delta_x < 0 && !pp.x_ov) {
        x = 0;
    } else if (x + pp.delta_x > 800 && !pp.x_ov) {
        x = 800;
    } else if (!pp.x_ov) {
        x += pp.delta_x;
    }

    if (y - pp.delta_y < 0 && !pp.y_ov) {
        y = 0;
    } else if (y - pp.delta_y > 600 && !pp.y_ov) {
        y = 600;
    } else if (!pp.y_ov) {
        y -= pp.delta_y;
    }
}
