#include "keyboard.h"
#include "../utils.h"
#include "../../sprites/sprites.h"

// Counter for keyboard interrupts (can be used for debugging/performance)
uint32_t counter_KBC = 0;
// Stores the last keycode detected (used by other parts of the program)
uint8_t last_keycode = 0;
// ID used when subscribing/unsubscribing keyboard interrupts
static int kbd_int_id = 1;
// Holds scancode bytes (1 or 2 bytes depending on the key)
static uint8_t scancode[2];
// Flag to track if the next byte is part of a 2-byte scancode
static bool is_two_byte = false;
// Current scancode being processed (may be part of a 2-byte sequence)
uint8_t current_code = 0;

//Subscribes to keyboard interrupts.
int (kbd_subscribe_int)(uint8_t *bit_no) {
    if (!bit_no) return 1;

    *bit_no = BIT(kbd_int_id);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_int_id);
}

//Unsubscribes from keyboard interrupts.
int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbd_int_id);
}

void (kbd_ih)() {
    uint8_t byte;
    // Read scancode from keyboard controller
    if (kbc_read_output(KBC_OUT_CMD, &byte, 0) != 0)
      return;
    
      // Check for 2-byte scancode prefix (0xE0)
    if (byte == TWO_BYTES) {
      is_two_byte = true;
      scancode[0] = byte;
      return; 
    }
  
    if (is_two_byte) {
      scancode[1] = byte;
      is_two_byte = false;

      // Only process make codes (key press, not release)
      if (!(byte & BREAK_CODE)) {
        update_cursor_position(scancode[1]); // Use second byte
      }

      last_keycode = byte;

    } else {
      scancode[0] = byte;

      if (!(byte & BREAK_CODE)) {
        update_cursor_position(byte);  // Single-byte key press
      }

  
      last_keycode = byte;
    }
}

//Returns the most recent keycode pressed, if any.
bool keyboard_get_scancode(uint8_t *code) {
  if (code == NULL || last_keycode == 0) return false;

  *code = last_keycode;
  return true;
}
  
