#include "mouse.h"


struct packet mouse_packet;

int hookid_mouse = 3;
int byte_order_packet;
bool finished = false;


int mouse_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hookid_mouse);

  if (sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid_mouse) != 0) {
    printf("Error subscribing\n");
  }

  return 0;
}

int mouse_unsubscribe_int() {
  if (sys_irqrmpolicy(&hookid_mouse) != 0) return 1;

  return 0;
}

int kbc_restore_mouse() {
  if (issue_cmd_to_mouse(DISABLE_DATA_REP)) return 1;
  return 0;
}


void (mouse_ih)() {
  uint8_t status_reg;

  if (util_sys_inb(STATUS_PORT, &status_reg)) {
    printf("Error reading STATUS port !\n");
  }

  if (status_reg & BIT(0)) {
    if (status_reg & BIT(6)) {
      printf("Timeout error!\n");
      return;
    }
    if (status_reg & BIT(7)) {
      printf("Parity error!\n");
      return;
    }
    if (read_byte_to_mouse_packet() != 0) {
      printf("Error reading packet\n");
    }

    return;
  }
}

int issue_cmd_to_mouse(uint8_t cmd) {
  uint8_t obf;
  int attempts = 10;
  
  issue_cmd_to_KBC(STATUS_PORT,(uint8_t)MOUSE_WRITE_COM);
  issue_cmd_to_KBC(KBD_OUT_BUF,cmd);
  
  while (attempts != 0) {
    if (util_sys_inb(KBD_OUT_BUF, &obf) != 0) return 1;

    if (obf == ACK) {
      return 0;
    }

    tickdelay(micros_to_ticks(2000));
    attempts--;
  }

  return 1;
}

int read_data_from_KBC_mouse(uint8_t *data) {
  uint8_t stat;
  int attempts = 10;

  while( attempts != 0) {
    if (util_sys_inb(STATUS_PORT, &stat) != 0) {
      return 1;
    }
    if (stat & (BIT(0))) {
       // see if OBF is full
      util_sys_inb(KBD_OUT_BUF, data);
      if ((stat &(BIT(7)| BIT(6))) != 0) {
        return 1;
      }
      if (stat & BIT(5)) {
        return 0;
      }
      else return 1;
    }

    tickdelay(micros_to_ticks(2000));
    attempts--;
  }
  return 1;
}

int read_byte_to_mouse_packet() {
  uint8_t byte = 0;

  if (read_data_from_KBC_mouse(&byte)) {
    printf("Error reading packet bytes");
    return 1;
  }

  if (byte_order_packet == 0 && !(byte & BIT(3))) { // first byte of the packet must have bit 3 set!
    return 0;                                       // skip
  }

  mouse_packet.bytes[byte_order_packet] = byte;

  if (byte_order_packet == 2) {
    array_to_packet();
    finished = true;
    byte_order_packet = 0;
    return 0;
  }

  byte_order_packet++;

  return 0;
}

void array_to_packet() {
  mouse_packet.lb = mouse_packet.bytes[0] & BIT(0);
  mouse_packet.rb = mouse_packet.bytes[0] & BIT(1);
  mouse_packet.mb = mouse_packet.bytes[0] & BIT(2);
  mouse_packet.delta_x = (mouse_packet.bytes[0] & BIT(4)) ? (0xFF00 | mouse_packet.bytes[1]) : mouse_packet.bytes[1];
  mouse_packet.delta_y = (mouse_packet.bytes[0] & BIT(5)) ? (0xFF00 | mouse_packet.bytes[2]) : mouse_packet.bytes[2];
  mouse_packet.x_ov = mouse_packet.bytes[0] & BIT(6);
  mouse_packet.y_ov = mouse_packet.bytes[0] & BIT(7);
}

int issue_cmd_to_KBC(uint8_t port, uint8_t cmd) {
  uint8_t stat;
  int attempts = 10;
  while (attempts != 0) {
    if (util_sys_inb(STATUS_PORT, &stat) != 0) {
      return 1;
    }
    /* loop while 8042 input buffer is not empty */
    if ((stat & BIT(1)) == 0) {
      sys_outb(port, cmd); /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(2000)); // e.g. tickdelay()
    attempts--;
  }
  return 1; // ran out of attempts
}
