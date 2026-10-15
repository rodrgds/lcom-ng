#include "mouse.h"
#include "../utils.h"

static int hook_id = 2;

struct packet packet_pp;
int pp_index = 0;
bool packet_read = false;

int (mouse_subscribe_int)(uint8_t* bit_no){

  hook_id = 2;

  *bit_no = hook_id;

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &hook_id);
}

int (mouse_unsubscribe_int)(){

  return sys_irqrmpolicy(&hook_id);
}

void (mouse_ih)(){

  if (packet_read){
    packet_read = false;
    pp_index = 0;
  }

  uint8_t status;

  if (util_sys_inb(STATUS_PORT, &status))
    return;

  if (status & (TIME_OUT_ERROR | PARITY_ERROR))
    return;

  if (!(status & OUT_BUF_READY))
    return;

  if (!(status & DATA_FROM_MOUSE))
    return;

  uint8_t packet_byte;

  if (util_sys_inb(KBD_OUT_BUF, &packet_byte))
    return;

  if (pp_index == 0 && !(packet_byte & MOUSE_SYNC_BYTE)){
    packet_read = false;
    return;
  }

  packet_pp.bytes[pp_index] = packet_byte;

  switch (pp_index)
  {
  case 0: {
    packet_pp.lb = packet_byte & MOUSE_LEFT_BTN;
    packet_pp.rb = packet_byte & MOUSE_RIGHT_BTN;
    packet_pp.mb = packet_byte & MOUSE_MIDDLE_BTN;
    if (packet_byte &  MOUSE_DX_BIT9)
      packet_pp.delta_x = 0xFF00;
    else {
      packet_pp.delta_x = 0;
    }
    if (packet_byte &  MOUSE_DY_BIT9)
      packet_pp.delta_y = 0xFF00;
    else {
      packet_pp.delta_y = 0;
    }
    packet_pp.x_ov = packet_byte & MOUSE_X_OVF;
    packet_pp.y_ov = packet_byte & MOUSE_Y_OVF;
    break;
  }
  case 1: {
    packet_pp.delta_x = packet_pp.delta_x | ((int16_t) packet_byte);
    break;
  }
  case 2: {
    packet_pp.delta_y = packet_pp.delta_y | ((int16_t) packet_byte);
    break;
  }
  default:
    return;
  }

  if (pp_index >= 2){
    packet_read = true;
  }

  pp_index++;
}

int (enable_data_reporting)(){

  return write_command(ENBL_DATA_REP, NULL, NULL, 0);
}

int (disable_data_reporting)(){

  return write_command(DIS_DATA_REP, NULL, NULL, 0);
}

int (get_response)(uint8_t* response){

  uint8_t st;

  do {
    if (util_sys_inb(STATUS_PORT, &st))
      return 1;
    tickdelay(micros_to_ticks(DELAY_US));
  } while (!(st & OUT_BUF_READY));

  if (util_sys_inb(KBD_OUT_BUF, response))
    return 1;

  return 0;
}

int (wait_for_input_buffer)(){

  uint8_t st;

  do {
    if (util_sys_inb(STATUS_PORT, &st))
      return 1;
    tickdelay(micros_to_ticks(DELAY_US));
  } while (st & IN_BUF_READY);

  return 0;
}

int (write_to_mouse)(uint8_t cmd){

  if (wait_for_input_buffer())
    return 1;

  if (sys_outb(INPUT_CMD, REQUEST_FRWD))
    return 1;

  if (wait_for_input_buffer())
    return 1;

  if (sys_outb(INPUT_DATA, cmd))
    return 1;

  tickdelay(micros_to_ticks(DELAY_US));

  uint8_t aknowledgement_byte = 0;

  if (util_sys_inb(KBD_OUT_BUF, &aknowledgement_byte))
    return 1;
  

  if (aknowledgement_byte != SUCCESS)
    return 2;

  return 0;
}

int (write_command)(uint8_t cmd, uint8_t* response, uint8_t args[], uint32_t arg_n){

  int error;

  while((error = write_to_mouse(cmd))){
    if (error == 2)
      continue;
    else if (error == 1)
      return 1;
    else
      break;
  }

  for (int i = 0; i < (int) arg_n; i++)
    if (write_to_mouse(args[i]))
      return 1;

  if (response != NULL)
    get_response(response);

  return 0;
}
