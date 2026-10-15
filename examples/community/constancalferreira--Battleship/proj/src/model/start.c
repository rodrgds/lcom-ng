#include "start.h"

extern uint8_t *current_buffer;
uint8_t *start_buffer;
extern unsigned int frame_size;


extern uint8_t scancode;

static StartOption selected_option = GO_START;


void set_start_buffer() {
  if (start_buffer == NULL) {
    start_buffer = (uint8_t *) malloc(frame_size);
  }
}

void init_start() {
  set_start_buffer();
  draw_start_background(start_buffer);
}


void start_main_loop() {
  draw_start();
  draw_cursor(current_buffer); 
  swap_buffers();
}


void start_keyboard_handler() {
  if (scancode == UP_ARROW_KEY) {
      if (selected_option == GO_START)
          selected_option = GO_HELP;
      else
          selected_option = (StartOption)(selected_option - 1);
  }
  else if (scancode == DOWN_ARROW_KEY) {
      if (selected_option == GO_HELP)
          selected_option = GO_START;
      else
          selected_option = (StartOption)(selected_option + 1);
  } 
  else if (scancode == ENTER_KEY) {
      if (selected_option == GO_START) {
          set_state(MODE);
      }
      else if (selected_option == GO_RULES) {
          set_state(RULES);
      }
      else if (selected_option == GO_HELP) {
          set_state(HELP);
      }
  }
}


bool is_mouse_over_start_option(StartOption option) {
  switch(option) {
    case GO_START:
      return (cursor_x >= OPTIONS_X &&
              cursor_x <= OPTIONS_X + start_option->width &&
              cursor_y >= OPTION_START_Y &&
              cursor_y <= OPTION_START_Y + start_option->height);
    case GO_RULES:
      return (cursor_x >= OPTIONS_X &&
              cursor_x <= OPTIONS_X + rules_option->width &&
              cursor_y >= OPTION_RULES_Y &&
              cursor_y <= OPTION_RULES_Y + rules_option->height);
    case GO_HELP:
      return (cursor_x >= OPTIONS_X &&
              cursor_x <= OPTIONS_X + help_option->width &&
              cursor_y >= OPTION_HELP_Y &&
              cursor_y <= OPTION_HELP_Y + help_option->height);
    default:
      return false;
  }
}


void start_mouse_handler() {
  if (is_mouse_over_start_option(GO_START)) {
    selected_option = GO_START;
    if (mouse_packet.lb) {
      set_state(MODE);
    }
  }
  else if (is_mouse_over_start_option(GO_RULES)) {
    selected_option = GO_RULES;
    if (mouse_packet.lb) {
      set_state(RULES);
    }
  }
  else if (is_mouse_over_start_option(GO_HELP)) {
    selected_option = GO_HELP;
    if (mouse_packet.lb) {
      set_state(HELP);
    }
  }
}


StartOption get_start_option() {
  return selected_option;
}

void set_start_option(StartOption option) {
  selected_option = option;
}
