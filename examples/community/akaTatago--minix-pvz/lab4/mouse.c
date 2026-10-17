#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"


static int mouse_hook_id = 12; 
uint8_t mouse_byte;
bool mouse_error = false;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = mouse_hook_id;
  
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) {
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hook_id) != 0) {
    return 1;
  }
  return 0;
}

void (mouse_ih)() {
  uint8_t status;
  mouse_error = true;
  
  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return;
  
  if ((status & KBC_OBF) && (status & KBC_AUX)) { 
    if (util_sys_inb(0x60, &mouse_byte) != 0) return;
    
    if ((status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0) { 
      mouse_error = false;
    }
  }
}

int mouse_write_cmd(uint8_t cmd) {
  uint8_t status, ack;
  uint8_t max_attempts = 10;
  
  while (max_attempts > 0) {
    if (util_sys_inb(0x64, &status) != 0) return 1;
    
    if ((status & KBC_IBF) == 0) { 
      if (sys_outb(0x64, 0xD4) != 0) return 1; 
    }
    
    if (util_sys_inb(0x64, &status) != 0) return 1;
    if ((status & KBC_IBF) == 0) {
      if (sys_outb(0x60, cmd) != 0) return 1; 
      if (util_sys_inb(0x60, &ack) != 0) return 1; 
      if (ack == 0xFA) return 0; 
    }
    max_attempts--;
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
}

void mouse_parse_packet(struct packet *pp, uint8_t bytes[]) {
  for (int i = 0; i < 3; i++) {
    pp->bytes[i] = bytes[i];
  }
  pp->lb = bytes[0] & BIT(0);
  pp->rb = bytes[0] & BIT(1);
  pp->mb = bytes[0] & BIT(2);
  pp->delta_x = (bytes[0] & BIT(4)) ? (0xFF00 | bytes[1]) : bytes[1];
  pp->delta_y = (bytes[0] & BIT(5)) ? (0xFF00 | bytes[2]) : bytes[2];
  pp->x_ov = bytes[0] & BIT(6);
  pp->y_ov = bytes[0] & BIT(7);
}


int (mouse_enable_data_report)() {
  return mouse_write_cmd(0xF4);
}

int (mouse_disable_data_report)() {
  return mouse_write_cmd(0xF5);
}
