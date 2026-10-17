#include <lcom/lcf.h>
#include "mouse.h"

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"

int hook_id_ms = MOUSE_IRQ;
uint8_t byte;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) { //checks if the pointer is valid
        printf("Pointer bit_no is invalid.\n");
        return 1;
    }

    *bit_no = hook_id_ms;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_ms)) {
        printf("Error subscribing mouse interrupts.\n");
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_id_ms)) {
        printf("Failed unsubscribing the mouse interrupts.\n");
        return 1;
    }
    return 0;
}

void (mouse_ih)() {
    uint8_t stataux;
    if(util_sys_inb(STAT_REG, &stataux)) {
        printf("Unable to read the status reg\n");
        return;
    }
    if(stataux & OBF) {
        if(util_sys_inb(OUT_BUF, &byte)) return;
        if(stataux & (TO_ERR | PAR_ERR) || !(stataux & STAT_AUX)) byte = 0;
    }
    else byte = 0;
}

void (packet_parser)(struct packet *pp, uint8_t *packetBytes) {
    pp->bytes[0] = packetBytes[0];
    pp->bytes[1] = packetBytes[1];
    pp->bytes[2] = packetBytes[2];
    pp->lb = (packetBytes[0] & LB);
    pp->rb = (packetBytes[0] & RB);
    pp->mb = (packetBytes[0] & MB);
    pp->x_ov = (packetBytes[0] & X_OV);
    pp->y_ov = (packetBytes[0] & Y_OV);
    pp->delta_x = (uint16_t)packetBytes[1];
    pp->delta_y = (uint16_t)packetBytes[2];

    if((packetBytes[0] & X_SIGN)) pp->delta_x |= (0xFF<<8);
    if((packetBytes[0] & Y_SIGN)) pp->delta_y |= (0xFF<<8);
}

int write_mouse_cmd(uint32_t cmd) {
    uint8_t stataux, ret;
    bool condition = true;
    while(condition) {
        if(util_sys_inb(STAT_REG, &stataux)) return 1;
        if(stataux & IBF) continue;
        else if(sys_outb(KBC_CMD_REG, KBC_WRITE_MOUSE)) return 1;
        if(util_sys_inb(STAT_REG, &stataux)) return 1;
        if (stataux & IBF) continue;
        else if(sys_outb(OUT_BUF, cmd)) return 1;
        tickdelay(micros_to_ticks(WAIT_KBC));
        if(util_sys_inb(OUT_BUF, &ret)) return 1;
        condition = (ret != ACK);
    }
    return 0;
}


