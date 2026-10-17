#include <lcom/lcf.h>

#include "KBC.h"
#include <minix/sysutil.h>

int keyboard_hook = 1;
int keyboard_counter = 0;
uint8_t scancode = 0;

void(kbc_ih)() {
  if (read_KeyboardController_Output(KBC_COMMAND_REG, &scancode) != 0)
    printf("Error reading scancode\n");
}

int(read_KeyboardController_Output)(uint8_t port, uint8_t *data) {
  uint8_t stat;
  
  if (util_sys_inb(STATUS_REGISTER_PORT, &stat) != 0) {
    printf("Error reading status\n");
    return 1;
  }
  
  if (stat & BIT(0)) {
    // Read the data
    if (util_sys_inb(OUTPUT_BUFFER_PORT, data) != 0)
      return 1;
    
    if ((stat & (KBC_PAR_ERR | KBC_TO_ERR)) != 0) {
      return 1;
    }
    return 0;
  }
  return 1;
}

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(keyboard_hook);
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook) != 0)
    return 1;

  return 0;
}

int(kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&keyboard_hook) != 0)
    return 1;
  return 0;
}
