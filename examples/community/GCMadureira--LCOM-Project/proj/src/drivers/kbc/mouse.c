/** 
 * @file mouse.c
 * @brief Source code file for all the logic to interact with the kbc's mouse
 */


#include "mouse.h"

static int mouse_hook_id; ///< The hook id to unsubscribe the mouse interruptions
static uint8_t mouse_packet_byte; ///< The lastest packet byte read

uint8_t (mouse_get_packet_byte)(){
  return mouse_packet_byte;
}

int (mouse_subscribe_int_exclusive)(uint8_t *bit_no){
  mouse_hook_id = *bit_no;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(){
  uint8_t status;
  util_sys_inb(KBC_OUT_REG, &status);
  mouse_packet_byte = status;
}

int (mouse_stream_enable_data_reporting)(){
  uint8_t status;
  for(int i = 0; i < 5; ++i){ // wait for 100ms max
    if(kbc_write_command(WR_MOUSE_BYTE)) return 1; //try to enable data reporting
    if(kbc_write_command_arg(ENABLE_DATA_REPORTING)) return 1;

    tickdelay(micros_to_ticks(20000));

    if(util_sys_inb(KBC_OUT_REG, &status)) continue;
    if(status == MOUSE_BYTE_ACK) return 0;
  }
  return 1;
}

int (mouse_stream_disable_data_reporting)(){
  uint8_t status;
  for(int i = 0; i < 5; ++i){ // wait for 100ms max
    if(kbc_write_command(WR_MOUSE_BYTE)) return 1; // try to disable data reporting
    if(kbc_write_command_arg(DISABLE_DATA_REPORTING)) return 1;

    tickdelay(micros_to_ticks(20000));

    if(util_sys_inb(KBC_OUT_REG, &status)) continue;
    if(status == MOUSE_BYTE_ACK) return 0; // command was acknowledged
  }
  return 1;
}

struct packet (mouse_assemble_packet)(uint8_t packet_bytes[3]){
  struct packet packet;
  memcpy(packet.bytes, packet_bytes, 3);

  packet.rb = packet_bytes[0] & RIGHT_BUTTON_PRESSED;
  packet.mb = packet_bytes[0] & MIDDLE_BUTTON_PRESSD;
  packet.lb = packet_bytes[0] & LEFT_BUTTON_PRESSED;

  // form a 2s complement number with the delta byte and the MSB bit
  packet.delta_x = (int16_t)packet_bytes[1] | (packet_bytes[0] & MSB_X_DELTA ? 0xFF00 : 0);
  packet.delta_y = (int16_t)packet_bytes[2] | (packet_bytes[0] & MSB_Y_DELTA ? 0xFF00 : 0);

  packet.x_ov = packet_bytes[0] & X_OVERFLOW;
  packet.y_ov = packet_bytes[0] & Y_OVERFLOW;

  return packet;
}
