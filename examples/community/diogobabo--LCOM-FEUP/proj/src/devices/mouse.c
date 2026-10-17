#include "mouse.h"

int hook_id_m = 6;
int flag_m = 0;
uint8_t byte;
uint8_t scancode;

void (mouse_ih)() {
  uint8_t st;
  flag_m = 0;
  if(util_sys_inb(STAT_REG,&st) != 0) {
    flag_m = 1;
  }
  if((st & KBC_OBF) == 0) {
    printf("OBF not full");
    flag_m = 1;
    return;
  } 
  if(util_sys_inb(OUT_BUF, &scancode) != 0) {
    flag_m = 1;
  }
  if((st & ERROR_PARITY_TIMEOUT) != 0) {
    printf("Error Parity / Timeout");
    flag_m = 1;
  }
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_m;
  if(sys_irqsetpolicy(IRQ_MOUSE,(IRQ_REENABLE | IRQ_EXCLUSIVE) ,&hook_id_m) != 0) {
    printf("Error mouse subscribe");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_m) != 0) {
    printf("Error mouse unsubscribe");
    return 1;
  }
  return 0;
}


void (mouse_set_packet)(struct packet* pacote) {

  pacote->lb = pacote->bytes[0] & LEFT_BUTTON;
  
  pacote->rb = pacote->bytes[0] & RIGHT_BUTTON;

  pacote->mb = pacote->bytes[0] & MIDDLE_BUTTON;
  
  if(pacote->bytes[0] & Y_SIGN) {
    pacote->delta_y = pacote->bytes[2] - 256;
  }
  else {
    pacote->delta_y = pacote->bytes[2];
  }

  if(pacote->bytes[0] & X_SIGN) {
    pacote->delta_x = pacote->bytes[1] - 256;
  }
  else {
    pacote->delta_x = pacote->bytes[1];
  }

  pacote->x_ov = pacote->bytes[0] & X_OVERFLOW;

  pacote->y_ov = pacote->bytes[0] & Y_OVERFLOW;
}

int (check_ibf_full)() {
  uint8_t st;
  if(util_sys_inb(STAT_REG, &st) != 0) {return 1;}
  if((st & IBF_BIT) != 0) {
    printf("IBF Full");
    return 1;
  }
  return 0;
}

int (send_mouse_cmd)(uint8_t cmd) {
  int end = 1;
  uint8_t st;
  while(end) {
    if(check_ibf_full() != 0) {
      return 1;
    }
    if(sys_outb(STAT_REG, WRITE_BYTE) != 0) {
      return 1;
    }
    if(check_ibf_full() != 0) {
      return 1;
    }
    if(sys_outb(WRITE_PORT, cmd) != 0) {
      return 1;
    }
    if(util_sys_inb(OUT_BUF, &st) != 0) {
      return 1;
    }
    if(st == ACK) {
      end = 0;
    }
    else if(st == ERROR) {
      return 1;
    }
    else {
      continue;
    }
  }
  return 0;
}

int (mouse_disable_data_reporting)() {
  if(send_mouse_cmd(DISABLE_DATA) != 0) {return 1;}
  return 0;
}
