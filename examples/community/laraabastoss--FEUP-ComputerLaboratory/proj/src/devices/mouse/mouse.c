#include "mouse.h"

int hook_id_mouse = 2;
uint8_t mouse_packet_bytes[3];       
uint8_t current_byte;             
uint8_t mouse_index = 0; 
MouseInfo mouse_packet = {0, 400, 300, 0, 0}; //initial position of mouse
extern vbe_mode_info_t vbe_info;


int (mouse_subscribe_interrupts)(){
  return sys_irqsetpolicy(MOUSEIRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}


int (mouse_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_interrupt_handler)(){
  get_output_KBC(0x60, &current_byte, 1);
  if (mouse_index > 0) {                           
    mouse_packet_bytes[mouse_index] = current_byte;
    mouse_index++;
  }
  else if ((current_byte & BIT(3)) && (mouse_index == 0)) { 
    mouse_packet_bytes[0]= current_byte;
    mouse_index++;
  }
}


void (mouse_fill_packet)(){
    mouse_packet.left_click = mouse_packet_bytes[0] & BIT(0);

    if (mouse_packet_bytes[0] & BIT(6) || mouse_packet_bytes[0] & BIT(7)) return;

    if (mouse_packet_bytes[0] & BIT(4)){
      mouse_packet.delta_x=(0xFF00 | mouse_packet_bytes[1]);
    }
    else{
      mouse_packet.delta_x=mouse_packet_bytes[1];
    }

    mouse_packet.positive_x=mouse_packet_bytes[0] & BIT(4);

    int16_t delta_x, delta_y;
    if ((mouse_packet_bytes[0] & BIT(4))){
      delta_x = mouse_packet.x + (0xFF00 | mouse_packet_bytes[1]);
    }
    else {
      delta_x = mouse_packet.x + mouse_packet_bytes[1];
    }

    if ((mouse_packet_bytes[0] & BIT(5))){
      delta_y = mouse_packet.y - (0xFF00 | mouse_packet_bytes[2]);
    }
    else {
      delta_y = mouse_packet.y - mouse_packet_bytes[2];
    }

    if (delta_x < 0 || delta_x > vbe_info.XResolution || delta_y < 0 || delta_y > vbe_info.YResolution) return; // check if mouse is outside of screen
    mouse_packet.x = delta_x;
    mouse_packet.y = delta_y;


}

int (activate_mouse_stream)(){
  uint8_t tries = 10; uint8_t answer = 0;
  while (tries && answer != 0xFA) {
      tries--;
      if (write_command_KBC(0x64, 0xD4))
          return 1;
      if (write_command_KBC(0x60, 0xEA))
          return 1;
      tickdelay(micros_to_ticks(20000));
      if (util_sys_inb(0x60, &answer))
          return 1;
      if (answer == 0xFA)
          return 0;
  }
    return 0;
}

int (activate_mouse_datareport)(){
  uint8_t tries = 10; uint8_t answer = 0;
  while (tries && answer != 0xFA) {
      tries--;
      if (write_command_KBC(0x64, 0xD4))
          return 1;
      if (write_command_KBC(0x60, 0xF4))
          return 1;
      tickdelay(micros_to_ticks(20000));
      if (util_sys_inb(0x60, &answer))
          return 1;
      if (answer == 0xFA)
          return 0;
  }
    return 0;
}

int (deactivate_mouse)() {
  uint8_t tries = 10; uint8_t answer = 0;
  while (tries && answer != 0xFA) {
      tries--;
      if (write_command_KBC(0x64, 0xD4))
          return 1;
      if (write_command_KBC(0x60, 0xF5))
          return 1;
      tickdelay(micros_to_ticks(20000));
      if (util_sys_inb(0x60, &answer))
          return 1;
      if (answer == 0xFA)
          return 0;
  }
  return 0;
}
