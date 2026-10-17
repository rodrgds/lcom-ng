#include "game/state.h"

#include "state/state.h"

#include "game/wordlist.h"
#include "game/controller/game.h"
#include "game/controller/local_player.h"

void (game_init)(char *wordlist) {
  game_wordlist_init(wordlist);
  state_register(GAME, game_state_start, game_state_end);
}

void (game_cleanup)() {
  game_wordlist_cleanup();
}

void (game_state_start)() {
  game_start();
  local_player_start();
}

void (game_state_end)() {
  local_player_end();
  game_end();
}
