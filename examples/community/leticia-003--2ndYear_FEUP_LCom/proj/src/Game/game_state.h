#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
  STATE_MENU,
  STATE_PLAYING,
  STATE_EXIT,
  STATE_INSTRUCTIONS
} GameState;

void game_set_state(GameState state);
GameState game_get_state(void);

#endif
