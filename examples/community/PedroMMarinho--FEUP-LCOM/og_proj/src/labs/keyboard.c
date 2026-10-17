#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"
#include "kbc.h"

static uint8_t scancode;
static int keyboard_hookId = 1;
static bool errorIH = false;

void(kbc_ih)() {
  if (read_outputBuffer(&scancode)) errorIH = true;
}

bool keyboardIHError(){
  return errorIH;
}

int(keyboard_subscribe)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(keyboard_hookId);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hookId);
}

int(keyboard_unsubscribe)() {
  return sys_irqrmpolicy(&keyboard_hookId);
}

uint8_t getKeyboardScancode() {
  return scancode;
}
