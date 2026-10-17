#include "kbd.h"
#include "i8042_kbd.h"

static int kbd_hook_id = 1;

char keys[0x90]; // value high enough to hold all scancodes
uint8_t scan_code[2];
int kbd_i = 0;
bool kbd_ih_flag = false;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = kbd_hook_id;
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

int (check_status)(uint8_t st) {
  if (st & PARITY) return PARITY_ERR;
  if (st & TIME_OUT) return TIME_OUT_ERR;
  if (st & AUX) return AUX_DATA;
  return OK;
}

int (kbd_get_status)(uint8_t *st) {
  return util_sys_inb(STATUS_REG, st);
}

int (kbd_read_out_buffer)(uint8_t *output) {
  return util_sys_inb(OUT_BUF, output);
}

int (kbd_write_cmdb)(int port, uint8_t cmdb) {
  int timeout = 0;
  uint8_t st;

  while (timeout < 3) {
    if (kbd_get_status(&st) != OK) {
      fprintf(stderr, "Error getting status while issuing command!\n");
      return -1;
    }

    if ((st & IBF) == 0) {
      if (sys_outb(port, cmdb) != OK) {
        fprintf(stderr, "Error writing command byte to input buffer!\n");
        return -1;
      }
      return OK;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    timeout++;
  }

  return -1;
}

int (kbd_read_ret_cmdb)(uint8_t *data) {
  int timeout = 0;
  uint8_t st;

  while (timeout < 3) {
    if (kbd_get_status(&st) != OK) {
      fprintf(stderr, "Error getting status when reading command byte return value!\n");
      return -1;
    }

    if ((st & OBF) && !(st & (PARITY | TIME_OUT | AUX))) {
      if (kbd_read_out_buffer(data) != OK) {
        fprintf(stderr, "Error reading KBC command byte return value from output buffer!\n");
        return -1;
      }
      return OK;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    timeout++;
  }

  return -1;
}

int (kbd_reenable_ints)() {
  uint8_t output;

  if (kbd_write_cmdb(KBC_CMD_REG, READ_CMDB) != OK) {
    fprintf(stderr, "Error writing command byte while reenabling interrupts!\n");
    return -1;
  }

  if (kbd_read_ret_cmdb(&output) != OK) {
    fprintf(stderr, "Error reading return command byte!\n");
    return -1;
  }

  output |= KBC_INT;

  if (kbd_write_cmdb(KBC_CMD_REG, WRITE_CMDB) != OK) {
    fprintf(stderr, "Error writing command byte while reenabling interrupts!\n");
    return -1;
  }

  if (kbd_write_cmdb(KBC_ARGS, output) != OK) {
    fprintf(stderr, "Error writing command byte arguments to input buffer!\n");
    return -1;
  }

  return OK;
}

void (kbc_ih) () {
  uint8_t output;
  kbd_read_out_buffer(&output);

  uint8_t st;
  kbd_get_status(&st);

  if (check_status(st) == OK) {
    scan_code[kbd_i] = output;

    if (output != TWO_BYTE_SC) {
      kbd_ih_flag = !(output & IS_BREAK);
    }
  }
}
