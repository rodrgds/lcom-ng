#include "../headers/mouse.h"

static int hookid = 2; // timer - 0, kb - 1, mouse - 2
struct packet pp;
unsigned mouse_counter = 0;

int (subscribe_mouse)(uint8_t *bit_no) {
  *bit_no = hookid;
  if (sys_irqsetpolicy(IRQ12 , IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid)) {
    return 1;
  }
  return 0;
}

int enable_mouse() {
  return sys_irqenable(&hookid);
}

int disable_mouse() {
  return sys_irqdisable(&hookid);
}

int (unsubscribe_mouse)() {
  uint8_t status;

  if (sys_irqrmpolicy(&hookid)) {
    return 1;
  }

  mouse_get_status_byte(&status);

  if (status & OBF) {
    util_sys_inb(OUT_BUF, &status);
  }

  return 0;
}

int mouse_get_status_byte(uint8_t *status) {
  uint8_t mouse_status;
  if (util_sys_inb(KBD_STAT_REG, &mouse_status)) {
    return 1;
  }
  *status = mouse_status;
  return 0;
}

void (mouse_ih)(void) {
  uint8_t status, byte;
  
  if (mouse_get_status_byte(&status)) {
    return;
  }

  if (status & OBF) {
    if(util_sys_inb(OUT_BUF, &byte)) {
      return;
    }
    //printf("%d\n",mouse_counter);
    if (mouse_counter % 3 == 0) { // Synchronize
      if (byte & BIT(3)) {
        pp.bytes[mouse_counter % 3] = byte;
        mouse_counter++;
      } else {
        return;
      }
    } else {
      pp.bytes[mouse_counter % 3] = byte;
      mouse_counter++;
    }
  }
}

void assemble_packet(struct packet *pp) {
  pp->lb = pp->bytes[0] & LB;
  pp->rb = pp->bytes[0] & RB;
  pp->mb = pp->bytes[0] & MB;
  pp->x_ov = pp->bytes[0] & X_OVF;
  pp->y_ov = pp->bytes[0] & Y_OVF;
  if (pp->bytes[0] & MSB_X_DELTA) {
    pp->delta_x = pp->bytes[1] - 256;
  } else {
    pp->delta_x = pp->bytes[1];
  }
  if (pp->bytes[0] & MSB_Y_DELTA) {
    pp->delta_y = pp->bytes[2] - 256;
  } else {
    pp->delta_y = pp->bytes[2];
  }
}

int mouse_cmd_write(uint8_t command) {
  uint8_t status, ackn;

  // Read status byte to see if IBF is full
  // Send command to mouse
  // Re-send command if failed in one byte
  // Halt execution if error in two consecutive bytes

  do {
    if (mouse_get_status_byte(&status)) {
      return 1;
    }
    if (status & IBF) {
      continue;
    }
    if (sys_outb(KBC_CMD_REG, WRITE_TO_MOUSE)) {
      return 1;
    }

    if (mouse_get_status_byte(&status)) {
      return 1;
    }
    if (status & IBF) {
      continue;
    }
    if (sys_outb(KBC_ARG_REG, command)) {
      return 1;
    }

    tickdelay(micros_to_ticks(20000));

    if (util_sys_inb(OUT_BUF, &ackn)) {
      return 1;
    }

    if (ackn == ERROR) {
      return -1;
    }
  } while (ackn != ACK);
  
  return 0;
}
