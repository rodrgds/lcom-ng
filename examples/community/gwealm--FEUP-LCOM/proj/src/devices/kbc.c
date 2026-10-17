#include "devices/kbc.h"
#include "utils.h"

int (kbc_read_status)(struct kbc_status *stat) {
  uint8_t stat_word;
  if (util_sys_inb(KBC_REG_STAT, &stat_word) != OKAY) {
    printf("kbc_read_status: could not read status register\n");
    return KBC_EREAD;
  }

  stat->parity_error = stat_word & KBC_STAT_PARITY_ERR;
  stat->timeout_error = stat_word & KBC_STAT_TIMEOUT_ERR;
  stat->aux = stat_word & KBC_STAT_AUX;
  stat->keyboard_inhibited = stat_word & KBC_STAT_KB_INH;
  stat->a2 = stat_word & KBC_STAT_A2;
  stat->sys = stat_word & KBC_STAT_SYS;
  stat->input_buffer_full = stat_word & KBC_STAT_IBF;
  stat->output_buffer_full = stat_word & KBC_STAT_OBF;

  return OKAY;
}

int (kbc_send_byte)(int port, uint8_t byte) {
  if (port != KBC_REG_CTRL && port != KBC_REG_IN_BUF) {
    printf("kbc_send_byte: provided port (0x%x) is not on kbc\n", port);
    return KBC_EPORT;
  }

  struct kbc_status stat;

  int triesLeft = KBC_TIMEOUT_TRIES;
  while (triesLeft > 0) {
    if (kbc_read_status(&stat) != OKAY) {
      printf("kbc_send_byte: could not read status register\n");
      return KBC_EREAD;
    }

    if (!stat.input_buffer_full) {
      if (sys_outb(port, byte) != 0) {
        printf("kbc_send_byte: could not send byte\n");
        return KBC_EWRITE;
      }

      return OKAY;
    }

    tickdelay(micros_to_ticks(KBC_DELAY_US));
    triesLeft--;
  }

  printf("kbc_send_byte: number of tries exceeded\n");
  return KBC_ETIMEOUT;
}

int (kbc_read_byte)(int port, uint8_t *byte) {
  if (port != KBC_REG_STAT && port != KBC_REG_OUT_BUF) {
    printf("kbc_read_byte: provided port (0x%x) is not on kbc\n", port);
    return KBC_EPORT;
  }

  struct kbc_status stat;

  int triesLeft = KBC_TIMEOUT_TRIES;
  while (triesLeft > 0) {
    if (kbc_read_status(&stat) != OKAY) {
      printf("kbc_read_byte: could not read status register\n");
      return KBC_EREAD;
    }

    if (stat.output_buffer_full) {
      if (util_sys_inb(port, byte) != 0) {
        printf("kbc_read_byte: could not send byte\n");
        return KBC_EREAD;
      }

      if (stat.parity_error || stat.timeout_error) {
        return KBC_EVALIDITY;
      }

      return OKAY;
    }
    
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    triesLeft--;
  }

  printf("kbc_read_byte: number of tries exceeded\n");
  return KBC_ETIMEOUT;
}

int (kbc_send_command)(enum kbc_command_dest dest, uint8_t *bytes, size_t n) {
  int err_no;

  int triesLeft = KBC_TIMEOUT_TRIES;
  while (triesLeft > 0) {
    bool sucess = true;

    for (size_t i = 0; i < n; i++) {
      if ((err_no = kbc_send_byte(KBC_REG_CTRL, dest)) != OKAY) {
        printf("kbc_send_command: could not send byte 0x%x to port 0x%x\n", dest, KBC_REG_CTRL);
        return err_no;
      }

      if ((err_no = kbc_send_byte(KBC_REG_IN_BUF, bytes[i])) != OKAY) {
        printf("kbc_send_command: could not send byte 0x%x to port 0x%x\n", bytes[i], KBC_REG_IN_BUF);
        return err_no;
      }

      uint8_t ack;
      if ((err_no = kbc_read_byte(KBC_REG_OUT_BUF, &ack)) != OKAY) {
        printf("kbc_send_command: could not read byte from port 0x%x\n", KBC_REG_OUT_BUF);
        return err_no;
      }

      if (ack == KBC_ACK_ERROR) {
        printf("kbc_send_command: received ERROR acknowledgment\n");
        return KBC_EACK;
      } else if (ack == KBC_NACK) {
        sucess = false;
        break;
      } else if (ack != KBC_ACK) {
        printf("kbc_send_command: unrecognized acknowledgment (0x%x) received\nkbc_send_command: Skipping...\n", ack);
      }
    }

    if (sucess) {
      return OKAY;
    }

    triesLeft--;
  }

  printf("kbc_send_command: number of tries exceeded\n");
  return KBC_ETIMEOUT;
}
