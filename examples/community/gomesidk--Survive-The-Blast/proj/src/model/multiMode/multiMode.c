#include "multiMode.h"


struct multiMode_imp {
  SpriteLoader *loader; // Pointer to the sprite loader
  MmConnection *mm_connection; // Pointer to the multiplayer connection
  MmGame *mm_game; // Pointer to the game state
  MmWinner *mm_winner; // Pointer to the winner state
  Mm_State current_state; // Current state of the single mode
};

MultiMode *create_multiMode(SpriteLoader *loader) {
  MultiMode *mm = (MultiMode *) malloc(sizeof(MultiMode));
  if (mm == NULL) {
    return NULL;
  }
  
  sp_clear_buffers(); // Clear the serial port buffer
  mm->mm_connection = create_mm_connection(loader);
  if (mm->mm_connection == NULL) {
    free(mm);
    return NULL; 
  }
  
  mm->mm_game = NULL;
  mm->mm_winner = NULL;
  mm->current_state = MM_CONNECTION; 
  mm->loader = loader;
  return mm;
}

void destroy_multiMode(MultiMode *mm) {
  if (mm == NULL) {
    return;
  }
  
  if (mm->mm_connection != NULL) {
    destroy_mm_connection(mm->mm_connection);
  }
  
  if (mm->mm_game != NULL) {
    destroy_mm_game(mm->mm_game);
  }
  
  if (mm->mm_winner != NULL) {
    destroy_mm_winner(mm->mm_winner);
  }
  free(mm);
}

void draw_multiMode(MultiMode *mm) {
  if (mm == NULL) {
    return;
  }
  
  switch (mm->current_state) {
    case MM_CONNECTION:
      draw_mm_connection(mm->mm_connection);
      break;

    case MM_GAME:
      draw_mm_game(mm->mm_game);
      break;

    case MM_WINNER:
      draw_mm_winner(mm->mm_winner);
      break;

    default:
      break;
  }
}


// 0 Continue game
// 1 Go back to menu
int process_multi_mode_kbd(MultiMode *mm, KeyPressed *key) {
  if (mm == NULL || key == NULL) {
    return 1; // Go back to menu
  }

  switch (mm->current_state) {
    case MM_GAME:
      if (process_mm_game_kbd(mm->mm_game, key) == 1) {
        destroy_mm_game(mm->mm_game);
        mm->mm_game = NULL; // Clear the game state
        return 1; // Go back to menu
      }
      break;

    case MM_WINNER:
      if (process_mm_winner_kbd(mm->mm_winner, key) == 1) {
        destroy_mm_winner(mm->mm_winner);
        mm->mm_winner = NULL; // Clear the winner state
        return 1; // Go back to menu
      }
      break;

    default:
      break;
  }
  
  return 0; // Continue game
}

// 0 Continue game
// 1 Go back to menu
int process_multi_mode_mouse(MultiMode *mm, Cursor *c) {
  if (mm == NULL || c == NULL) {
    return 1; // Go back to menu
  }

  int ret;
  switch (mm->current_state) {
    case MM_CONNECTION:
      if (process_mm_connection_mouse(mm->mm_connection, c) == 1) {
        destroy_mm_connection(mm->mm_connection);
        mm->mm_connection = NULL; // Clear the connection state
        return 1; // Go back to menu
      }
      break;

    case MM_GAME:
      if (process_mm_game_mouse(mm->mm_game, c) == 1) {
        destroy_mm_game(mm->mm_game);
        mm->mm_game = NULL; // Clear the game state
        return 1; // Go back to menu
      }
      break;

    case MM_WINNER:
      ret = process_mm_winner_mouse(mm->mm_winner, c);
      if (ret == 1) {
        destroy_mm_winner(mm->mm_winner);
        mm->mm_winner = NULL; // Clear the winner state
        return 1; // Go back to menu
      } else if (ret == 2) {
        destroy_mm_winner(mm->mm_winner);
        mm->mm_winner = NULL; // Clear the winner state
        mm->current_state = MM_CONNECTION; // Go back to game state
        mm->mm_connection = create_mm_connection(mm->loader);
        if (mm->mm_connection == NULL) {
          return 1; // Go back to menu if connection creation failed
        }
      }
      break;

    default:
      break;
  }

  return 0; // Continue game
}

// 0 Continue game
// 1 Go back to menu
int process_multi_mode_timer(MultiMode *mm) {
  if (mm == NULL) {
    return 1; // Go back to menu
  }

  int ret;
  switch (mm->current_state) {

    case MM_CONNECTION:
      if (process_mm_connection_timer(mm->mm_connection) == 1) {
        destroy_mm_connection(mm->mm_connection);
        mm->mm_connection = NULL; // Clear the connection state
        return 1; // Go back to menu
      }
      break;

    case MM_GAME:
      ret = process_mm_game_timer(mm->mm_game);
      if (ret == 2) {
        mm->current_state = MM_WINNER; // Transition to winner state
        mm->mm_winner = create_mm_winner(mm->loader, get_winner(mm->mm_game));
        if (mm->mm_winner == NULL) {
          return 1; // Go back to menu if winner creation failed
        }
        destroy_mm_game(mm->mm_game);
        mm->mm_game = NULL; // Clear the game state
      }else if(ret == 1) {
        destroy_mm_game(mm->mm_game);
        mm->mm_game = NULL; // Clear the game state
        return 1; // Go back to menu
      }
      break;

    default:
      break;
  }

  return 0; // Continue game
}

// 0 Continue game
// 1 Go back to menu
int process_multi_mode_sp(MultiMode *mm, uint8_t byte){
  if (mm == NULL) {
    return 1; // Go back to menu
  }

  int ret;
  switch (mm->current_state) {
    case MM_CONNECTION:
      if (mm->mm_connection == NULL) break;
      ret = process_mm_connection_sp(mm->mm_connection, byte);
      if (ret == -1) {
        destroy_mm_connection(mm->mm_connection);
        mm->mm_connection = NULL;
        return 1; // Go back to menu
      }else if (ret == 1) {
        mm->current_state = MM_GAME; 
        mm->mm_game = create_mm_game(mm->loader, 1);
        if (mm->mm_game == NULL) {
          return 1; // Go back to menu if game creation failed
        }
        destroy_mm_connection(mm->mm_connection);
        mm->mm_connection = NULL; 
      }else if (ret == 2) {
        mm->current_state = MM_GAME;
        mm->mm_game = create_mm_game(mm->loader, 2);
        if (mm->mm_game == NULL) {
          return 1; // Go back to menu if game creation failed
        }
        destroy_mm_connection(mm->mm_connection);
        mm->mm_connection = NULL; 
      }
      break;

    case MM_GAME:
      if (mm->mm_game == NULL) break;
      if (process_mm_game_sp(mm->mm_game, byte) == 1) {
        destroy_mm_game(mm->mm_game);
        mm->mm_game = NULL;
        mm->current_state = MM_CONNECTION; // Go back to connection state
        mm->mm_connection = create_mm_connection(mm->loader);
        if (mm->mm_connection == NULL) {
          return 1; // Go back to menu if connection creation failed
        }
      }
      break;

    default:
      break;
  }

  return 0; // Continue game
}
