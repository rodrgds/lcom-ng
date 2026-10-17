#include <lcom/lcf.h>
#include "packet.h"
#include "i8042.h"

void (resetPacket)(struct packet *pp){
    pp->bytes[0] = 0x00;
    pp->bytes[1] = 0x00;
    pp->bytes[2] = 0x00;
    pp->delta_x = 0x0000;
    pp->delta_y = 0x0000;    
}

void (parseToPacket)(int byteCounter, uint32_t data, struct packet *pp){
    int byte = byteCounter % 3;
    uint16_t sign;

    if (byte == 1){
        pp->bytes[0] = data;
        pp->lb = data & LB;
        pp->rb = data & RB;
        pp->mb = data & MB;
        pp->x_ov = data & X_OVF;
        pp->y_ov = data & Y_OVF;
        if (data & X_SIGN)
            pp->delta_x = 1; // IF 1 THEN delta-x is negative
        else pp->delta_x = 0;
        if (data & Y_SIGN)
            pp->delta_y = 1; // IF 1 THEN delta-y is negative
        else pp->delta_y = 0;
    }
    else if (byte == 2){
        sign = pp->delta_x;
        pp->bytes[1] = data;
        pp->delta_x = data;

        if (sign){ // delta-x is negative
            pp->delta_x = pp->delta_x | 0xff00; // sign extention
        }
    }
    else if (byte == 0){
        sign = pp->delta_y;
        pp->bytes[2] = data;
        pp->delta_y = data;

        if (sign){ // delta-y is negative
            pp->delta_y = pp->delta_y | 0xff00; // sign extention
        }

    }
}

