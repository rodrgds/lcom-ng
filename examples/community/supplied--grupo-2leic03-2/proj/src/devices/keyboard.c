#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"
#include "../utils/utils.h"

#define KEYBOARD_IRQ 1

#define KBC_STATUS_REG 0x64
#define KBC_OUT_BUF 0x60

#define KBC_OBF BIT(0)
#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)

#define ESC_MAKE 0x01
#define ESC_BREAK 0x81

#define A_MAKE 0x1E
#define A_BREAK 0x9E

#define D_MAKE 0x20
#define D_BREAK 0xA0

#define W_MAKE 0x11
#define W_BREAK 0x91

#define S_MAKE 0x1F
#define S_BREAK 0x9F

#define LEFT_ARROW_MAKE 0x4B
#define LEFT_ARROW_BREAK 0xCB

#define RIGHT_ARROW_MAKE 0x4D
#define RIGHT_ARROW_BREAK 0xCD

#define UP_ARROW_MAKE 0x48
#define UP_ARROW_BREAK 0xC8

#define DOWN_ARROW_MAKE 0x50
#define DOWN_ARROW_BREAK 0xD0

#define TWO_BYTE_CODE 0xE0

static int keyboard_hook_id = KEYBOARD_IRQ;
static uint8_t scancode = 0;

int keyboard_subscribe(uint32_t *irq_set) {
  if (irq_set == NULL) {
    return 1;
  }

  keyboard_hook_id = KEYBOARD_IRQ;
  *irq_set = BIT(keyboard_hook_id);

  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != OK) {
    return 1;
  }

  return 0;
}

int keyboard_unsubscribe(void) {
  if (sys_irqrmpolicy(&keyboard_hook_id) != OK) {
    return 1;
  }

  return 0;
}

void keyboard_ih(void) {
  uint8_t status;

  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return;
  if (!(status & KBC_OBF)) return;

  util_sys_inb(KBC_OUT_BUF, &scancode);

  if (status & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) {
    scancode = 0;
    return;
  }
}

uint8_t keyboard_get_scancode(void) {
  return scancode;
}
