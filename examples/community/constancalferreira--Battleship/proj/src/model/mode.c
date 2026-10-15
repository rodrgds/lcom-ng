#include "mode.h"


extern uint8_t *current_buffer;
uint8_t *mode_buffer; 
extern unsigned int frame_size;

extern uint8_t scancode;

GameMode game_mode;


void set_mode_buffer() {
  if (mode_buffer == NULL) {
    mode_buffer = (uint8_t *) malloc(frame_size);
  }
}

void init_mode() {
  set_mode_buffer();
  draw_mode(mode_buffer); 
}

void mode_main_loop() {
  memcpy(current_buffer, mode_buffer, frame_size);
  draw_cursor(current_buffer);                        
  swap_buffers();
}



void mode_keyboard_handler() {
  switch (scancode) {
    case ONE_KEY:
      set_state(ARENA);
      game_mode = SINGLE_PLAYER;
      break;
    case TWO_KEY:
      set_state(ARENA);  
      game_mode = MULTI_PLAYER;
      break;
    default:
      break;
  }
}









