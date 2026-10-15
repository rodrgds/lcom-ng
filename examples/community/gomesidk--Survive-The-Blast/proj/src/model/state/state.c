#include <lcom/lcf.h>
#include "state.h"

struct state_imp {
  SingleMode *sm; 
  MultiMode *mm; 
  Menu *m; 
  Cursor *c;
  SpriteLoader *loader; 
  GameState current_state; 
};

State *create_state(SpriteLoader *loader, Cursor *c) {
  State *state = (State *) malloc(sizeof(State));
  if (state == NULL) {
    return NULL;
  }

  state->loader = loader;
  state->sm = NULL;
  state->m = create_menu(loader);
  state->mm = NULL;
  state->c = c;
  state->current_state = MENU;
  return state;
}

void destroy_state(State *state) {
  if (state == NULL) {
    return;
  }

  if (state->sm != NULL) {
    destroy_singleMode(state->sm);
  }
  if (state->m != NULL) {
    destroy_menu(state->m);
  }
  if (state->mm != NULL) {
    destroy_multiMode(state->mm);
  }

  free(state);
}

void update_state(State *state, KeyPressed *key, Cursor *c, uint8_t sp_byte) {
  if (state == NULL) {
    panic("State is NULL");
  }

  update_state_sp(state, sp_byte);
  update_state_kbd(state, key);
  update_state_mouse(state, c);
  update_state_timer(state);
}

void update_state_sp(State *state, uint8_t sp_byte) {
  if (state == NULL) {
    panic("State is NULL");
  }

  switch (state->current_state) {
    case MULTI_MODE:
      if (process_multi_mode_sp(state->mm, sp_byte) == 1) {
        state->current_state = MENU;
        destroy_multiMode(state->mm);
        state->mm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(state->c);
      }
      break;

    default:
      break;
  }
}

void update_state_kbd(State *state, KeyPressed *key) {
  if (state == NULL) {
    panic("State is NULL");
  }

  switch (state->current_state) {
    case SINGLE_MODE:
      if (process_single_mode_kbd(state->sm, key) == 1) {
        state->current_state = MENU; // Go back to menu
        destroy_singleMode(state->sm);
        state->sm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(state->c);
      }
      break;
    case MULTI_MODE:
      if (process_multi_mode_kbd(state->mm, key) == 1) {
        state->current_state = MENU;
        destroy_multiMode(state->mm);
        state->mm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(state->c);
      }
      break;  
    default:
      break;
  }
}

void update_state_mouse(State *state, Cursor *c) {
  if (state == NULL || c == NULL) {
    panic("State or Cursor is NULL");
  }

  int ret;
  switch (state->current_state) {
    case SINGLE_MODE:
      if (process_single_mode_mouse(state->sm, c) == 1) {
        state->current_state = MENU; // Go back to menu
        destroy_singleMode(state->sm);
        state->sm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(c);
      }
      break;

    case MULTI_MODE:
      if (process_multi_mode_mouse(state->mm, c) == 1) {
        state->current_state = MENU; // Go back to menu
        destroy_multiMode(state->mm);
        state->mm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(c);
      } 
      break;

    case MENU:
      ret = process_menu_mouse(state->m, c);
      if (ret == 1) {
        state->current_state = EXIT; // Exit game
        destroy_menu(state->m);
        state->m = NULL;
      } else if (ret == 2) {
        state->current_state = SINGLE_MODE; // Go to single mode
        destroy_menu(state->m);
        state->m = NULL;
        state->sm = create_singleMode(state->loader);
        reset_cursor_button_pressed(c);
      } else if (ret == 3) {
        state->current_state = MULTI_MODE; // Go to multi mode
        destroy_menu(state->m);
        state->m = NULL;
        state->mm = create_multiMode(state->loader);
        reset_cursor_button_pressed(c);
      }
      break;
    default:
      break;
  }
}

void update_state_timer(State *state) {
  if (state == NULL) {
    panic("State is NULL");
  }

  switch (state->current_state) {
    case SINGLE_MODE:
      if (process_single_mode_timer(state->sm) == 1) {
        state->current_state = MENU; // Go back to menu
        destroy_singleMode(state->sm);
        state->sm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(state->c);
      }
      break;

    case MULTI_MODE:
      if (process_multi_mode_timer(state->mm) == 1) {
        state->current_state = MENU; // Go back to menu
        destroy_multiMode(state->mm);
        state->mm = NULL;
        state->m = create_menu(state->loader);
        reset_cursor_button_pressed(state->c);
      }
      break;
    default:
      break;
  }
}

void draw_state(State *state) {
  if (state == NULL) {
    panic("State is NULL");
  }

  switch (state->current_state) {
    case SINGLE_MODE:
      draw_singleMode(state->sm);
      break;

    case MULTI_MODE:
      draw_multiMode(state->mm);
      break;
      
    case MENU:
      draw_menu(state->m);
      break;

    default:
      break;
  }

  draw_cursor(state->c);
}

int is_game_over(State *state) {
  if (state == NULL) {
    panic("State is NULL");
  }

  return (state->current_state == EXIT) ? 1 : 0;
}
