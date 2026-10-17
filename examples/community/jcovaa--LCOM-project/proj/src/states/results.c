#include "results.h"
#include "../drivers/video/graphics.h"
#include "../drivers/kbc/keyboard.h"
#include "../drivers/kbc/mouse.h"
#include "../ui_assets.h"
#include "../grid.h"
#include "util_states.h"
#include "start_menu.h"
#include "show_original.h"
#include "../levels.h"
#include <string.h>

#define OPTION_NEXT_RESTART 0
#define OPTION_QUIT 1

extern uint8_t scancode;
extern struct packet mouse_packet;
extern uint16_t mouse_x;
extern uint16_t mouse_y;

extern uint16_t x_resolution;
extern uint16_t y_resolution;

extern int current_level;

static bool user_won = false;
static int selected_option = 0;

void init_results(bool success, int level) {
  user_won = success;
  current_level = level;
  selected_option = OPTION_NEXT_RESTART;
}

void draw_results(GameState *state) {
  set_white_background();

  if (user_won) {
    const char* win_message[] = {"YOU", "WON!"};
    draw_centered_paragraph(win_message, sizeof(win_message) / sizeof(win_message[0]), 150, 10, 5);
  } else {
    const char* lose_message[] = {"YOU", "LOST!"};
    draw_centered_paragraph(lose_message, sizeof(lose_message) / sizeof(lose_message[0]), 150, 10, 5);
  }
  
  uint16_t menu_center_x = x_resolution / 2;
  uint16_t option1_y = 300;
  uint16_t option2_y = 350;
  uint16_t word_spacing = 10;
  uint16_t letter_spacing = 3;
  uint16_t box_height = 30;

  const char* next_level_text[] = {"NEXT", "LEVEL"};
  const char* restart_text[] = {"RESTART"};
  const char* quit_text[] = {"QUIT"};

  if (selected_option == OPTION_NEXT_RESTART) {
    vg_draw_rectangle(menu_center_x - 100, option1_y - 5, 190, box_height, 0xE0E0FF);
  }

  if (user_won) {
    draw_centered_paragraph(next_level_text, sizeof(next_level_text) / sizeof(next_level_text[0]), option1_y, word_spacing, letter_spacing);
  } else {
    draw_centered_paragraph(restart_text, sizeof(restart_text) / sizeof(restart_text[0]), option1_y, word_spacing, letter_spacing);
  }

  if (selected_option == OPTION_QUIT) {
    vg_draw_rectangle(menu_center_x - 50, option2_y - 5, 90, box_height, 0xE0E0FF);
  }

  draw_centered_paragraph(quit_text, sizeof(quit_text) / sizeof(quit_text[0]), option2_y, word_spacing, letter_spacing);
}

void handle_next_restart_action(GameState *state) {
  if (user_won) {
    if (current_level < 5) {
      current_level++;
      init_show_original();
      load_level(current_level);
      *state = SHOW_ORIGINAL;
    } else {
      *state = START_MENU;
      init_start_menu();
    }
  } else {
    init_show_original();
    load_level(current_level);
    *state = SHOW_ORIGINAL;
  }
}

void handle_results_input(GameState *state) {
  // Keyboard input
  if (scancode != 0) {
    switch (scancode) {
      case UP_ARROW:
        selected_option = OPTION_NEXT_RESTART;
        break;
      case DOWN_ARROW:
        selected_option = OPTION_QUIT;
        break;
      case ENTER_KEY:
        if (selected_option == OPTION_NEXT_RESTART) {
          handle_next_restart_action(state);
        }
        else if (selected_option == OPTION_QUIT) {
          *state = EXIT_GAME;
        }
        break;
      case ESC_KEY:
        *state = EXIT_GAME;
        break;
    }
    
    scancode = 0;
  }

  // Mouse input
  static int prev_lb = 0;

  uint16_t menu_center_x = x_resolution / 2;
  uint16_t option1_y = 300;
  uint16_t option2_y = 350;
  uint16_t option1_w = 190;
  uint16_t option1_h = 30;
  uint16_t option2_w = 90;
  uint16_t option2_h = 30;

  if (mouse_packet.lb && !prev_lb) {
    if (mouse_x >= menu_center_x - 100 && mouse_x <= menu_center_x - 100 + option1_w &&
        mouse_y >= option1_y - 5 && mouse_y <= option1_y - 5 + option1_h) {
      selected_option = OPTION_NEXT_RESTART;
      handle_next_restart_action(state);
    }
    else if (mouse_x >= menu_center_x - 50 && mouse_x <= menu_center_x - 50 + option2_w &&
             mouse_y >= option2_y - 5 && mouse_y <= option2_y - 5 + option2_h) {
      selected_option = OPTION_QUIT;
      *state = EXIT_GAME;
    }
  }
  prev_lb = mouse_packet.lb;
}
