#include "mouse.h"
#include "../helper/i8042.h"
#include "../helper/structs.h"

#include <lcom/lcf.h>

int mouse_hook_id = 2;
uint8_t packet[3];
int packet_byte_count = 0;
bool lever = false;
extern struct mouse_packet pack;


extern struct Sprite *mouse;


static bool prevlb = false;
static bool prevrb = false;
static bool prevmb = false;



int mouse_int_sub(uint8_t *irq_mask) {
    if (irq_mask == NULL) return 1;
    *irq_mask = BIT(mouse_hook_id);
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) == 0) {
        printf("Mouse subscribe was done!!");
        return 0;
    }
    printf("ERROR SUBSCRIBING!!");
    return 1;
}


int mouse_int_unsub() {
    return sys_irqrmpolicy(&mouse_hook_id); 
}

int mouse_set_reporting(bool value) {
    uint8_t temp;
    if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    while (temp & IN_BUF_FULL) {
        if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    } 
    if (sys_outb(IN_COM_REG, WR_MOUSE_BYTE) != 0) return 1;
    if (value) {
        if (sys_outb(IN_ARG_REG, EN_DATA_REP) != 0) return 1;
    } else {
        if (sys_outb(IN_ARG_REG, DIS_DATA_REP) != 0) return 1;
    }
    
    if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    while (!(temp & OUT_BUF_FULL)) {
        if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    }
    if (util_sys_inb(OUT_REG, &temp) != 0) return 1;
    if (temp == BYTE_ACK) return 0;
    printf("didnt got a ACK...");
    // segunda tentativa por ter dado erro primeiro
    if (sys_outb(IN_COM_REG, WRITE_BYTE) != 0) return 1;
    while (temp & IN_BUF_FULL) {
        if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    }
    if (value) {
        if (sys_outb(IN_ARG_REG, EN_DATA_REP) != 0) return 1;
    } else {
        if (sys_outb(IN_ARG_REG, DIS_DATA_REP) != 0) return 1;
    }
    
    if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    while (!(temp & OUT_BUF_FULL)) {
        if (util_sys_inb(ST_REG, &temp) != 0) return 1;
    }
    if (util_sys_inb(OUT_REG, &temp) != 0) return 1;
    if (temp == BYTE_ACK) return 0;
    else return 1;
}

int mouse_get_packet(struct mouse_packet *pack) {
    for (int i = 0; i < 3; i++) {
        pack->bytes[i] = packet[i]; 
    }

    pack->rb = packet[0] & MOUSE_RB;
    pack->lb = packet[0] & MOUSE_LB;
    pack->mb = packet[0] & MOUSE_MB;

    pack->x_ovflow = packet[0] & X_OVR_FLOW;
    pack->y_ovflow = packet[0] & Y_OVR_FLOW;

    int16_t delta_x = (packet[0] & X_SIGN) ? (int16_t)(0xFF00 | packet[1]) : (int16_t)(packet[1]);
    int16_t delta_y = (packet[0] & Y_SIGN) ? (int16_t)(0xFF00 | packet[2]) : (int16_t)(packet[2]);

    pack->x_pos = delta_x;
    pack->y_pos = -delta_y;

    return 0;
}



bool left_mouse_clicked(struct mouse_packet *pack) {
    bool isClicked = (!prevlb && pack->lb);
    prevlb = pack->lb;
    return isClicked;
}


bool mid_mouse_clicked(struct mouse_packet *pack) {
    bool isClicked = (!prevmb && pack->mb);
    prevmb = pack->mb;
    return isClicked;
}


bool right_mouse_clicked(struct mouse_packet *pack) {
    bool isClicked = (!prevrb && pack->rb);
    prevrb = pack->rb;

    return isClicked;
}



void (mouse_ih)() {
    
    uint8_t temp;

    if (util_sys_inb(ST_REG, &temp) != 0) return;
    if (!(temp & OUT_BUF_FULL) || !(temp & MOUSE_AUX)) return;

    if (util_sys_inb(OUT_REG, &temp) != 0) return;
    if (!lever) {
        if (!(temp & BIT(3))) return;
        lever = true;
    }

    if (packet_byte_count % 3 == 0 && !(temp & BIT(3))) return;

    packet[packet_byte_count%3] = temp;
    packet_byte_count++;

    if (packet_byte_count % 3 == 0) {
        lever = false; 
    }
}


bool mouse_hover(struct Sprite *button) {
    return mouse->x_pos >= button->x_pos &&
           mouse->x_pos <= button->x_pos + button->width &&
           mouse->y_pos >= button->y_pos &&
           mouse->y_pos <= button->y_pos + button->height;
}


