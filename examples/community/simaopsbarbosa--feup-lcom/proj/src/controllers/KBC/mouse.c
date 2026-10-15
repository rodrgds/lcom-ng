#include "mouse.h"
#include "../../view/view.h"
#include <stdio.h> 

int mouse_hook_id = 2;
struct packet pp;
uint8_t current_mouse_byte;
int current_mouse_byte_index = 0;
uint8_t read_bytes[3];
extern vbe_mode_info_t mode_info;

bool new_packet = false;

void(mouse_ih)() {
  if (KBC_read_output(KBC_OUT_CMD, &current_mouse_byte, 1)) {
    printf("[ERROR] could not read byte from mouse\n");
  }
}

int mouse_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int mouse_unsubscribe_int() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void mouse_sync() {
  if ((current_mouse_byte_index == 0) && (current_mouse_byte & IS_FIRST_BYTE)) {
    read_bytes[current_mouse_byte_index] = current_mouse_byte;
    current_mouse_byte_index++;
  } else if (current_mouse_byte_index > 0) {
    read_bytes[current_mouse_byte_index] = current_mouse_byte;
    current_mouse_byte_index++;
  }
  if(current_mouse_byte_index == 3) {
    generate_packet();
    new_packet = true;
    current_mouse_byte_index = 0;
  } else { new_packet = false; }
}

// parses 3 bytes into a packet
void generate_packet() {
  pp.bytes[0] = read_bytes[0];
  pp.bytes[1] = read_bytes[1];
  pp.bytes[2] = read_bytes[2];
  pp.lb = read_bytes[0] & MOUSE_LB;
  pp.mb = read_bytes[0] & MOUSE_MB;
  pp.rb = read_bytes[0] & MOUSE_RB;
  pp.delta_x = read_bytes[1];
  if (read_bytes[0] & MSB_X_DELTA) {
    pp.delta_x |= 0xFF00;
  }
  pp.delta_y = read_bytes[2];
  if (read_bytes[0] & MSB_Y_DELTA) {
    pp.delta_y |= 0xFF00;
  }
  pp.x_ov = read_bytes[0] & MOUSE_X_OVERFLOW;
  pp.y_ov = read_bytes[0] & MOUSE_Y_OVERFLOW;
}

int mouse_write_command(uint8_t command) {
  uint8_t attempts = ATTEMPTS;
  uint8_t response;

  while (attempts > 0) {
    if (KBC_write_command(KBC_IN_CMD, MOUSE_WRITE_BYTE)) {
      return 1;
    }
    if (KBC_write_command(KBC_OUT_CMD, command)) {
      return 1;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_CMD, &response)) {
      return 1;
    }
    if (response == ACKNOWLEDGMENT) {
      return 0;
    }
    attempts--;
  }

  return 1;
}

bool isLeftButtonPressed() {
  return (pp.lb != 0);
}

bool isRightButtonPressed() {
  return (pp.rb != 0);
}
