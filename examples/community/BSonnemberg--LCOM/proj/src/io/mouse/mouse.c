#include "mouse.h"
#include "../kbc.h"
#include "../../vector2.h" 

static int hook_id = MOUSE_IRQ;

// -1 signals out of sync, leads to packet reread
static int8_t idx = -1;
static uint8_t byte = 0x0;

int mouse_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = hook_id;
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int mouse_unsubscribe_int() {
  return sys_irqrmpolicy(&hook_id);
}

/*void set_mouse_sample_rate(){
    uint32_t ack, p;
    int status;

    // Envie comando para endereçar o mouse
    sys_outb(0xD4, 0x64);
    sys_outb(0xF3, 0x60);

    // Espere até que possamos ler
    do {
        sys_inb(0x64, &p);
    } while (!(p & 1));
    asm("pause");

    // Leia a confirmação, esperando 0xFA
    sys_inb(0x60, &ack);
    if (ack != 0xFA) {
        // Lidar com erro de confirmação aqui
    }

    // Envie comando novamente para endereçar o mouse
    sys_outb(0xD4, 0x64);
    sys_outb(100, 0x60);  // Define a taxa de amostragem para 100

    // Esperar novamente até que possamos ler
    do {
        status = sys_inb(0x64, &p);
    } while (!(p & 1) && status == OK);
    asm("pause");

    // Leia a confirmação novamente
    sys_inb(0x60, &ack);
    if (ack != 0xFA) {
        printf("error");
    }
}*/


void (mouse_ih)() {
  if (kbc_read_outbuf(&byte) != 0) {
    printf("failed to read output buffer in ih");
    byte = 0x0;
    idx = -1;
  }
}

int mouse_parse_packet(struct packet *pp) {
  if (pp == NULL) {
    return 2;
  }
  if (idx == -1) {
    // out of sync, try to fix
    if (byte & BIT(3)) {
      idx = 0;
    }
    else return 2;
  }
  switch (idx) {
    case 0:
      pp->lb = byte & MOUSE_LB;
      pp->rb = byte & MOUSE_RB;
      pp->mb = byte & MOUSE_MB;
      pp->x_ov = byte & MOUSE_OVFX;
      pp->y_ov = byte & MOUSE_OVFY;
      break;
    case 1:
      pp->delta_x = byte;
      if (pp->bytes[0] & MOUSE_SIGN_X) {
        pp->delta_x |= 0xFF00;
      }
      break;
    case 2:
      pp->delta_y = byte;
      if (pp->bytes[0] & MOUSE_SIGN_Y) {
        pp->delta_y |= 0xFF00;
      }
      break;
  }
  pp->bytes[idx] = byte;
  if (idx != 2) {
    // incomplete packet
    idx++;
    return 1;
  }
  idx = -1;
  return 0;
}

int mouse_write_cmd(uint8_t cmd) {
  uint8_t tries = 3;
  while (tries != 0) {
    // get kbc ready to send cmd
    if (kbc_write(KBC_CMD_REG, MOUSE_CMD) != 0) {
      return 2;
    }
    // send command itself
    if (kbc_write(KBC_INBUF, cmd) != 0) {
      return 2;
    }
    uint8_t res;
    if (kbc_read_outbuf(&res) != 0) {
      return 3;
    }
    if (res == MOUSE_ACK) {
      return 0;
    }
    tries--;
  }
  return TIMEOUT;
}

int mouse_enable_data_rep() {
  return mouse_write_cmd(0xF4);
}

int mouse_disable_data_rep() {
  return mouse_write_cmd(0xF5);
}

int mouse_enable_stream() {
  return mouse_write_cmd(0xEA);
}
