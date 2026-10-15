#include <lcom/lcf.h>
#include "kbc.h"

uint8_t scancode_byte = 0;
int kbd_hook_id = 1;

uint8_t get_scancode_byte() {
  return scancode_byte; 
}

int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = (uint8_t) kbd_hook_id;
  return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

int (kbc_read_value)(uint8_t *value) {
  if (value == NULL) return 1;

  uint8_t status;
  if (util_sys_inb(ST_REG, &status) != 0) return 1;

  if ((status & KBC_OBF) == 0) return 1;

  if (util_sys_inb(OUT_BUFFER, value) != 0) return 1;

  return 0;
}

int (kbc_write_command)(int port, uint8_t cmd) {
  uint8_t status;
  int attempts = 10;

  while (attempts-- > 0) {
    if (util_sys_inb(ST_REG, &status) != 0) return 1;

    if ((status & KBC_IBF) == 0) {
      return sys_outb((port_t) port, cmd) != OK;
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int (kbc_enable_interrupts)() {
  uint8_t command_byte;

  if (kbc_write_command(CMD_REG, READ_CMD_BYTE) != 0) return 1;
  if (util_sys_inb(OUT_BUFFER, &command_byte) != 0) return 1;

  command_byte |= KBC_INT_BIT;

  if (kbc_write_command(CMD_REG, WRITE_CMD_BYTE) != 0) return 1;
  if (kbc_write_command(IN_BUFFER, command_byte) != 0) return 1;

  return 0;
}

// interrupt handler do teclado
void (kbc_ih)() {
  uint8_t status;
  // ler o status do kbc para perceber ver se temos scancode para ler e se nao ha erros
  if (util_sys_inb(ST_REG, &status) != 0) return;

  if (status & KBC_OBF) {
    uint8_t data;
    if (util_sys_inb(OUT_BUFFER, &data) != 0) return;

    // erros de parity e timeout
    if (!(status & (KBC_PARITY | KBC_TIMEOUT))) {
      scancode_byte = data;
    }
  }
}

int (wait_esc)() {
  int ipc_status;
  message msg;
  uint8_t bit_no;

  if (kbd_subscribe_int(&bit_no) != 0) return 1;

  uint32_t irq_set = BIT(bit_no);
  bool active = true;

  while (active) {
    int r;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();

            if (scancode_byte == ESC_BREAK_CODE) {
              active = false;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  return kbd_unsubscribe_int();
}