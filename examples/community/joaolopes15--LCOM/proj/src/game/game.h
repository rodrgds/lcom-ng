#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "../sprite/sprite.h"
#include "../breakout/breakout.h"
#include "../menus/mainmenu.h"
#include "../menus/instructionmenu.h"
#include "../menus/pausemenu.h"
#include "../menus/gameovermenu.h"

// enum of all possible game states
typedef enum {
  STATE_MENU,       // main menu state
  STATE_PLAYING,    // playing state
  STATE_PAUSED,     // paused state
  STATE_GAME_OVER,  // game over state
  STATE_HOW_TO_PLAY, // how to play state
  STATE_EXIT        // exit state to terminate the game
} game_state_t;

// game structure that holds all game-related information
typedef struct {
  game_state_t current_state;   // current state of the game
  game_state_t previous_state;  // previous state of the game

  breakout_t* breakout;
  main_menu_t* main_menu;
  instruction_menu_t* instruction_menu;
  pause_menu_t* pause_menu;
  game_over_menu_t* game_over_menu;
  
  bool key_left_pressed;
  bool key_right_pressed;
  bool key_up_pressed;
  bool key_down_pressed;
  bool key_space_pressed;

  int mouse_x;
  int mouse_y; 
  int mouse_target_x;
  bool mouse_control_active;
  
  Sprite *cursor_sprite;
  bool cursor_visible;

} game_t;

// initialize the game structure and set initial state
game_t* game_init();

// process keyboard input according to the current game state
void game_process_input(game_t* game, uint8_t scancode);

// process mouse input according to the current game state
void game_process_mouse_input(game_t* game, struct packet* mouse_packet);

// update the game logic based on the current state
void game_update(game_t* game);

// render the game according to the current state
void game_render(game_t* game);

// change the current game state
void game_change_state(game_t* game, game_state_t new_state);

// clean up game resources before exiting
void game_exit(game_t* game);

#endif
