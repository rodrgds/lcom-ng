#include "mouse.h"

uint8_t currentByte;
int mouse_hook_id = 2;
uint8_t currentIndex = 0;
uint8_t aux_packet[3];
MouseInfo mouse_info = {0, 0, 400, 300};
extern vbe_mode_info_t mode_info;

void (mouse_ih)() {
  if(read_KBC_output(0x60, &currentByte, 1)) printf("Error reading byte from mouse.");
}

int (write_to_mouse)(uint8_t command) {
  int attempts = 10;
  uint8_t ack_byte = 0;
  while ((attempts != 0) && (ack_byte != 0xFA)) {
    if (write_KBC_command(0x64, 0xD4)) return 1;
    if (write_KBC_command(0x60, command)) return 1;
    tickdelay(micros_to_ticks(20000));
    if (util_sys_inb(0x60, &ack_byte)) return 1;
    if (ack_byte == 0xFA) return 0;
    attempts--;
  }
  return 1;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)) return 1;
  return 0;
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_sync_bytes)() {
  if (currentIndex == 0 && (currentByte & BIT(3))) {
    aux_packet[0] = currentByte;
    currentIndex++;
  }
  else if (currentIndex > 0) {
    aux_packet[currentIndex++] = currentByte;
  }
}

void (mouse_move_to_info)() {
  mouse_info.l_click = aux_packet[0] & BIT(0);
  mouse_info.r_click = aux_packet[0] & BIT(1);
  if ((aux_packet[0] & BIT(6)) || (aux_packet[0] & BIT(7))) return;
  int16_t new_x = mouse_info.x + ((aux_packet[0] & BIT(4)) ? (0xFF00 | aux_packet[1]) : aux_packet[1]);
  int16_t new_y = mouse_info.y - ((aux_packet[0] & BIT(5)) ? (0xFF00 | aux_packet[2]) : aux_packet[2]);
  if (new_x < 0 || new_y < 0 || new_x > mode_info.XResolution || new_y > mode_info.YResolution) return;
  mouse_info.x = new_x;
  mouse_info.y = new_y;
}
