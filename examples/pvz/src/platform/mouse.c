#include "mouse.h"

static lcom_irq_t mouse_irq;
static uint8_t mouse_byte;
static bool mouse_error;
static bool mouse_subscribed;

uint8_t get_mouse_byte(void) {
    return mouse_byte;
}

bool get_mouse_error(void) {
    return mouse_error;
}

int mouse_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    if (lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse_irq) != LCOM_OK) return 1;
    mouse_subscribed = true;
    *bit_no = mouse_irq.bit_no;
    return 0;
}

int mouse_unsubscribe_int(void) {
    if (!mouse_subscribed) return 0;
    mouse_subscribed = false;
    return lcom_irq_unsubscribe(&mouse_irq) == LCOM_OK ? 0 : 1;
}

void mouse_ih(void) {
    uint8_t status = 0;
    mouse_error = false;
    if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK ||
        (status & KBC_ST_OBF) == 0 ||
        (status & KBC_ST_AUX) == 0) {
        mouse_error = true;
        return;
    }
    if (lcom_port_read8(KBC_OUT_BUF, &mouse_byte) != LCOM_OK ||
        (status & KBC_ST_ERR) != 0) {
        mouse_error = true;
    }
}

int mouse_write_command(uint8_t command) {
    if (lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) != LCOM_OK) return 1;
    if (lcom_port_write8(KBC_IN_BUF, command) != LCOM_OK) return 1;
    return 0;
}

void mouse_build_packet(uint8_t *packet_bytes, struct packet *pp) {
    if (packet_bytes == NULL || pp == NULL) return;
    pp->bytes[0] = packet_bytes[0];
    pp->bytes[1] = packet_bytes[1];
    pp->bytes[2] = packet_bytes[2];
    pp->lb = (packet_bytes[0] & MOUSE_LB) != 0;
    pp->rb = (packet_bytes[0] & MOUSE_RB) != 0;
    pp->mb = (packet_bytes[0] & MOUSE_MB) != 0;
    pp->x_ov = (packet_bytes[0] & MOUSE_X_OVFL) != 0;
    pp->y_ov = (packet_bytes[0] & MOUSE_Y_OVFL) != 0;
    pp->delta_x = (packet_bytes[0] & MOUSE_X_SIGN) ?
        (int16_t)(0xFF00u | packet_bytes[1]) : (int16_t)packet_bytes[1];
    pp->delta_y = (packet_bytes[0] & MOUSE_Y_SIGN) ?
        (int16_t)(0xFF00u | packet_bytes[2]) : (int16_t)packet_bytes[2];
}
