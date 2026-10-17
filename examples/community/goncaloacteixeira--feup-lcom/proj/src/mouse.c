//
// Created by skidr on 29/10/2019.
//

#include "mouse.h"

uint16_t byte_packet[3];
uint32_t byte;
struct packet prPacket;

static int32_t hookID = 12;

int mouse_subscribe_int(uint32_t *bit_no) {
    (*bit_no) |= BIT(hookID);
    if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) == OK) {
        return 0;
    } else {
        printf("Error::sys_irqsetpolicy(KBD_ID, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID)\n");
        return 1;
    }
}

int mouse_unsubscribe_int(void) {
    if (sys_irqrmpolicy(&hookID) == OK) {
        return 0;
    }
    else {
        printf("Error::sys_irqrmpolicy(&hookID)\n");
        return 1;
    }
}

void (mouse_ih)(void) {
    uint32_t st;
    if (sys_inb(ST_REG, &st) != 0) {
        printf("Error::sys_inb(ST_REG, &st)\n");
        return;
    }
    if (st & OB_FULL)
        if ((st & (TIMEOUT_ERROR | PRT_ERROR)) == 0)
            if (sys_inb(BUF_OUT, &byte) != 0) {
                printf("Error::sys_inb(BUF_OUT, &byte)\n");
                return;
            }
}

void process_packet(void) {

    // Masks to fill the struct

    if (byte_packet[0] & LB)
        prPacket.lb = true;
    else
        prPacket.lb = false;

    if (byte_packet[0] & RB)
        prPacket.rb = true;
    else
        prPacket.rb = false;

    if (byte_packet[0] & MB)
        prPacket.mb = true;
    else
        prPacket.mb = false;

    if (byte_packet[0] & X_OV)
        prPacket.x_ov = true;
    else
        prPacket.x_ov = false;

    if (byte_packet[0] & Y_OV)
        prPacket.y_ov = true;
    else
        prPacket.y_ov = false;

    if (byte_packet[0] & X_NEG)
        prPacket.delta_x = byte_packet[1] - 256;
    else
        prPacket.delta_x = byte_packet[1];

    if (byte_packet[0] & Y_NEG)
        prPacket.delta_y = byte_packet[2] - 256;
    else
        prPacket.delta_y = byte_packet[2];

    // assign the bytes
    prPacket.bytes[0] = byte_packet[0];
    prPacket.bytes[1] = byte_packet[1];
    prPacket.bytes[2] = byte_packet[2];
}

int mouse_issue_cmd(uint32_t command) {
    uint32_t st;
    uint32_t ctr = 0;

    while (ctr != ACK) {
        if (sys_inb(ST_REG, &st) != 0)
            return 1;

        if ((st & IB_FULL) == 0) {
            if (sys_outb(CMD_REG, CMD_INIT) != 0)
                return 1;
        } else
            continue;

        if (sys_inb(ST_REG, &st) != 0)
            return 1;

        if ((st & IB_FULL) == 0) {
            if (sys_outb(BUF_OUT, command) != 0)
                return 1;
        }
        else
            continue;

        tickdelay(micros_to_ticks(DELAY_US));

        if (sys_inb(BUF_OUT, &ctr) != 0)
            return 1;
    }
    return 0;
}

void process_remote(void) {
    // need to verify the status of the OutBuffer
    uint32_t st;
    if (sys_inb(ST_REG, &st) != 0)
    {
        printf("error::sys_inb(ST_REG, &st)\n");
        return;
    }
    if (st & OB_FULL) {
        if ((st & ( BIT(7) | BIT(6) )) == 0)
        {
            for (int i = 0; i < 3; i++)
            {
               if (sys_inb(BUF_OUT, &byte) != 0)
               {
                   printf("error::sys_inb(BUF_OUT, &byte)\n");
                   return;
               }

               if (i == 0)
                   if (byte & CHECK_BYTE1)
                       byte_packet[0] = byte;
               if (i == 1)
                   byte_packet[1] = byte;
               if (i == 2)
                   byte_packet[2] = byte;
            }
        }
    }
}

state_t state = I1;

void check_line(event_t *evt) {
    switch (state) {
        case I1:
            if (*evt == MOV1)
                state = D1;
            break;
        case D1:
            if (*evt == B)
                state = L1;
            else if (*evt == A1 || *evt == LB_UP)
                state = I1;
            break;
        case L1:
            if (*evt == LB_UP)
                state = I2;
            else if (*evt == A1)
                state = I1;
            break;
        case I2:
            if (*evt == MOV2)
                state = D2;
            else if (*evt == LB)
                state = I1;
            break;
        case D2:
            if (*evt == A2 || *evt == RB_UP)
                state = I1;
            else if (*evt == B)
                state = L2;
            break;
        case L2:
            if (*evt == A2)
                state = I1;
            else if (*evt == RB_UP)
                state = C;
            break;
        case C:
            break;
    }
}

