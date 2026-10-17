#include <lcom/lcf.h>

#include "../macros.h"
#include "../keyboard/kbd.h"
#include "mouse.h"

#include "../video_graphics/graphics.h"

extern unsigned h_res;
extern unsigned v_res;

int mouse_hook_id = 2;

struct packet mpacket;
uint8_t byteIdx = 0;
uint8_t bytes[3];
uint8_t mouse_scancode;
int x = 200;
int y = 200;

void (mouse_ih)() {
  read_output(WRITE_COMAND_BACK, &mouse_scancode, 1);
}

bool (mouse_output)() {
  if (byteIdx == 0) {
    if (!(mouse_scancode & FIRST_BYTE)) return false;
    bytes[byteIdx]= mouse_scancode;
    byteIdx++;
  }
  else if (byteIdx == 1) {
    bytes[byteIdx] = mouse_scancode;
    byteIdx++;
  }
  else if (byteIdx == 2) {
    bytes[byteIdx] = mouse_scancode;
      byteIdx = 0;
      return true;
  }
  return false;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;

  *bit_no = BIT(mouse_hook_id);

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void (create_mpacket)() {

  for (int i = 0 ; i < 3 ; i++) {
    mpacket.bytes[i] = bytes[i];
  }

  mpacket.lb = bytes[0] & MOUSE_LB;
  mpacket.mb = bytes[0] & MOUSE_MB;
  mpacket.rb = bytes[0] & MOUSE_RB;

  mpacket.x_ov = bytes[0] & X_OVFL;
  mpacket.y_ov = bytes[0] & Y_OVFL;

  mpacket.delta_x = (bytes[0] & MSB_X_DELTA) ? (0xFF00 | bytes[1]) : bytes[1];
  mpacket.delta_y = (bytes[0] & MSB_Y_DELTA) ? (0xFF00 | bytes[2]) : bytes[2];

  if (x + mpacket.delta_x >= 0 && x + mpacket.delta_x <= (int) h_res) x += mpacket.delta_x;
  else if (x + mpacket.delta_x < 0) x = 0;
  else x = (int) h_res;
  if (y - mpacket.delta_y >= 0 && y - mpacket.delta_y <= (int) v_res) y -= mpacket.delta_y;
  else if (y - mpacket.delta_y < 0) y = 0;
  else y = (int) v_res;
}

int (mouse_data_report)(uint8_t command) {

  uint8_t attemps = MAX_ATTEMPS;
  uint8_t mouse_response;

  do {
    attemps--;
    if (write_command(IN_BUF_COMMANDS, WRITE_BYTE_TO_AUX_DEVICE)) return 1;
    if (write_command(OUT_BUF, command)) return 1;
    if (util_sys_inb(OUT_BUF, &mouse_response)) return 1;
    if (mouse_response == ACK) return 0;
  } while (mouse_response != ACK && attemps);

  return 1;
}

int write_command(uint8_t port, uint8_t cmd){
    uint8_t status;
    if(util_sys_inb(ST_REG, &status)){
        printf("Error reading the status\n");
        return 1;
    }

    if((status & IBF) == 0){
        if(sys_outb(port, cmd)){
            printf("Error writing the command\n");
            return 1;
        }
        return 0;
    }

    return 1;
}
