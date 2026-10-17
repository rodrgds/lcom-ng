/**
 * @file event.c
 * @brief Implementation of input event handling for keyboard and mouse.
 * 
 * This file contains the implementation of the functions declared in event.h.
 */ 

#include "event.h"

extern int reload_timer_ticks;
extern state_t state;
extern int enemies;

int handle_event_game(event_t *e, game_t *game, sprite_t *cursor) {
  // Write logic to handle event
  if (e->type == EVENT_KEYBOARD) {
    handle_keyboard_event_game(e, get_game_player(game), get_game_wall_arr(game), get_game_enemies(game));
  }
  else if (e->type == EVENT_MOUSE) {
    handle_mouse_event_game(e, get_game_player(game), cursor, get_game_bullets(game));
  }
  return 0;
}

int handle_event_menu(event_t *e, menu_t *menu, sprite_t *cursor) {
  // Write logic to handle event
  if (e->type == EVENT_KEYBOARD) {
    handle_keyboard_event_menu(e, menu);
  }
  else if (e->type == EVENT_MOUSE) {
    handle_mouse_event_menu(e, menu, cursor);
  }
  return 0;
}

int handle_keyboard_event_game(event_t *e, player_t *player, wall_array_t wall_arr, enemy_t **enemies_arr) {
  uint8_t final_byte = e->k->size == 2 ? e->k->bytes[1] : e->k->bytes[0];
  if (final_byte == ESC_KEY) {
    state = GO_TO_PAUSE_MENU;
    return 0;
  }

  uint8_t key_code_for_state = final_byte & (~BREAK_CODE_BIT);

  int16_t dx = 0;
  int16_t dy = 0;

  switch (key_code_for_state) {
    case KEY_W:
      dy -= PLAYER_SPEED;
      break;
    case KEY_A:
      dx -= PLAYER_SPEED;
      break;
    case KEY_S:
      dy += PLAYER_SPEED;
      break;
    case KEY_D:
      dx += PLAYER_SPEED;
      break;
    default:
      break;
  }

  if (dx != 0 || dy != 0) {
    int prev_x = get_player_pos(player)->x;
    int prev_y = get_player_pos(player)->y;

    player_move(player, dx, dy);
    player_check_bounds(player, get_hres(), get_vres());
    int dx = 0, dy = 0;
    if (collides_with_any_wall(get_player_sprite(player), &wall_arr, &dx, &dy)) {
      set_player_pos(player, prev_x, prev_y);
    }
    for (int i = 0; i < enemies; ++i) {
      if (enemies_arr[i] && enemies_arr[i]->active) {
        if (check_collision(get_player_sprite(player), get_enemy_sprite(enemies_arr[i]))) {
          set_player_pos(player, prev_x, prev_y);
          break;
        }
      }
    }
  }
  else { // No Move/Invalid Press
    set_player_idle(player);
  }
  return 0;
}

int handle_mouse_event_game(event_t *e, player_t *player, sprite_t *cursor, bullet_t *bullets[]) {
  struct packet *pack = e->pack;
  bool lb_current = pack->lb;
  bool rb_current = pack->rb;

  bool mouse_left_button_previously_pressed = (e->prev_mouse_btn == 2);
  bool mouse_right_button_previously_pressed = (e->prev_mouse_btn == 1);

  // Player
  bool is_reloading = get_player_is_reloading(player);
  unsigned int current_ammo_in_magazine = get_player_current_ammo_in_magazine(player);
  position_t *player_pos = get_player_pos(player);

  // Cursor
  position_t *cursor_pos = get_sprite_pos(cursor);
  uint16_t c_width = get_sprite_width(cursor);
  uint16_t c_height = get_sprite_height(cursor);

  if (lb_current && !mouse_left_button_previously_pressed) { // BULLET FIRING LOGIC (LEFT CLICK)
    if (current_ammo_in_magazine > 0 && !is_reloading) {
      for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i] == NULL) { // Find an empty slot for a new bullet
          int16_t bullet_start_x = player_pos->x;
          int16_t bullet_start_y = player_pos->y;

          int16_t bullet_target_x = cursor_pos->x + c_width / 2;
          int16_t bullet_target_y = cursor_pos->y + c_height / 2;

          bullets[i] = bullet_init(bullet_start_x, bullet_start_y, bullet_target_x, bullet_target_y, true);
          set_player_current_ammo_in_magazine(player, current_ammo_in_magazine - 1);

          set_player_act(player, A_SHOOT);

          if (cursor_pos->x + c_width / 2 < player_pos->x) {
            set_player_flip_on_shoot(player, true);
          }
          else {
            set_player_flip_on_shoot(player, false);
          }
          break;
        }
      }
    } // --- END BULLET FIRING LOGIC ---
  }
  else if (rb_current && !mouse_right_button_previously_pressed) { // RELOAD LOGIC (RIGHT CLICK)
    if (!is_reloading && current_ammo_in_magazine < MAX_AMMO_IN_MAGAZINE) {
      set_player_is_reloading(player, true);
      reload_timer_ticks = 0;
    }
  }
  else { // Invalid Press
    set_player_idle(player);
  }
  // --- END RELOAD LOGIC ---

  return 0;
}

int handle_keyboard_event_menu(event_t *e, menu_t *menu) {
  unsigned int opt = get_menu_opt(menu);

  if (e->k->size == 2 && e->k->bytes[0] == FIRST_BYTE_SCANCODE) { // Handle extended keys
    uint8_t key_code = e->k->bytes[1];

    switch (key_code) {
      case ARROW_UP:
        if (opt > 0) {
          opt--;
        }
        else {
          opt = get_menu_size(menu) - 1;
        }
        break;
      case ARROW_DOWN:
        if (opt < get_menu_size(menu) - 1) {
          opt++;
        }
        else {
          opt = 0;
        }
        break;
      default:
        break;
    }
  }
  else if (e->k->size == 1) { // Regular keys
    uint8_t key_code = e->k->bytes[0];

    switch (key_code) {
      case KEY_W:
        if (opt > 0) {
          opt--;
        }
        else {
          opt = get_menu_size(menu) - 1;
        }
        break;
      case KEY_S:
        if (opt < get_menu_size(menu) - 1) {
          opt++;
        }
        else {
          opt = 0;
        }
        break;
      case KEY_ENTER:
        get_menu_callback(menu, opt)();
        break;
      default:
        break;
    }
  }

  set_menu_opt(menu, opt);
  return 0;
}

int handle_mouse_event_menu(event_t *e, menu_t *menu, sprite_t *cursor) {
  if (!menu || !cursor) {
    return 1;
  }

  position_t *cursor_pos = get_sprite_pos(cursor);
  uint16_t cursor_x = cursor_pos->x;
  uint16_t cursor_y = cursor_pos->y;

  sprite_t *option_bg_sprite = get_menu_option(menu);
  int item_clickable_width = get_sprite_width(option_bg_sprite);
  int item_clickable_height = get_sprite_height(option_bg_sprite);

  int options_common_x_start = get_menu_x_pos(menu);
  uint16_t title_screen_y = get_menu_title_y(menu);
  uint16_t vertical_pitch = get_menu_opt_vertical_offset(menu);

  bool lb_is_currently_pressed = e->pack->lb;
  bool lb_was_previously_pressed = (e->prev_mouse_btn == 2);
  bool is_new_left_click = lb_is_currently_pressed && !lb_was_previously_pressed;

  for (unsigned int i = 0; i < get_menu_size(menu); ++i) {
    int current_option_x_start = options_common_x_start;
    int current_option_y_start = title_screen_y + (i + 1) * vertical_pitch;

    bool cursor_is_over_option =
      (cursor_x >= current_option_x_start &&
       cursor_x < current_option_x_start + item_clickable_width &&
       cursor_y >= current_option_y_start &&
       cursor_y < current_option_y_start + item_clickable_height);

    if (cursor_is_over_option) {

      set_menu_opt(menu, i);

      if (is_new_left_click) {

        void (*callback_func)(void) = get_menu_callback(menu, i);
        if (callback_func) {
          callback_func();
        }
        return 0;
      }
    }
  }

  return 0;
}

int handle_event_lb(event_t *e, lb_page_t *lb_page, sprite_t *cursor) {
  // Write logic to handle event
  if (e->type == EVENT_KEYBOARD) {
    handle_keyboard_event_lb(e, lb_page);
  }
  else if (e->type == EVENT_MOUSE) {
    handle_mouse_event_lb(e, lb_page, cursor);
  }
  return 0;
}

int handle_keyboard_event_lb(event_t *e, lb_page_t *lb_page) {
  uint8_t key_code = e->k->bytes[0];

  if (key_code == KEY_ENTER) {
    get_lb_func(lb_page)();
  }
  return 0;
}

int handle_mouse_event_lb(event_t *e, lb_page_t *lb_page, sprite_t *cursor) {
  if (!lb_page || !cursor) {
    return 1;
  }

  sprite_t *option = get_lb_option(lb_page);

  position_t *cursor_pos = get_sprite_pos(cursor);
  uint16_t cursor_x = cursor_pos->x;
  uint16_t cursor_y = cursor_pos->y;

  position_t *option_pos = get_sprite_pos(option);
  int option_x = option_pos->x;
  int option_y = option_pos->y;

  int option_width = get_sprite_width(option);
  int option_height = get_sprite_height(option);

  bool lb_is_currently_pressed = e->pack->lb;
  bool lb_was_previously_pressed = (e->prev_mouse_btn == 2);
  bool is_new_left_click = lb_is_currently_pressed && !lb_was_previously_pressed;

  bool cursor_is_over_option =
    (cursor_x >= option_x &&
     cursor_x < option_x + option_width &&
     cursor_y >= option_y &&
     cursor_y < option_y + option_height);

  if (cursor_is_over_option) {
    if (is_new_left_click) {
      get_lb_func(lb_page)();
    }
  }
  return 0;
}

int handle_event_page(event_t *e, write_name_t *page, sprite_t *cursor) {
  // Write logic to handle event
  if (e->type == EVENT_KEYBOARD) {
    handle_keyboard_event_page(e, page, cursor);
  }
  else if (e->type == EVENT_MOUSE) {
    handle_mouse_event_page(e, page, cursor);
  }
  return 0;
}

int handle_keyboard_event_page(event_t *e, write_name_t *page, sprite_t *cursor) {
  bool writing = is_writing(page);
  unsigned int name_len = get_name_lenght(page);
  char *name = get_player_name(page);

  uint16_t char_width = get_sprite_width(get_text_sprite(get_page_text(page)));
  position_t *cursor_pos = get_sprite_pos(cursor);

  if (writing) {
    if (e->k->size == 1) { // Regular keys
      uint8_t key_code = e->k->bytes[0];

      if (name_len == MAX_NAME_CHARS) {
        name[name_len] = '\0';
        name_len++;
        set_name_lenght(page, name_len);
        set_page_writing(page, false);
        set_sprite_curr(cursor, 0);
        set_page_opt(page, 0);
        return 0;
      }

      switch (key_code) {
        case KEY_A:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'A';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_B:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'B';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_C:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'C';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_D:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'D';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_E:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'E';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_F:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'F';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_G:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'G';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_H:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'H';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_I:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'I';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_J:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'J';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_K:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'K';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_L:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'L';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_M:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'M';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_N:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'N';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_O:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'O';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_P:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'P';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_Q:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'Q';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_R:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'R';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_S:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'S';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_T:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'T';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_U:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'U';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_V:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'V';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_W:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'W';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_X:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'X';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_Y:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'Y';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_Z:
          if (name_len < MAX_NAME_CHARS) {
            name[name_len] = 'Z';
            name_len++;
            cursor_pos->x += char_width * 1.5;
          }
          break;
        case KEY_ENTER:
          name[name_len] = '\0';
          name_len++;
          set_page_writing(page, false);
          set_sprite_curr(cursor, 0);
          set_page_opt(page, 0);
          break;
        case KEY_BACKSPACE:
          if (name_len > 0) {
            name_len--;
            name[name_len] = '\0';
            cursor_pos->x -= char_width * 1.5;
          }
        default:
          break;
      }
      set_name_lenght(page, name_len);
    }
  }
  else {
    int opt = get_page_opt(page);

    if (e->k->size == 2 && e->k->bytes[0] == FIRST_BYTE_SCANCODE) { // Handle extended keys
      uint8_t key_code = e->k->bytes[1];

      switch (key_code) {
        case ARROW_UP:
          if (opt > 0) {
            opt--;
          }
          else {
            opt = (int) (get_page_size(page) - 1);
          }
          break;
        case ARROW_DOWN:
          if (opt < (int) (get_page_size(page) - 1)) {
            opt++;
          }
          else {
            opt = 0;
          }
          break;
        default:
          break;
      }
    }
    else if (e->k->size == 1) { // Regular keys
      uint8_t key_code = e->k->bytes[0];

      switch (key_code) {
        case KEY_W:
          if (opt > 0) {
            opt--;
          }
          else {
            opt = (int) (get_page_size(page) - 1);
          }
          break;
        case KEY_S:
          if (opt < (int) (get_page_size(page) - 1)) {
            opt++;
          }
          else {
            opt = 0;
          }
          break;
        case KEY_ENTER:
          get_page_callback(page, opt)();
          break;
        default:
          break;
      }
    }

    set_page_opt(page, opt);
  }
  return 0;
}

int handle_mouse_event_page(event_t *e, write_name_t *page, sprite_t *cursor) {
  bool writing = is_writing(page);
  if (writing) {
    // DO NOTHING
  }
  else {
    position_t *cursor_pos = get_sprite_pos(cursor);
    uint16_t cursor_x = cursor_pos->x;
    uint16_t cursor_y = cursor_pos->y;

    sprite_t *option_bg_sprite = get_page_option(page);
    position_t *pos = get_sprite_pos(option_bg_sprite);
    int item_clickable_width = get_sprite_width(option_bg_sprite);
    int item_clickable_height = get_sprite_height(option_bg_sprite);

    int options_common_x_start = pos->x;
    uint16_t base_y = get_page_title_y(page);

    // Get the heights
    unsigned int text_height = get_sprite_height(get_text_sprite(get_page_text(page)));
    unsigned int option_height = get_sprite_height(option_bg_sprite);

    bool lb_is_currently_pressed = e->pack->lb;
    bool lb_was_previously_pressed = (e->prev_mouse_btn == 2);
    bool is_new_left_click = lb_is_currently_pressed && !lb_was_previously_pressed;

    for (unsigned int i = 0; i < get_page_size(page); ++i) {
      int current_option_x_start = options_common_x_start;

      int current_option_y_start = base_y + 3 * text_height + 60 + i * (option_height + 10);

      bool cursor_is_over_option =
        (cursor_x >= current_option_x_start &&
         cursor_x < current_option_x_start + item_clickable_width &&
         cursor_y >= current_option_y_start &&
         cursor_y < current_option_y_start + item_clickable_height);

      if (cursor_is_over_option) {
        set_page_opt(page, i);

        if (is_new_left_click) {
          void (*callback_func)(void) = get_page_callback(page, i);
          if (callback_func) {
            callback_func();
          }
        }
      }
    }
  }
  return 0;
}

event_t *create_empty_event() {
  event_t *event = (event_t *) calloc(1, sizeof(event_t));
  if (event == NULL) {
    return NULL;
  }

  return event;
}

event_t *new_keyboard_event(struct key *k) {
  event_t *event = (event_t *) calloc(1, sizeof(event_t));
  if (!event) {
    return NULL;
  }
  event->type = EVENT_KEYBOARD;

  event->k = (struct key *) malloc(sizeof(struct key));
  if (!event->k) {
    free(event);
    return NULL;
  }
  *(event->k) = *k;

  return event;
}

event_t *new_mouse_event(struct packet *pack, unsigned int prev) {
  event_t *event = (event_t *) calloc(1, sizeof(event_t));
  if (!event) {
    return NULL;
  }
  event->type = EVENT_MOUSE;

  event->pack = (struct packet *) malloc(sizeof(struct packet));
  if (!event->pack) {
    free(event);
    return NULL;
  }
  *(event->pack) = *pack;
  event->prev_mouse_btn = prev;

  return event;
}
