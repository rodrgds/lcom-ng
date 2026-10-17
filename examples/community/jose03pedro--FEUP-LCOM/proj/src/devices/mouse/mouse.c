/**
 * @file mouse.c
 * @author
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mouse.h"

int hook_id_mouse = 2;
uint8_t byte_i = 0;
uint8_t mouse_bytes[3];
uint8_t current_byte;
MouseInfo mouse_info = {0, 0, 100, 100};
extern vbe_mode_info_t mode_info;

int(mouse_subscribe_interrupts)() {
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int(mouse_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id_mouse);
}

void(mouse_ih)() {
  read_KBC_output(KBC_WRITE_CMD, &current_byte, 1);
}

void mouse_sync_bytes() {
  if (byte_i == 0 && (current_byte & FIRST_BYTE)) {
    mouse_bytes[byte_i] = current_byte;
    byte_i++;
  }
  else if (byte_i > 0) {
    mouse_bytes[byte_i] = current_byte;
    byte_i++;
  }
}

void(mouse_sync_info)() {
  mouse_info.right_click = mouse_bytes[0] & MOUSE_RB;
  mouse_info.left_click = mouse_bytes[0] & MOUSE_LB;

  if (mouse_bytes[0] & MOUSE_X_OVERFLOW || mouse_bytes[0] & MOUSE_Y_OVERFLOW)
    return;

  int16_t delta_x = mouse_info.x + ((mouse_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1]);
  int16_t delta_y = mouse_info.y - ((mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2]);

  if (delta_x < 0 || delta_x > mode_info.XResolution || delta_y < 0 || delta_y > mode_info.YResolution)
    return;
  mouse_info.x = delta_x;
  mouse_info.y = delta_y;
}

int(mouse_write)(uint8_t cmd) {
  uint8_t attemps = MAX_ATTEMPS;
  uint8_t mouse_response;

  do {
    attemps--;
    if (write_KBC_command(KBC_IN_CMD, WRITE_BYTE_MOUSE))
      return 1;
    if (write_KBC_command(KBC_OUT_CMD, cmd))
      return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response))
      return 1;
  } while (mouse_response != ACK && attemps);

  return 0;
}
