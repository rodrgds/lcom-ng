#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "mouse.h"
#include "i8042.h"
#include "utils.h"


int mouse_hook_id;
uint8_t byte;
uint8_t packet[3];
struct packet pp;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    mouse_hook_id = MOUSE_IRQ;
    *bit_no = mouse_hook_id;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) == OK) {
        return 0;
    }
    else {
        printf("sys_irqsetpolicy() failed\n");
        return 1;
    }
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != OK) {
        printf("sys_irqmpolicy() failed\n");
        return 1;
    }
    return 0;
}

int write_cmd_kbd() {
    int try = 0;
    uint8_t st;
    while (try < 4) {
        if (util_sys_inb(KBC_ST_REG, &st) != OK) {
            printf ("Error in util_sys_inb()\n");
            return 1;
        }
        if (!(st & IBFULL)) {
            if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK) {
                printf("Error in sys_outb()\n");
                return 1;
            }
            return 0;
        }
        try++;
        tickdelay(micros_to_ticks(DELAY_US));        
    }
    printf("To many failed tries.\n");
    return 1;

}


int mouse_enable_data_reporting2(uint32_t cmd2) {
    uint8_t ack = 0;
    int try = 0;
    uint8_t st;    
    while (ack != ACK) {
        if (write_cmd_kbd() != OK) {
            printf("Error in write command");
            return 1;
        }
        while (try < 4) {
            if (util_sys_inb(KBC_ST_REG, &st) != OK) {
                printf ("Error in util_sys_inb()\n");
                return 1;
            }

            if (!(st & IBFULL)) {
                if (sys_outb(KBC_OUT_BUF, cmd2) != OK) {
                    printf("Error in sys_outb()\n");
                    return 1;
                }

                tickdelay(micros_to_ticks(DELAY_US));

                if (util_sys_inb(KBC_OUT_BUF, &ack) != OK) {
                    printf("Error in sys_outb()\n");
                    return 1;
                }
                printf("%x\n", ack);
                break;
            }

            try++;
            tickdelay(micros_to_ticks(DELAY_US));
            
        }
        if (try == 4) {
            printf("To many failed tries.\n");
            return 1;            
        }

        if (ack == NACK) {
            continue;
        }
        else if (ack == ERROR) {
            return 1;
        }
    }

    return 0;

}

void (mouse_ih)() {
    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != OK) {
        printf("Error in util_sys_inb()\n");
        return;
    }
    if (status & OBFULL) {
        if ((status & (PARITY_ERROR | TIMEOUT_ERROR)) == 0) {
            if (util_sys_inb(KBC_OUT_BUF, &byte) != OK) {
                printf("Error in util_sys_inb()\n");
           
            }
        return;     
        }
       
    }
    tickdelay(micros_to_ticks(DELAY_US));
}

void packet_make() {
    pp.bytes[0] = packet[0];
    pp.bytes[1] = packet[1];
    pp.bytes[2] = packet[2];
    pp.rb = (packet[0] & RB);
    pp.mb = (packet[0] & MB);
    pp.lb = (packet[0] & LB);
    pp.x_ov = (packet[0] & X_OVFL);
    pp.y_ov = (packet[0] & Y_OVFL);
    if (packet[0] & MSB_X) {
        pp.delta_x = packet[1] - 256;
    }
    else {
        pp.delta_x = packet[1];
    }
    if (packet[0] & MSB_Y) {
        pp.delta_y = packet[2] - 256;
    }
    else {
        pp.delta_y = packet[2];
    }
}
