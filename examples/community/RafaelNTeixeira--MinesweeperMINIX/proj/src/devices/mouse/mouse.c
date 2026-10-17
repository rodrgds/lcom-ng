#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "mouse.h"
#include "i8042_mouse.h"

static int mouse_hook_id = IRQ12_MOUSE;
uint8_t scan_bytes_mouse[3];
uint8_t scancode;
bool codecomplete_mouse = false;
int nrbytes_mouse = 0;
struct packet pkt;

uint16_t convert_2_complement(uint8_t number, uint8_t msb, struct packet *pkt) {
  if (pkt->bytes[0] & msb) return number - 256;
  return number;
}

int mouse_subscribe_int(uint8_t *bit_no){
  *bit_no = mouse_hook_id;
  return sys_irqsetpolicy(IRQ12_MOUSE, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hook_id);
}

int mouse_unsubscribe_int(){
  return sys_irqrmpolicy(&mouse_hook_id);
}

bool (check_input_buffer)() {
  uint8_t stat;

  if (util_sys_inb(MOUSE_CMD_REG, &stat) != 0) return false;

  if (stat & IBF) 
    return false;
  else
    return true;
}

bool (check_output_buffer)() {
  uint8_t stat;

  if (util_sys_inb(MOUSE_CMD_REG, &stat) != 0) return false;

  if (stat & OBF) {
    if ((stat & (PARITY | TIMEOUT)) != 0)
      return false;
    else
      return true;
  }
  else
    return false;
  
}

void (mouse_ih)(){
  while (1) {
    if (check_output_buffer()) {
      if (util_sys_inb(MOUSE_OUT_BUF, &scancode) != 0) {scancode = 0x00;}
      break;
    }
    else {
      scancode = 0x00;
      break;
    }
  }
}

void (buildPacket)(struct packet *pkt) {
  pkt->lb = pkt->bytes[0] & LB;
  pkt->rb = pkt->bytes[0] & RB;
  pkt->mb = pkt->bytes[0] & MB;

  pkt->delta_x = convert_2_complement(pkt->bytes[1], MSB_X, pkt);
  pkt->delta_y = convert_2_complement(pkt->bytes[2], MSB_Y, pkt);

  pkt->x_ov = pkt->bytes[0] & X_OVFL;
  pkt->y_ov = pkt->bytes[0] & Y_OVFL;
}

int (mouse_enable_data_report)() {
  uint8_t resp;
  uint8_t seconderror = 0;

 while (seconderror < 3) {
    if (sys_outb(MOUSE_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
    if (mouse_write_cmd(DT_REP_ENABLE, &resp) != 0) return 1;

    if (resp == ACK) return 0;
    else if (resp == NACK) seconderror++;
    else if (resp == ERROR) return 1;
  }
  return 1;
}

int (mouse_disable_data_report)() {
  uint8_t resp;
  uint8_t seconderror = 0;

   while (seconderror < 3) {
    if (sys_outb(MOUSE_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
    if (mouse_write_cmd(DT_REP_DISABLE, &resp) != 0) return 1;

    if (resp == ACK) return 0;
    else if (resp == NACK) seconderror++;
    else if (resp == ERROR) return 1;
  }
  return 1;
}

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp) {
  int count = 0;

  while (count < 3) {
    if (check_input_buffer()) {
      if (sys_outb(WCB, cmd) != 0) return 1;
      if (util_sys_inb(MOUSE_OUT_BUF, resp) != 0) return 1;
      return 0;
    }
    count++;
  }
  return 1;
}

int (mouse_stream)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  while (seconderror < 3) {
    if (sys_outb(MOUSE_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
    if (mouse_write_cmd(SET_STM_MODE, &resp) != 0) return 1;

    if (resp == ACK) return 0;
    else if (resp == NACK) seconderror++;
    else if (resp == ERROR)return 1;
  }
  return 1;
}

int (mouse_read_data)() {
  uint8_t resp;
  uint8_t seconderror = 0;

   while (seconderror < 3) {
    if (sys_outb(MOUSE_CMD_REG, WRITE_TO_MOUSE) != 0) return 1;
    if (mouse_write_cmd(READ_DATA, &resp) != 0) return 1;

    if (resp == ACK) {mouse_ih(); return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
  }
  return 1;
}
