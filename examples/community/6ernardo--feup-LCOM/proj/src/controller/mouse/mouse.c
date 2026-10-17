#include "controller/mouse/mouse.h"
#include "controller/video/graphics.h"
int hook_id_mouse = 3;
MouseInfo mouse_info = {0, 0, 0, 100, 100};
uint8_t byte_index = 0;
uint8_t mouse_bytes[3];
uint8_t current_byte;
extern vbe_mode_info_t mode_info;


int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }
  *bit_no = BIT(hook_id_mouse);
  return (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse));
}
int(mouse_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id_mouse));
}
void(mouse_ih)() {
  if (read_KBC_output(KBC_OUT_CMD, &current_byte, 1)) {
    printf("Error in reading byte from mouse\n");
  }
}
void mouse_sync_bytes() {
  if (byte_index == 0 && (current_byte & FIRST_BYTE)) { // é o byte CONTROL, o bit 3 está ativo
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
  else if (byte_index > 0) { // recebe os deslocamentos em X e Y
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}
void(mouse_bytes_to_packet)() {
  int16_t delta_x;
  int16_t delta_y;
  mouse_info.left_click = mouse_bytes[0] & MOUSE_LB;
  mouse_info.right_click = mouse_bytes[0] & MOUSE_RB;
  mouse_info.middle_click = mouse_bytes[0] & MOUSE_MB;
  if (mouse_bytes[0] & MOUSE_X_OVERFLOW || mouse_bytes[0] & MOUSE_Y_OVERFLOW)
    return;
  if (mouse_bytes[0] & MOUSE_X_DELTA) {
    delta_x = (mouse_bytes[1] | 0xFF00);
  }
  else {
    delta_x = (mouse_bytes[1]);
  }
  if (mouse_bytes[0] & MOUSE_Y_DELTA) {
    delta_y = (mouse_bytes[2] | 0xFF00);
  }
  else {
    delta_y = (mouse_bytes[2]);
  }
  int16_t new_x = mouse_info.x + delta_x;
  int16_t new_y = mouse_info.y - delta_y;
  if (new_x < 0 || new_x > mode_info.XResolution || new_y < 0 || new_y > mode_info.YResolution)
    return;
  mouse_info.x = new_x;
  mouse_info.y = new_y;
}
int(mouse_write)(uint8_t command) {

  uint8_t attemps = MAX_ATTEMPS;
  uint8_t mouse_response;

  do {
    attemps--;
    if (write_KBC_command(KBC_IN_CMD, WRITE_BYTE_TO_MOUSE))
      return 1;
    if (write_KBC_command(KBC_OUT_CMD, command))
      return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response))
      return 1;
    if (mouse_response == ACK)
      return 0;
  } while (mouse_response != ACK && attemps);

  return 1;
}
int(mouse_read_data)() {

  if (sys_outb(0x64, WRITE_BYTE_TO_MOUSE) != 0) {
    return 1;
  }

  if (mouse_write(MOUSE_READ_DATA) != 0) {
    return 1;
  }

  return 0;
}

int getX() {
  return mouse_info.x;
}
int getY() {
  return mouse_info.y;
}


