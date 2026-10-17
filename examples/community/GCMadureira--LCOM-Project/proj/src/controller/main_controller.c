/** 
 * @file main_controller.c
 * @brief Source code to control the program
 * 
 * This file handles the logic for the whole program, controlling the menus and the arena by calling the functions in @ref arena_controller.c. It also calls the drawing functions.
 */


 
#include "main_controller.h"

/**
 * @brief Processes the inputs events for the @ref active_menu.
 * 
 * Moves the menu option and handles what happens when the user selects one of them. <br>
 * Changes the @ref game_state accordingly and sets up the new one.
 * 
 * @return 0 in case of success, 1 otherwise
 */
static int (process_menu)();

/**
 * @brief Processes the inputs events for the @ref active_arena.
 * 
 * Sets the player speed based on the inputs, handles the ranged attack and controls the mouse movement.
 * 
 * @return 0 in case of success, 1 otherwise
 */
static int (process_game)();

static menu* active_menu = NULL;               ///< The pointer to the active arena, if one exists
static arena* active_arena = NULL;             ///< The pointer to the active menu, if one exists

static enum game_state game_state = MAIN_MENU; ///< The state of the program
static unsigned long frame = 0;                ///< The frame count of the program

static uint32_t high_score = 0;                ///< Track the highest achieved time


enum game_state (get_game_state)(){
  return game_state;
}

void (set_game_state)(enum game_state state) {
  game_state = state;
}

unsigned long (get_current_frame)() {
  return frame;
}

int (load_highscore)() {
  FILE* file = fopen("highscore.txt", "r");
  if (file == NULL) return 1;

  fscanf(file, "%u", &high_score);
  fclose(file);
  
  return 0;
}

int (save_highscore)() {
  FILE* file = fopen("highscore.txt", "w");
  if (file == NULL) return 1;

  fprintf(file, "%u", high_score);
  fclose(file);

  return 0;
}

uint32_t (get_high_score)() {
  return high_score;
}

void (update_high_score)(uint32_t new_time) {
  if (new_time > high_score) {
    high_score = new_time;
  }
}

static int (process_menu)(){
  input_event event;
  while(events_get_next(&event) == 0) {
    if(event.event_type == KEYBOARD_EVENT) {
      switch (event.scancode_byte1) {
        case KEY_MK_W:
          menu_select_option_up(active_menu);
          break;
        case KEY_MK_S:
          menu_select_option_down(active_menu);
          break;
        case KEY_MK_ENTER: {
          // main menu processing
          if(game_state == MAIN_MENU) {
            if (active_menu->menu_status == 0) {
              game_state = GAME;
              active_arena = arena_create();
              setup_arena_controller(); // setup the controllers for a new game
              setup_enemy_controller();
              menu_destroy(active_menu);
              active_menu = NULL;
              return 0;
            }
            else if (active_menu->menu_status == 1) game_state = QUIT;
          }
          else if(game_state == GAME_OVER_MENU) {
            if (active_menu->menu_status == 0) {
              game_state = GAME;
              if(active_arena != NULL) arena_destroy(active_arena);
              active_arena = arena_create();
              setup_arena_controller(); // setup the controllers for a new game
              setup_enemy_controller();
              menu_destroy(active_menu);
              active_menu = NULL;
              return 0;
            }
            else if (active_menu->menu_status == 1) {
              game_state = MAIN_MENU;
              if(active_arena != NULL) arena_destroy(active_arena);
              active_arena = NULL;
              menu_destroy(active_menu);
              active_menu = menu_create_main();
              return 0;
            }
          }
          break;
        }
      }
    }
  }

  return 0;
}


static int (process_game)(){
  entity* player = active_arena->player;
  entity* mouse = active_arena->mouse;
  input_event event;

  while(events_get_next(&event) == 0) {
    if(event.event_type == KEYBOARD_EVENT) {
      switch (event.scancode_byte1) {
        case KEY_MK_W: //moving up
          player->speed_y = -3;
          break;
        case KEY_MK_S: //moving down
          player->speed_y = 3;
          break;
        case KEY_MK_D: //moving right
          player->last_horizontal_direction = true;
          player->speed_x = 3;
          break;
        case KEY_MK_A: //moving left
          player->last_horizontal_direction = false;
          player->speed_x = -3;
          break;
        case KEY_BK_W:
          if(player->speed_y < 0) player->speed_y = 0;
          break;
        case KEY_BK_S:
          if(player->speed_y > 0) player->speed_y = 0;
          break;
        case KEY_BK_D:
          if(player->speed_x > 0) player->speed_x = 0;
          break;
        case KEY_BK_A:
          if(player->speed_x < 0) player->speed_x = 0;
          break;
      }
    }
    else if(event.event_type == MOUSE_EVENT) {
      mouse->pos_x += (double)event.mouse_packet.delta_x/2;
      mouse->pos_y += -(double)event.mouse_packet.delta_y/2;

      // bound the position inside the screen
      mouse->pos_x = MIN(MAX(0, mouse->pos_x), vg_get_hres() - cursor_img.width);
      mouse->pos_y = MIN(MAX(0, mouse->pos_y), vg_get_vres() - cursor_img.height);

      if(event.mouse_packet.lb)
        handle_ranged_attack(active_arena);
    }
  }
  
  return 0;
}

int (process_frame)() {
  // The game state can change on the process part, need to separate it
  if(game_state == MAIN_MENU || game_state == GAME_OVER_MENU) process_menu();
  else if(game_state == GAME) process_game();
  

  if(game_state == MAIN_MENU) draw_menu(active_menu);
  else if(game_state == GAME) {
    // If arena_process_frame == 1 it means check_collisions == 1
    if(arena_process_frame(active_arena) == 1) {
      // If player Health == 0 return to menu
      // We are doing the entire "changing health bar sprite" on the viewers
      if (active_arena->player->health == 0) {
        update_high_score(get_arena_game_time());
        game_state = GAME_OVER_MENU;
        active_menu = menu_create_game_over();
      }
    }
    else draw_arena(active_arena);
  }
  else if(game_state == GAME_OVER_MENU) {
    draw_arena(active_arena);
    draw_menu(active_menu);
  }

  vg_show_frame();
  ++frame;

  return 0;
}

int (setup_game)() {
  if(static_images_load()) return 1;
  if(animations_load()) return 1;
  load_highscore();
  active_menu = menu_create_main();
  game_state = MAIN_MENU;
  return 0;
}

int (clean_game)() {
  save_highscore();
  events_clear();
  animations_clean();
  if(active_menu != NULL) menu_destroy(active_menu);
  if(active_arena != NULL) arena_destroy(active_arena);
  return 0;
}

