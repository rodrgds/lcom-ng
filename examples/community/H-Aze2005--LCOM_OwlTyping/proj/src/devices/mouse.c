#include "mouse.h"

int mouse_hook_id = 2;
int current_mouse_byte = 0;
struct packet mouse_packet;
int cursor_x = 500;
int cursor_y = 350;

int subscribe_mouse(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(mouse_hook_id);

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;

    return 0;
}

int unsubscribe_mouse() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;  
}

int (mouse_disable_data_reporting)() {
    uint32_t response;

    if (sys_outb(KBC_STATUS, MOUSE_WRITE_BYTE) != 0) return 1;
    
    // Send the command to disable data reporting
    if (sys_outb(KBC_OUT_BUFFER, MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

    // Acknowledgment byte
    if (sys_inb(KBC_OUT_BUFFER, &response) != 0) return 1;
    if (response != MOUSE_ACK) return 1; // Everything is OK

    return 0;
}

void mouse_int_h() {
    uint32_t status;
    uint32_t data;

    sys_inb(KBC_STATUS, &status);

    if (status & KBC_OBF) {
        sys_inb(KBC_OUT_BUFFER, &data);

        if (current_mouse_byte == 0) {
            if (data & BIT(3)) {
                mouse_packet.bytes[0] = (uint8_t) data;
                current_mouse_byte = 1;
            }
        } else if (current_mouse_byte == 1) {
            mouse_packet.bytes[1] = (uint8_t) data;
            current_mouse_byte = 2;
        } else if (current_mouse_byte == 2) {
            mouse_packet.bytes[2] = (uint8_t) data;
            current_mouse_byte = 0; // Reset for next packet

            // Buttons
            mouse_packet.lb = mouse_packet.bytes[0] & BIT(0);
            mouse_packet.rb = mouse_packet.bytes[0] & BIT(1);
            mouse_packet.mb = mouse_packet.bytes[0] & BIT(2);

            // Overflows
            mouse_packet.x_ov = mouse_packet.bytes[0] & BIT(6);
            mouse_packet.y_ov = mouse_packet.bytes[0] & BIT(7);

            // X movement
            mouse_packet.delta_x = (int16_t) mouse_packet.bytes[1];
            if (mouse_packet.bytes[0] & BIT(4)) // Sign extend
                mouse_packet.delta_x |= 0xFF00;

            // Y movement
            mouse_packet.delta_y = (int16_t) mouse_packet.bytes[2];
            if (mouse_packet.bytes[0] & BIT(5)) // Sign extend
                mouse_packet.delta_y |= 0xFF00;
            
           
        }
    }
}
