#include "game/controller/game.h"

#include "scheduler/scheduler.h"
#include "state/state.h"
#include "utils.h"

#include "game/wordlist.h"
#include "game/layout.h"
#include "game/controller/local_player.h"

static size_t timer_task_id;

void (game_start)() {
  game_reset();
  
  game *g = game_get();
  g->running = true;
  g->target_word = game_wordlist_select_random();
  g->ellapsed_time = 0;

  printf("game_start: target word is %s\n", g->target_word);
  timer_task_id = scheduler_set_interval(1, game_handle_scheduler_tick);

  event_on(KEY_PRESS, game_handle_keypress);
}

void (game_end)() {
  event_unregister(KEY_PRESS, game_handle_keypress);
  scheduler_remove_task(timer_task_id);
}

void (game_go_back)() {
  state_set(MENU);
}

void (game_handle_scheduler_tick)() {
  game *g = game_get();
  if (!g->running) {
    return;
  }

  g->ellapsed_time++;
}

void (game_handle_keypress)(struct event *ev) {
  game *g = game_get(); 
  if (!g->running) {
    return;
  }

  char ch = *(char*) ev->data;
  if (ch == '\n') {
    local_player_submission_status status = local_player_submit_word(game_verify_word);
    
    switch (status) {
      case TOO_SHORT:
        printf("game: word is too short\n");
        break;

      case NOT_IN_WORDLIST:
        printf("game: word is not in wordlist\n");

        break;

      case SUBMITTED:
        printf("game: word was submitted\n");
        break;

      case FINISHED:
        g->running = false;
        scheduler_set_timeout(3, game_go_back);
        return;
    }
  } else if (ch == '\x1B') {
    state_set(MENU);
  }
}

bool (game_verify_word)(char *word, cell_background *line) {
  game *g = game_get();

  char *target = g->target_word;
  uint8_t unmatched['z' - 'a' + 1] = { 0 };

  size_t remaining_cols = COLS;
  for (size_t i = 0; i < COLS; i++) {

    char word_letter = word[i];
    char target_letter = target[i];

    if (word_letter != target_letter) {
      unmatched[target_letter - 'a'] += 1;
      continue;
    }

    line[i] = GREEN;
    remaining_cols--;
  }

  if (remaining_cols == 0) {
    return true;
  }

  for (size_t i = 0; i < COLS; i++) {
    if (line[i] != EMPTY) {
      continue;
    }

    char word_letter = word[i];
    uint8_t *left = unmatched + (word_letter - 'a');
    
    if (*left == 0) {
      line[i] = GRAY;
      continue;
    }

    line[i] = YELLOW;
    *left -= 1;
  }

  return false;
}
