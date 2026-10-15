#include "menu.h"

extern uint8_t *current_buffer;
uint8_t *menu_buffer;
extern unsigned int frame_size;

extern uint8_t scancode;

extern uint8_t scancode;


void set_menu_buffer() {
  if (menu_buffer == NULL) {
    menu_buffer = (uint8_t *) malloc(frame_size);
  }
}

void init_menu() {
  set_menu_buffer();
  draw_menu_background(menu_buffer);
}


void menu_main_loop() {
  draw_menu();
  draw_cursor(current_buffer); 
  swap_buffers();
}


void menu_keyboard_handler() {
  switch(scancode) {
    case ENTER_KEY:
      set_state(START);
      scancode = 0;
      break;
    default:
      break;  
  }
}


