#include "../include/event_handler.h"

void kbd_handler(uint8_t* scancode) {
  action_t action;
  switch (*scancode) {
    case AU_PRESSED: action = UP; break;
    case AD_PRESSED: action = DOWN; break;
    case AL_PRESSED: action = LEFT; break;
    case AR_PRESSED: action = RIGHT; break;
    case ENTER_PRESSED: action = SELECT; break;
    case ESC_PRESSED: action = EXIT; break;
    default: action = NOTHING;
  }
  if (action != NOTHING)
    state_set_action(action);
}

void timer_handler(){
}

void mouse_handler(struct packet *mouse_packet) {

  state_set_packet(mouse_packet);

}
