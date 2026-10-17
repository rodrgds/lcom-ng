#include "mouse.h"

int mouse_hook_id = 2;
int mouse_ih_counter = 0;
uint8_t mouse_data;
uint8_t mouse_bytes[3];
extern vbe_mode_info_t m_info;
MouseHelper mouseHelper = {576, 432, 0, 0, 0};

int (mouse_subscribe_interrupts)() {
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_write_command)(uint8_t command) {
  uint8_t status;
  int repeat = MAX_ATTEMPS;

  do {
    repeat--;
    if(keyboard_mouse_write_command(KBC_IN_CMD, WRITE_BYTE_CMD_M) != 0){
      return 1;
    }

    if(keyboard_mouse_write_command(KBC_OUT_CMD, command) != 0){
      return 1;
    }

    if(util_sys_inb(KBC_OUT_CMD, &status) != 0){
      return 1;
    }

    if(status == MOUSE_ACK){
      return 0;
    }
  } while((status != MOUSE_ACK) && repeat);

  return 1;
}

void (assemble_packet)() {
  if(mouse_ih_counter == 0){
    if(mouse_data & BIT(3)){
      mouse_bytes[mouse_ih_counter] = mouse_data;
      mouse_ih_counter++;
    }
  } 
  
  else {
    mouse_bytes[mouse_ih_counter] = mouse_data;
    mouse_ih_counter++;
  }
}

void (parse_packet)() {
  if(mouse_bytes[0] & BIT(6) || mouse_bytes[0] & BIT(7)) return;

  mouseHelper.click_left_button = mouse_bytes[0] & BIT(0);
  mouseHelper.click_right_button = mouse_bytes[0] & BIT(1);
  mouseHelper.click_middle_button = mouse_bytes[0] & BIT(2);
  
  uint16_t x_value;
  uint16_t y_value;

  if (mouse_bytes[0] & BIT(4)) {
      x_value = mouseHelper.x_pos + (0xFF00 | mouse_bytes[1]);
  } else {
      x_value = mouseHelper.x_pos + mouse_bytes[1];
  }

  if (mouse_bytes[0] & BIT(5)) {
      y_value = mouseHelper.y_pos - (0xFF00 | mouse_bytes[2]);
  } else {
      y_value = mouseHelper.y_pos - mouse_bytes[2];
  }

  if(x_value < 0 || x_value > m_info.XResolution - 5 || y_value < 0 || y_value > m_info.YResolution - 5) return;
  else {
    mouseHelper.x_pos = x_value;
    mouseHelper.y_pos = y_value;
  } 
}
