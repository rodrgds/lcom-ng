#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "mouse.h"
#include "utils.h"
#include "i8042.h"

static uint8_t mouse_byte;
static bool mouse_error;
static int hook_id_mouse = 2;

uint8_t get_mouse_byte() {
    return mouse_byte;
}

bool get_mouse_error() {
    return mouse_error;
}

int mouse_subscribe_int (uint8_t *bit_no) {
    *bit_no = hook_id_mouse;
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int mouse_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_ih) (){
    uint8_t status;
    mouse_error = false;
    // Read status register
    if (util_sys_inb(MOUSE_STAT_REG, &status) != 0) {
        mouse_error = true;
        return;
    }
    // Check if output buffer has data and if it's a mouse data byte
    if ((status & KBC_OBF) && (status & KBC_AUX)) {
        // Read mouse byte
        if (util_sys_inb(MOUSE_OUT_BUF, &mouse_byte) != 0) {
            mouse_error = true;
            return;
        }
        // Check for errors
        if ((status & KBC_TIMEOUT) != 0 || (status & KBC_PARITY) != 0) {
            mouse_error = true;
        }
    } else {
        mouse_error = true;
    }
}

// Tells the KBC that we want to talk to mouse and then sends the command to it
int mouse_write_command(uint8_t command) {
    uint8_t ack_byte;
    uint8_t status_check;
    int send_attempts = 3; // Retry sending the command up to 3 times (in case of NACK)
    while (send_attempts > 0) {
        // Wait for IBF to be clear before writing to CMD register
        do {
            if (util_sys_inb(MOUSE_STAT_REG, &status_check) != 0) return 1;
        } while (status_check & KBC_IBF);

        // Write 0xD4 to inform that we want to talk to mouse
        if (sys_outb(MOUSE_CMD_REG, WRITE_BYTE_MOUSE) != 0) return 1;

        // Wait for IBF to be clear before writing the actual command
        do {
            if (util_sys_inb(MOUSE_STAT_REG, &status_check) != 0) return 1;
        } while (status_check & KBC_IBF);

        // Write the command to the mouse
        if (sys_outb(MOUSE_IN_BUF, command) != 0) return 1;

        // Read response, discarding any leftover mouse stream data bytes
        int read_attempts = 10;
        while (read_attempts > 0) {
            tickdelay(micros_to_ticks(DELAY_US));

            if (util_sys_inb(MOUSE_STAT_REG, &status_check) != 0) return 1;
            if (status_check & KBC_OBF) {
                if (util_sys_inb(MOUSE_OUT_BUF, &ack_byte) != 0) return 1;
                if (ack_byte == ACK)   return 0; // Success
                if (ack_byte == ERROR) return 1; // Unrecoverable error
                if (ack_byte == NACK)  break;    // Resend command
                // Otherwise: leftover mouse data byte, discard and keep reading
            }
            read_attempts--;
        }
        send_attempts--;
    }
    return 1;
}

// Builds a struct packet from 3 raw bytes and prints it
void mouse_build_packet(uint8_t *packet_bytes, struct packet *pp) {
    for (int i = 0; i < 3; i++) {
        pp->bytes[i] = packet_bytes[i];
    }

    pp->lb = packet_bytes[0] & MOUSE_LB;
    pp->rb = packet_bytes[0] & MOUSE_RB;
    pp->mb = packet_bytes[0] & MOUSE_MB;
    pp->x_ov = packet_bytes[0] & MOUSE_X_OVFL;
    pp->y_ov = packet_bytes[0] & MOUSE_Y_OVFL;

    pp->delta_x = (packet_bytes[0] & MOUSE_X_SIGN) ? (packet_bytes[1] | 0xFF00) : packet_bytes[1];
    pp->delta_y = (packet_bytes[0] & MOUSE_Y_SIGN) ? (packet_bytes[2] | 0xFF00) : packet_bytes[2];

    mouse_print_packet(pp);
}
