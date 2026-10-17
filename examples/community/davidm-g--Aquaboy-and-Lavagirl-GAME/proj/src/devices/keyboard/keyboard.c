#include "keyboard.h"

int kbd_hook_id = 1;
uint8_t kbd_outbuf;

void(kbc_ih)() {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf() == 0) {
      if (util_sys_inb(OUT_BUF, &kbd_outbuf) == 0) {
        return;
      }
    }
    attempts--;
  }
}
int wait_esc_key() {
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t bit_no = 0x00;
  if (keyboard_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  while (kbd_outbuf != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & bit_no) {
            kbc_ih();
          }
          break;
        default:
          break;
      }
    }
  }
  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }
  return 0;
}
int(keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(kbd_hook_id);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

int can_read_outbuf() {
  uint8_t st = 0;
  // read status
  if (util_sys_inb(ST_REGISTER, &st) != 0) {
    return 1;
  }
  // empty out_buf
  if ((st & OBF) == 0) {
    return 1;
  }
  // timeout error
  if ((st & TIM_ERR) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  // parity error
  if ((st & PAR_ERR) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  /*
  // mouse data in buffer
  if ((st & AUX) != 0) {
    util_sys_inb(OUT_BUF, &kbd_outbuf);
    return 1;
  }
  */
  return 0;
}

int write_to_kbc(uint8_t port, uint8_t cmdbyte) {
  uint8_t st;
  int attempts = 10;
  while (attempts > 0) {
    if (util_sys_inb(ST_REGISTER, &st) != 0) {
      return 1;
    } /* assuming it returns OK */ /* loop while 8042 input buffer is not empty */
    if ((st & IBF) == 0) {         // if input buffer is not full
      if (sys_outb(port, cmdbyte) != 0) {
        return 1;
      } /* no args command */
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int read_value_data_from_kbc(uint8_t port, uint8_t *output) {
  int attempts = 10;
  while (attempts > 0) {
    if (can_read_outbuf() == 0) {
      if (util_sys_inb(OUT_BUF, output) == 0) { // read data from output buffer
        return 0;
      }
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1; // if attempts end, throw error
}
int restore_interrupts() {
  uint8_t cmdbyte;
  if (write_to_kbc(KBC_CMD_REG, READ_CMD_BYTE))
    return 1; // telling kbc we are going to read command byte
  if (read_value_data_from_kbc(OUT_BUF, &cmdbyte))
    return 1; // retrieving command byte from output buffer
  printf("%x", cmdbyte);
  cmdbyte |= ENABLE_INTERRUPT;
  printf("%x", cmdbyte); // enabling interrupts on command byte
  if (write_to_kbc(KBC_CMD_REG, WRITE_CMD_BYTE))
    return 1; // telling kbc we are going to write command byte
  if (write_to_kbc(WRITE_CMD_BYTE, cmdbyte))
    return 1; // write the command byte
  return 0;
}
