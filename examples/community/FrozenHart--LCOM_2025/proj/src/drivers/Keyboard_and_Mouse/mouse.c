#include "mouse.h"
#include <stdint.h>


static int mouse_irq_id = 3;
static uint8_t packet_data[3];
//static uint8_t byte_counter = 0;
//static uint8_t lst_byte;
uint32_t mouse_packet_count;
struct packet mouse_packet;
mouse_packet_state_t current_state = BYTE_1_WAIT;

int mouse_x = 5;
int mouse_y = 5;

bool click_started = false;
bool last_mouse_lb = false;


int (mouse_subscribe_int)(uint8_t *bit) {
    *bit = BIT(mouse_irq_id);                 // Store the bit number for the interrupt handler
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_irq_id) != 0) {
        printf("Failed to set policy\n");
        return 1; // Error in setting policy
    }
    return 0;

}

int (mouse_unsubscribe_int)() {
    if(sys_irqrmpolicy(&mouse_irq_id)!=0) {
        printf("Failed to remove policy\n");
        return 1;
    }
    return 0;

}



void (mouse_ih)() {
    switch (current_state) {
        case BYTE_1_WAIT:

            // Read the first byte of the mouse packet
            if (read_mouse_data(&mouse_packet.bytes[0]) != 0) {
                printf("Failed to read first byte\n");
                return;
            }

            current_state = BYTE_2_WAIT;
            break;
        case BYTE_2_WAIT:

            // Read the second byte of the mouse packet
            if (read_mouse_data(&mouse_packet.bytes[1]) != 0) {
                printf("Failed to read second byte\n");
                return;
            }

            current_state = BYTE_3_WAIT;
            break;

        case BYTE_3_WAIT:

            // Read the third byte of the mouse packet
            if (read_mouse_data(&mouse_packet.bytes[2]) != 0) {
                printf("Failed to read third byte\n");
                return;
            }

            // Process the mouse packet
            if (process_mouse_packet(&mouse_packet) != 0) {
                printf("Failed to process mouse packet\n");
                return;
            }
            
            click_started = (mouse_packet.lb && !last_mouse_lb);
            last_mouse_lb = mouse_packet.lb;


            mouse_packet_count++;             // Increment the packet count
            current_state = BYTE_1_WAIT; // Reset state for next packet
            break;
    }
}
/*
void mouse_sync_bytes() {
    if (byte_counter == 0) {
        if (lst_byte & BIT(3)) {  // Bit 3 deve estar ativo no primeiro byte
            packet_buffer[0] = lst_byte;
            byte_counter = 1;
        }
    } else {
        packet_buffer[byte_counter] = lst_byte;
        byte_counter++;

        if (byte_counter > 3) {
            byte_counter = 0; // segurança contra overflow
        }
    }
}*/

void (mouse_bytes_to_packet)() {
    for (int i = 0; i < 3; ++i) {
        mouse_packet.bytes[i] = packet_data[i];
    }

    uint8_t first = packet_data[0];
    uint8_t x = packet_data[1];
    uint8_t y = packet_data[2];

    mouse_packet.lb = first & MOUSE_LB;
    mouse_packet.mb = first & MOUSE_MB;
    mouse_packet.rb = first & MOUSE_RB;

    mouse_packet.x_ov = first & MOUSE_X_OVF;
    mouse_packet.y_ov = first & MOUSE_Y_OVF;

    mouse_packet.delta_x = (first & MOUSE_MSB_X) 
        ? (int16_t)(NEGATIVE | x)
        : (int16_t) x;
    mouse_packet.delta_y = (first & MOUSE_MSB_Y) 
        ? (int16_t)(NEGATIVE | y)
        : (int16_t) y;
}

int (mouse_write_command)(mouse_commands_t cmd, uint8_t *command_byte) {

    uint8_t tries = 0;
    uint8_t stat;

    while (tries<MAX_ATTEMPTS) {

        if (kbc_check_config(&stat) != 0) {
            printf("Failed to check configuration\n");
            return 1;
        }

        if (stat & KBC_PARITY) {
            printf("Parity error - invalid data\n");
            return 1;
        }

        if (stat & KBC_TIMEOUT) {
            printf("Timeout error - invalid data\n");
            return 1;
        }

        if ((stat & KBC_IBF)==0) {
            if (cmd != KBC_CUSTOM_CMD) { // If the command is not custom, write the command to the KBC
                if (sys_outb(KBC_CMD_IO, (uint8_t) cmd) != 0) {
                    printf("Failed to write command to KBC\n");
                    return 1;
                }
            }
            else { // If the command is custom, write the command byte to the KBC
                if (sys_outb(KBC_CMD_IO, *command_byte) != 0) {
                    printf("Failed to write command byte to KBC\n");
                    return 1;
                }
            }
            return 0; 
        }

        tickdelay(micros_to_ticks(10000)); // 10 ms
        tries++;
    }

    return 1;
}

int(mouse_disable_data_reporting)() {
  int attempts = 0;
  while (attempts < MAX_ATTEMPTS) {
    if (kbc_write_command(KBC_ENABLE_MOUSE_COMANDS,0) != 0) {
      printf("Failed to write command to KBC\n");
      return 1;
    }

    if (mouse_write_command(MOUSE_DISABLE_DATA_REPORTING,0) != 0) {
      printf("Failed to write command to Mouse\n");
      return 1;
    }

    tickdelay(micros_to_ticks(10000)); // 10 ms

    uint8_t mouse_acknowledgment;
    if (util_sys_inb(KBC_CMD_IO, &mouse_acknowledgment) != 0) {
      printf("Failed to read Mouse Acknowledgment\n");
      return 1;
    }

    switch (mouse_acknowledgment) {
      case ACK:
        return 0;
      case NACK:
        attempts++;
        break;
      case ERROR:
        printf("ERROR Second consecutive invalid byte\n");
        return 1;
      default:
        break;
    }
  }
  return 1;
}

int(read_mouse_data)(uint8_t *data) {
  uint8_t status = 0;
  int attempts = 0;

  while (MAX_ATTEMPTS > attempts) {
    // Check if there was some communications error and read the status register
    if (kbc_check_config(&status) != 0) {
      printf("Failed to check configuration\n");
      return 1;
    }

    // OBF has data to read
    if ((status & KBC_OBF) != 0) {
      // Read data from the output buffer
      if (util_sys_inb(KBC_CMD_IO, data) != 0) {
        printf("Failed to read status register\n");
        return 1;
      }

      if (status & KBC_PARITY) {
        printf("Parity error - invalid data\n");
        return 1;
      }

      if (status & KBC_TIMEOUT) {
        printf("Timeout error - invalid data\n");
        return 1;
      }

      return 0;
    }

    attempts++;
  }

  printf("Timeout waiting for input buffer to be empty\n");
  return 1;
}

int(process_mouse_packet)(struct packet *packet) {
  // Extract button states from the first byte
  packet->lb = packet->bytes[0] & MOUSE_LB; // Left button
  packet->rb = packet->bytes[0] & MOUSE_RB; // Right button
  packet->mb = packet->bytes[0] & MOUSE_MB; // Middle button

  // Extract overflow flags from the first byte
  packet->x_ov = packet->bytes[0] & MOUSE_X_OVF; // X overflow
  packet->y_ov = packet->bytes[0] & MOUSE_Y_OVF; // Y overflow

  // Calculate X displacement (9-bit signed value)
  packet->delta_x = (packet->bytes[0] & MOUSE_MSB_X) ? (NEGATIVE | packet->bytes[1]) : packet->bytes[1];
  packet->delta_y = (packet->bytes[0] & MOUSE_MSB_Y) ? (NEGATIVE | packet->bytes[2]) : packet->bytes[2];

  // Update the cursor sprite position
  mouse_x += packet->delta_x;
  mouse_y -= packet->delta_y; // Invert Y direction for screen coordinates
  
  return 0;
}



int(mouse_enable_data_reporting_costume)() {
  int attempts = 0;
  while (attempts < MAX_ATTEMPTS) {
    if (kbc_write_command(KBC_ENABLE_MOUSE_COMANDS, 0) != 0) {
      printf("Failed to write command to KBC\n");
      return 1;
    }

    if (mouse_write_command(MOUSE_ENABLE_DATA_REPORTING,0) != 0) {
      printf("Failed to write command to Mouse\n");
      return 1;
    }

    tickdelay(micros_to_ticks(10000)); // 10 ms

    uint8_t ack;

    if (util_sys_inb(KBC_CMD_IO, &ack) != 0) {
      printf("Failed to read Mouse Acknowledgment\n");
      return 1;
    }

    switch (ack) {
      case ACK:
        return 0;
      case NACK:
        attempts++;
        break;
      case ERROR:
        printf("ERROR Second consecutive invalid byte\n");
        return 1;
      default:
        break;
    }
  }
  return 1;
}

