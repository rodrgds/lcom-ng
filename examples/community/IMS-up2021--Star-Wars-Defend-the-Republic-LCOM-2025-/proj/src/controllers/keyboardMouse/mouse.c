#include "mouse.h"
#include "entity.h"
#include <lcom/xpm.h>
#include "KBC.h"
#include "gameState.h"

struct packet mouse_packet;
int mouse_hook_id = 4;
uint8_t byte_index = 0;
uint8_t current_byte;
uint8_t mouse_bytes[3];

bool mouse_ready = false;



int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = (uint8_t)mouse_hook_id;

    if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
        printf("mouse_subscribe_int: sys_irqsetpolicy failed!\n");
        return 1;
    }

    return 0;
}


int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}


void (mouse_ih)() {
    uint8_t byte_just_read;
    read_kbc_out(KBC_OUT_CMD, &byte_just_read, 1);
    if (byte_index != 0 || byte_just_read & BIT(3)) {
        
            mouse_bytes[byte_index] = byte_just_read;
            byte_index++;
        
    } 
    if (byte_index == 3) {
        for (int i = 0; i < 3; i++) {
            mouse_packet.bytes[i] = mouse_bytes[i];
        }
        mouse_packet.lb = (mouse_bytes[0] & MOUSE_LB);
        mouse_packet.rb = (mouse_bytes[0] & MOUSE_RB);
        mouse_packet.mb = (mouse_bytes[0] & MOUSE_MB);
        mouse_packet.x_ov = (mouse_bytes[0] & MOUSE_X_OVERFLOW);
        mouse_packet.y_ov = (mouse_bytes[0] & MOUSE_Y_OVERFLOW);

        mouse_packet.delta_y = ((mouse_bytes[0] & MOUSE_Y_DELTA) << 3) | mouse_bytes[2];
        mouse_packet.delta_x = ((mouse_bytes[0] & MOUSE_X_DELTA) << 4) | mouse_bytes[1];
        if (mouse_packet.delta_x & BIT(8)) {
            mouse_packet.delta_x |= 0xFF00;
        }
        if (mouse_packet.delta_y & BIT(8)) {
            mouse_packet.delta_y |= 0xFF00;
        }
        byte_index = 0;
        mouse_ready = true;
    }
}


int (mouse_write)(uint8_t command_to_mouse) {
    uint8_t mouse_response;
    int attempts = MAX_ATTEMPS;

    do {
        attempts--;
        if (kbc_write_byte(KBC_IN_CMD, 0xD4)) {
            printf("mouse_write: Failed to send 0xD4 to KBC\n");
            return 1;
        }
        if (kbc_write_byte(KBC_WRITE_CMD, command_to_mouse)) {
            printf("mouse_write: Failed to send command (0x%02X) to KBC for mouse\n", command_to_mouse);
            return 1;
        }
        
            if (util_sys_inb(KBC_OUT_CMD, &mouse_response)) {
                printf("mouse_write: Failed to read mouse response\n");
                continue; // Try again
            }
            if (mouse_response == MOUSE_ACK) { // MOUSE_ACK is 0xFA
                return 0;
            } else {
                printf("Mouse response: 0x%02X to command 0x%02X (attempt %d)\n", mouse_response, command_to_mouse, MAX_ATTEMPS - attempts);
            }
    } while (attempts > 0);

    return 1;
}




