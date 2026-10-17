#include "menu/controller/menu.h"

#include "state/state.h"

#include "menu/layout.h"
#include "menu/model/menu.h"

void (menu_start)() {
  menu_reset();
  menu_populate();


  event_on(MOUSE_CLICK, menu_handle_mouse_click);
  event_on(KEY_PRESS, menu_handle_key_press);
}

void (menu_end)() {
  event_unregister(KEY_PRESS, menu_handle_key_press);
  event_unregister(MOUSE_CLICK, menu_handle_mouse_click);
}

void (menu_populate)() {
  menu_option start_game = {
    MENU_OPTION_START_GAME_X,
    MENU_OPTION_START_GAME_Y,
    MENU_OPTION_START_GAME_WIDTH,
    MENU_OPTION_START_GAME_HEIGHT,
    MENU_OPTION_START_GAME_DESCRIPTION,
    menu_do_start_game
  };

  menu_option exit_game = {
    MENU_OPTION_END_GAME_X,
    MENU_OPTION_END_GAME_Y,
    MENU_OPTION_END_GAME_WIDTH,
    MENU_OPTION_END_GAME_HEIGHT,
    MENU_OPTION_END_GAME_DESCRIPTION,
    menu_do_exit_game
  };

  // menu_option scoreboard = {
  //   MENU_OPTION_SCOREBOARD_X,
  //   MENU_OPTION_SCOREBOARD_Y,
  //   MENU_OPTION_SCOREBOARD_WIDTH,
  //   MENU_OPTION_SCOREBOARD_HEIGHT,
  //   MENU_OPTION_SCOREBOARD_DESCRIPTION,
  //   menu_do_scoreboard
  // };

  // menu_option multiplayer = {
  //   MENU_OPTION_MULTIPLAYER_X,
  //   MENU_OPTION_MULTIPLAYER_Y,
  //   MENU_OPTION_MULTIPLAYER_WIDTH,
  //   MENU_OPTION_MULTIPLAYER_HEIGHT,
  //   MENU_OPTION_MULTIPLAYER_DESCRIPTION,
  //   menu_do_multiplayer
  // };

  menu *m = menu_get();

  m->options[0] = start_game;
  m->options[1] = exit_game;
  // m->options[2] = scoreboard;
  // m->options[3] = multiplayer;

  m->n = 2;
}

void (menu_handle_mouse_click)(struct event *ev) {
  uint16_t *pos = ev->data;

  uint16_t x = pos[0];
  uint16_t y = pos[1];

  menu *m = menu_get();
  for (size_t i = 0; i < m->n; i++) {
    menu_option *opt = m->options + i;


    if (x >= opt->x && x < opt->x + opt->width && y >= opt->y && y < opt->y + opt->height) {
      opt->action();
    }
  }
}

void (menu_handle_key_press)(struct event *ev) {
  char ch = *(char*) ev->data;
  if (ch == '\x1B') {
    menu_do_exit_game();
  }
}

void (menu_do_start_game)() {
  state_set(GAME);
}

void (menu_do_exit_game)() {
  event_dispatch(EXIT, NULL, 0);
}

void (menu_do_scoreboard)() {

}

void (menu_do_multiplayer)() {
  state_set(MULTIPLAYER);
}
