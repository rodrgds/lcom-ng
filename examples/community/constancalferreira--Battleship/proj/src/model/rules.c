#include "rules.h"

extern uint8_t *current_buffer;
extern unsigned int frame_size;
extern uint8_t scancode;

uint8_t *rules_buffer = NULL;

void set_rules_buffer() {
    if (rules_buffer == NULL) {
        rules_buffer = (uint8_t *) malloc(frame_size);
    }
}

void init_rules() {
    set_rules_buffer();
    draw_rules(rules_buffer);
}

void rules_main_loop() {
    memcpy(current_buffer, rules_buffer, frame_size);
    draw_cursor(current_buffer); 
    swap_buffers();
}

void rules_keyboard_handler() {
  switch (scancode) {
    case Q_KEY:
      set_state(get_previous_state());
      scancode = 0;
      break;
    default:
      break;
    }
}



