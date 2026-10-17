#include "keyboard.h"
#include "lcom/lcf.h"

#include "i8042.h"
#include "kbc.h"

int kb_hook_id = 1;
uint8_t scancode;
bool is_scancode_invalid = 1;

int(keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(kb_hook_id);

  return (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_hook_id));  
}

int(keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&kb_hook_id);
}

void(kbc_ih)(void) {
  if (kbc_read_output(&scancode, 0)) {
    is_scancode_invalid = 1;
  }
  is_scancode_invalid = 0;
}

int (keyboard_restore_interrupts)() {
  uint8_t commandByte;

  if (kbc_write_command(KBC_CMD_REG, KBD_READ_CMD)) return 1;
  if (kbc_read_output(&commandByte, 0)) return 1;

  commandByte |= KBC_KB_INTERRUPT_ENABLE;
  if (kbc_write_command(KBC_CMD_REG, KBD_WRITE_CMD)) return 1;
  if (kbc_write_command(KBC_IN_BUF, commandByte)) return 1;

  return 0;
}

int (process_keyboard_esc)() {
  int ipc_status, r;
  message msg;
  uint8_t irq_set;
  if (keyboard_subscribe_int(&irq_set)) return 1;

  while (scancode != ESC_BREAK_CODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int()) return 1;

  return 0;
}
