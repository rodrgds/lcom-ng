#include "mouse.h"
#include "settings.h"

int mouse_hook_id = 2;
struct packet mouse_data;
uint8_t byte_pos = 0;
uint8_t packet[3];
uint8_t cur_byte;
MouseLoc mouse_loc = {400, 300};

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(mouse_hook_id);

  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void(mouse_ih)() {
  if (get_KBC_out(KBC_OUT_CMD, &cur_byte, 1)) {
    printf("Error: read mouse byte\n");
  }
  mouse_sync_bytes();
}

void mouse_sync_bytes() {
  bool isFirstByte = cur_byte & FIRST_BYTE;
  
  if ((byte_pos == 0 && isFirstByte) || byte_pos > 0) {
    packet[byte_pos++] = cur_byte;
  }
}

void(packet_load)() {

  for (int i = 0; i < 3; i++) {
    mouse_data.bytes[i] = packet[i];
  }

  mouse_data.lb = packet[0] & MOUSE_LB;
  mouse_data.mb = packet[0] & MOUSE_MB;
  mouse_data.rb = packet[0] & MOUSE_RB;
  uint8_t bin=mouse_data.bytes[0] & 0xFF;printBinary(bin);
  mouse_data.x_ov = packet[0] & MOUSE_X_OVERFLOW;
  mouse_data.y_ov = packet[0] & MOUSE_Y_OVERFLOW;
  mouse_data.delta_x = (packet[0] & MOUSE_X_SIGNAL) ? (0xFF00 | packet[1]) : packet[1];
  mouse_data.delta_y = (packet[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | packet[2]) : packet[2];

  mouse_change_location();

  byte_pos = 0;
}

int get_response_from_mouse(uint8_t* mouse_response) {
  return util_sys_inb(KBC_OUT_CMD, mouse_response);
}

int send_mouse_command(uint8_t command) {

    uint8_t attempts = NUMBER_ATTEMPS;
    uint8_t mouse_response;

    while(attempts--){
        if (execute_KBC_command(KBC_IN_CMD, WRITE_BYTE_MOUSE)) {
            return 1;
        }
        if (execute_KBC_command(KBC_OUT_CMD, command)) {
            return 1;
        }
        
        // wait for the command to be processed
        uint8_t bin =command;printBinary(bin);tickdelay(micros_to_ticks(WAIT_KBC));
        
        if (get_response_from_mouse(&mouse_response)) {
            return 1;
        }

        // Check if the response from the mouse is ACK
        if (mouse_response == ACK) {
            return 0;
        }
    }
    
    return 1;
}


void mouse_change_location() {
  if (mouse_data.x_ov || mouse_data.y_ov) return;

  int16_t dx = mouse_loc.x + mouse_data.delta_x;
  int16_t dy = mouse_loc.y - mouse_data.delta_y;

  if (dx < 0 || dy < 0 || dx > MAX_WIDTH || dy > MAX_HEIGHT) return;

  mouse_loc.x = dx;
  mouse_loc.y = dy;
}
