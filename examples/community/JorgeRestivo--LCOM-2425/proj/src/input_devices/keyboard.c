#include "keyboard.h"

uint8_t scancode = 0;
static int kb_id = 1;

int(keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(kb_id);   // ✅ Usa BIT(kb_id) para passar o valor correto da máscara

  return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_id);
}


int(keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&kb_id);
}

void(kbc_ih)(){
  uint8_t status;
  if (read_KBC_status(&status) != 0){
    return;
  }
  if ((status & BIT(0)) && !(status & BIT(5))) {
    read_KBC_output(0x60, &scancode, 0);
}
}


int(keyboard_restore)() {

  uint8_t commandByte;
  if (write_KBC_command(0x64, 0x20) != 0) {
    return 1;
  }
  if (read_KBC_output(0x60, &commandByte, 0) != 0) {
    return 1;
  }

  commandByte |= BIT(0);

  if (write_KBC_command(0x64, 0x60) != 0) {
    return 1;
  }
  if (write_KBC_command(0x60, commandByte) != 0) {
    return 1;
  }

  return 0;
}
