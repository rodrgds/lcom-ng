#include <lcom/lcf.h>

#include "dispatcher.h"
#include "elements.h"
#include "game_logic.h"
#include "menus.h"

state_t st = MAIN_MENU; // game state

state_t get_state() {
  return st;
}

void game_state(event_t evt) {

  while (st != LEAVE) {
    switch (st) {
      case MAIN_MENU:
        if (evt == INIT) {
          create_mouse();
          if (draw_main_menu())
            return;
          evt = menus(MAIN_MENU);
        }
        else if (evt == START_GAME) {
          st = ONGOING;
          destroy_mouse();
          evt = INIT;
        }
        else if (evt == INSTRUCTIONS) {
          st = INSTRUCTIONS_MENU;
          destroy_mouse();
          evt = INIT;
        }
        else if (evt == EXIT) {
          st = LEAVE;
          destroy_mouse();
        }
        break;

      case INSTRUCTIONS_MENU:
        if (evt == INIT) {
          create_mouse();
          if (draw_instructions())
            return;
          evt = menus(INSTRUCTIONS_MENU);
        }
        else if (evt == GO_BACK_TO_MAIN_MENU) {
          st = MAIN_MENU;
          destroy_mouse();
          evt = INIT;
        }
        break;

      case ONGOING:
        if (evt == INIT) {
          create_elements();
          if (draw_static_elements())
            return;
          play();
          st = FINISHED;
        }
        break;

      case FINISHED:
        destroy_elements();
        st = LEAVE;
        break;

      default:
        break;
    }
  }
}
