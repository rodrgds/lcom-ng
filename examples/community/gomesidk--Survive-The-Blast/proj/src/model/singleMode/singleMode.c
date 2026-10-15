#include "singleMode.h"

struct singleMode_imp {
  SpriteLoader *loader; // Pointer to the sprite loader
  SmGame *sm_game; // Pointer to the game state
  SmDied *sm_died; // Pointer to the died state
  Sm_State current_state; // Current state of the single mode
  Score* score; // Pointer to the score object, it is in this class because it is used in both game and died states
};

SingleMode *create_singleMode(SpriteLoader *loader) {
  SingleMode *sm = (SingleMode *) malloc(sizeof(SingleMode));
  if (sm == NULL) {
    return NULL;
  }
  
  sm->score = create_score(loader);
  sm->sm_game = create_sm_game(loader, sm->score);
  if (sm->sm_game == NULL) {
    free(sm);
    return NULL; 
  }
  sm->sm_died = NULL;
  sm->current_state = GAME; 
  sm->loader = loader;
  return sm;
}

void destroy_singleMode(SingleMode *sm) {
  if (sm == NULL) {
    return;
  }
  if (sm->sm_game != NULL) {
    destroy_sm_game(sm->sm_game);
  }
  if (sm->sm_died != NULL) {
    destroy_sm_died(sm->sm_died);
  }
  if (sm->score != NULL) {
    destroy_score(sm->score);
  }
  free(sm);
}

void draw_singleMode(SingleMode *sm) {
  if (sm == NULL) {
    return;
  }
  
  switch (sm->current_state) {
    case GAME:
      draw_sm_game(sm->sm_game);
      break;

    case DIED:
      draw_sm_died(sm->sm_died);
      break;

    default:
      break;
  }
}

// 0 Continue game
// 1 Go back to menu
int process_single_mode_kbd(SingleMode *sm, KeyPressed *key) {
  if (sm == NULL || key == NULL) {
    return 1; 
  }

  switch (sm->current_state) {
    case GAME:
      if (process_sm_game_kbd(sm->sm_game, key) == 1) {
        return 1; 
      }
      break;

    case DIED:
      if (process_sm_died_kbd(sm->sm_died, key) == 1) {
        return 1; 
      }
      break;

    default:
      break;
  }
  return 0; 
}

// 0 Continue game
// 1 Go back to menu
int process_single_mode_mouse(SingleMode *sm, Cursor *c){
  if (sm == NULL || c == NULL) {
    return 1; 
  }

  int ret;
  switch (sm->current_state) {
    case GAME:
      if (process_sm_game_mouse(sm->sm_game, c) == 1) {
        destroy_sm_game(sm->sm_game);
        sm->sm_game = NULL;
        return 1; 
      }
      break;

    case DIED:
      ret = process_sm_died_mouse(sm->sm_died, c);
      if (ret == 1) {
        return 1; // Go back to menu
      } else if (ret == 2) {
        sm->current_state = GAME; 
        destroy_sm_died(sm->sm_died); 
        destroy_score(sm->score);
        sm->score = create_score(sm->loader);
        sm->sm_game = create_sm_game(sm->loader, sm->score);
        if (sm->sm_game == NULL) {
          return 1; // Error creating game state
        }
      }
      break;

    default:
      break;
  }
  
  return 0; 
}

// 0 Continue game
// 1 Go back to menu
int process_single_mode_timer(SingleMode *sm) {
  if (sm == NULL) {
    return 1; // Go back to menu
  }

  int ret;
  switch (sm->current_state) {
    case GAME:
      ret = process_sm_game_timer(sm->sm_game);
      if (ret == 1) {
        return 1; 
      } else if (ret == 2) {
        sm->current_state = DIED; 
        sm->sm_died = create_sm_died(sm->loader, sm->score);
        destroy_sm_game(sm->sm_game); 
        sm->sm_game = NULL; 
      }
      break;
    default:
      break;
  }
  
  return 0; 
}



