#include "keyboard.h"
#include "KBC.h"

uint8_t scancode[2];
int keyboard_hook_id = 1;
int kbd_index = 0;

bool kbd_ih_flag = false;


int (kbd_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;
  *bit_no = (uint8_t)keyboard_hook_id;
  if (sys_irqsetpolicy(IRQ_KBC, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) {
    printf("kbd_subscribe_int: Kernel alocou/usou hook_id = %d para TIMER0_IRQ\n", keyboard_hook_id);
    return 1;
  }
  return 0;
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&keyboard_hook_id);
}

int (kbd_get_status)(uint8_t *st) {
  return util_sys_inb(KBC_STAT_REG, st);
}

int (kbd_read_out_buffer)(uint8_t *output) {
  return util_sys_inb(KBC_OUT_CMD , output);
}

int (check_status)(uint8_t st) {
  if (st & ERR_PARITY) return 1;
  if (st & ERR_TIMEOUT) return 1;
  if (st & BIT(5)) return 4;
  return 0;
}

void (kbc_ih)() {
  uint8_t output;
  kbd_read_out_buffer(&output);

  uint8_t st;
  kbd_get_status(&st);

  if (check_status(st) == OK) {
    scancode[kbd_index] = output;

    if (output != TWO_BYTE_CODE) {
      kbd_ih_flag = !(output & MAKE_CODE);
    }
  }
}

/*
int(kbd_restore)() {
    uint8_t commandByte;

    if(kbc_write_byte(KBC_IN_CMD, KBC_READ_CMD) != 0) return 1;
    if(kbc_read_byte(KBC_OUT_CMD, 0) != 0) return 1;

    commandByte |= ENABLE_INT;

    if(kbc_write_byte(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;
    if(kbc_write_byte(KBC_WRITE_CMD, commandByte) != 0) return 1;

    return 0;
}*/
