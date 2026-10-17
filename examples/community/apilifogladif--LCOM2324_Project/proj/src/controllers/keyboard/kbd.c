#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "kbd.h"
#include "../macros.h"
#include "../utils/utils.c"

int hook_kbc = 1;

int(kbd_subscribe_int)(uint8_t *IRQ) {
  if(IRQ == NULL) {
    printf("Bit_no is a null pointer\n");
    return 1;
  }
  *IRQ = BIT(hook_kbc);

  sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_kbc);
  return 0;
}

int(kbd_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_kbc);
  return 0;
}

int (read_output)(uint8_t port, uint8_t *scancode, uint8_t mouse) {
  uint8_t st = 0;

  if (util_sys_inb(ST_REG, &st) != 0) {
    printf("Error (getting stats)");
    return 1;
  }

  if (OBF & st) {
    if (util_sys_inb(port, scancode) != 0) {
      printf("Error (reading buffer)");
      return 1;
    }

    if (((st & (PARITY_ERROR | TIMEOUT_ERROR)) != 0)) {
      return 1;
    }

    if (mouse && ((st & AUX) == 0)) return 1;
    if (!mouse && ((st & AUX) != 0)) return 1;

    return 0;
  }
  return 1;
}

int (restoreKBD)() {
  uint8_t cmd;
  if (sys_outb(IN_BUF_COMMANDS, READ_COMMAND_BACK) != 0) return 1;
  if (util_sys_inb(OUT_BUF, &cmd) != 0) return 1;
  cmd |= BIT(0);
  if (sys_outb(IN_BUF_COMMANDS, WRITE_COMAND_BACK) != 0) return 1;
  if (sys_outb(IN_BUF_ARGUMENTS, cmd) != 0) return 1;
  return 0;
}
