#include <lcom/lcf.h>

#include "controller/input/input.h"

Key(translate_scancode)(uint8_t sc) {
  switch (sc) {
    // arrow keys
    case 0x48: return KEY_UP;
    case 0x50: return KEY_DOWN;
    case 0x4B: return KEY_LEFT;
    case 0x4D: return KEY_RIGHT;

    // WASD keys
    case 0x11: return KEY_UP;
    case 0x1F: return KEY_DOWN;
    case 0x1E: return KEY_LEFT;
    case 0x20: return KEY_RIGHT;

    case 0x1C: return KEY_ENTER;
    case 0x39: return KEY_SPACE;
    case 0x01: return KEY_ESCAPE;
    default: return KEY_NONE;
  }
}
