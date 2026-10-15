#include "mouse.h"
#include "../i8042.h"
#include "../KBC.h"

#define SCREEN_WIDTH 1130
#define SCREEN_HEIGHT 840

int mouse_hook_id = 3;
struct packet mouse_packet;
uint8_t byte_index = 0;      
uint8_t mpacket[3];            
uint8_t current_byte;  
int mouse_x = 576;
int mouse_y = 432;

int mouse_subscribe_int(uint8_t *irq_set) {
    if (irq_set == NULL) return 1; 
    *irq_set = BIT(mouse_hook_id);  
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int mouse_unsubscribe_int() {
    return sys_irqrmpolicy(&mouse_hook_id); 
}

void (mouse_ih)() {
  if (KBC_get_byte(&current_byte) != 0) {
    printf("ERROR: couldn't read byte from mouse\n");
    return;
  }
}

void mouse_sync_bytes() {
    if (byte_index == 0 && (current_byte & BIT(3))) {
        mpacket[byte_index] = current_byte;
        byte_index++;
    }
    else if (byte_index > 0){
        mpacket[byte_index] = current_byte;
        byte_index++;
    }
}

void (mouse_bytes_to_packet)() {
    for (int i = 0; i < 3; i++) {
        mouse_packet.bytes[i] = mpacket[i];
    }
    mouse_packet.rb = mpacket[0] & MOUSE_RB;
    mouse_packet.lb = mpacket[0] & MOUSE_LB;
    mouse_packet.mb = mpacket[0] & MOUSE_MB;
    mouse_packet.x_ov = mpacket[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = mpacket[0] & MOUSE_Y_OVERFLOW;
    mouse_packet.delta_x = mpacket[0] & MOUSE_X_SIGN ? (0xFF00 | mpacket[1]) : mpacket[1];
    mouse_packet.delta_y = mpacket[0] & MOUSE_Y_SIGN ? (0xFF00 | mpacket[2]) : mpacket[2];

}

int (mouse_write)(uint8_t command) {
  uint8_t tries = MAX_ATTEMPTS;
  uint8_t mouse_response = 0;
  uint8_t status;

  while(tries && mouse_response != ACK){
    tries--;
    if (KBC_write_command(KBC_IN_CMD, WRITE_BYTE_MOUSE) != 0) {
      printf("ERROR: couldn't write to mouse\n");
      return 1;
    }
    while (1) {
      KBC_read_status(&status);
      if ((status & FULL_IN_BUFFER) == 0) break;
      tickdelay(micros_to_ticks(WAIT_KBC));
    }
    if (KBC_write_command(KBC_OUT_CMD, command) != 0) {
      printf("ERROR: couldn't write to mouse\n");
      return 1;
    }
    if (KBC_read_output(&mouse_response) != 0) {
      printf("ERROR: couldn't read mouse response\n");
      return 1;
    }
    if (mouse_response == ACK) {
      return 0;
    }
  }
  return 1;
  
}

void update_mouse_position() {
    mouse_x += mouse_packet.delta_x;
    mouse_y -= mouse_packet.delta_y;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT - 1;
}
