#include "mouse.h"

int mouse_hook_id = 2;
int c = 0;
uint8_t MSBY = 0;
uint8_t MSBX = 0;

int sub_mouse_int(uint8_t* mask) {
  if (mask == NULL) return 1;
  *mask = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;
  return 0;
}

int unsub_mouse_int() {
  if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
  return 0;
}

void (mouse_ih)() {   
  c++;
  if (c == 3) {
    if (read_packet_byte() != 0) {
      c--;
    }
    else {
      packet_complete = true;
      c = 0;
    }
  }
  else {
    if (read_packet_byte() != 0) {
      c--;
    }
    else {
      packet_complete = false;
    }
  }
}

int (write_mouse_command)(uint8_t command) {
  int trys = 10;
  uint8_t response;
  while (trys > 0) {
    if (write_command_byte(KBC_CMD_REG, WB_TO_MOUSE) != 0) return 1;
    if (write_command_byte(KBC_IN_BUF, command) != 0) return 1;
    tickdelay(micros_to_ticks(20000));
    if (mouse_read_command_byte(&response) != 0) return 1;
    if (response == ACK)  return 0;
  }
  return 1;
}

int (read_packet_byte)() {
  uint8_t status, byte;
  int trys = 20;
  while(trys > 0) {
    aux_sys_inb(KBC_ST_REG, &status); 
    if( status & KBC_OBF ) {
      if (aux_sys_inb(KBC_OUT_BUF, &byte) == OK); 
      if (((status &(KBC_PAR_ERR | KBC_TO_ERR)) == 0) && (MOUSE_DATA & status)) {
          switch (c)
          {
            case 1:
              if (byte & BIT(3)) {
                p.bytes[0] = byte;
                p.y_ov = (byte >> 7) & 0x01;
                p.x_ov = (byte >> 6) & 0x01;
                MSBY = (byte >> 5) & 0x01;
                MSBX = (byte >> 4) & 0x01;
                p.mb = (byte >> 2) & 0x01;
                p.rb = (byte >> 1) & 0x01;
                p.lb = byte & 0x01;  
                break;
              }
              else {
                c--;
                break;
              }
            case 2:
              p.bytes[1] = byte;
              p.delta_x = (MSBX == 1) ? (0xFF00 | byte) : byte;
              MSBX = 0;
              break;
            case 3:
              p.bytes[2] = byte;
              p.delta_y = (MSBY == 1) ? (0xFF00 | byte) : byte;
              MSBY = 0;
              break;
            default:
              break;
          }
          return 0;
      }
    }
  tickdelay(micros_to_ticks(20000)); 
  trys--;
  }
  return 1;
}


