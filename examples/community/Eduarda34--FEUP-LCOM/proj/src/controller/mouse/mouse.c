#include <lcom/lcf.h>
#include "mouse.h"
#include <stdint.h>
#include <stdio.h>

struct packet mouse_packet;
uint8_t packet_bytes[3];
uint8_t byte_index = 0;
static uint8_t current_byte;

int mouse_hook_id = 2;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);

    if (sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) {
        return 1;
    }
    return 0;
}

int (mouse_disable)() {
    if (sys_irqdisable(&mouse_hook_id) != 0) {
        return 1;
    }
    return 0;
}

int (mouse_enable)() {
    if (sys_irqenable(&mouse_hook_id) != 0) {
        return 1;
    }
    return 0;
}

void (mouse_ih)() {
    if (read_kbc_output_buffer(0x60, &current_byte, 1)) 
        printf("Error: Could not read mouse data!\n");
    
}

void (mouse_sync_packet)() {
    if (byte_index == 0 && (current_byte & BIT(3))) { // é o byte CONTROL, o bit 3 está ativo
        packet_bytes[byte_index++]= current_byte;
      }
      else if (byte_index > 0) {                            // recebe os deslocamentos em X e Y
        packet_bytes[byte_index++] = current_byte;
    }
}

void (mouse_packet_handler)() {
        // We have a complete packet! Let's parse it.
    for (int i = 0 ; i < 3 ; i++) {
        mouse_packet.bytes[i] = packet_bytes[i];
    }
    mouse_packet.lb = packet_bytes[0] & BIT(0);
    mouse_packet.mb = packet_bytes[0] & BIT(2);
    mouse_packet.rb = packet_bytes[0] & BIT(1);
    mouse_packet.x_ov = packet_bytes[0] & BIT(6);
    mouse_packet.y_ov = packet_bytes[0] & BIT(7);
    mouse_packet.delta_x = (packet_bytes[0] & BIT(4)) ? (0xFF00 | packet_bytes[1]) : packet_bytes[1];
    mouse_packet.delta_y = (packet_bytes[0] & BIT(5)) ? (0xFF00 | packet_bytes[2]) : packet_bytes[2];

    byte_index = 0; // Reset for the next packet
        // We'll signal to mouse_test_packet that a packet is ready
   
}

int (mouse_write)(uint8_t cmd) {
    uint8_t ack;
    int retries = 5;

    while (retries > 0) {
        if (write_kbc_command(0x64, 0xD4) != 0) { 
            // Disable data reporting
            printf("Error: Could not write to KBC command port!\n");
            return 1;
        }

        if (write_kbc_command(0x60, cmd) != 0) {
            // Send command to disable data reporting
            printf("Error: Could not send command to disable data reporting!\n");
            return 1;
        }

        tickdelay(micros_to_ticks(20000)); 

        if (util_sys_inb(0x60, &ack) == 0) {
            if (ack == 0xFA) { // Hardcoding ACK
                return 0; // Success!
            } else if (ack == 0xFE) { // Hardcoding NACK
                printf("Warning: Mouse responded with NACK, retrying...\n");
            } else {
                printf("Warning: Received unexpected response from mouse: 0x%X, retrying...\n", ack);
            }
        } else {
            printf("Error: Failed to read acknowledgement from mouse\n");
        }
        retries--;
    }  

    return 1; // Failed after retries
}
