#include "start_menu.h"
#include "../game.h"
#include "../drivers/video/graphics.h"
#include "../drivers/kbc/keyboard.h"
#include "../drivers/kbc/mouse.h"
#include "../ui_assets.h"
#include "../sprite.h"
#include "util_states.h"

#define MENU_OPTION_START 0
#define MENU_OPTION_QUIT 1

static int selected_option = MENU_OPTION_START;
extern uint8_t scancode;
extern struct packet mouse_packet;
extern uint16_t mouse_x;
extern uint16_t mouse_y;

extern uint16_t x_resolution;
extern uint16_t y_resolution;

void init_start_menu() {
  selected_option = MENU_OPTION_START;
}

void draw_start_menu() {
  set_white_background();

  uint16_t tx = (x_resolution - 550) / 2;
  uint16_t ty = 10;
  draw_sprite(gameTitle, tx, ty);

  uint16_t menu_x = x_resolution / 2 - 50;
  uint16_t start_y = 300;
  uint16_t quit_y = 350;
  uint16_t spacing = 3;

  const char* start_text = "START";
  const char* quit_text = "QUIT";

  if (selected_option == MENU_OPTION_START) {
    vg_draw_rectangle(menu_x - 10, start_y - 5, 100, 30, 0xE0E0FF);
  }
  draw_centered_paragraph(&start_text, 1, start_y, spacing, 1);

  if (selected_option == MENU_OPTION_QUIT) {
    vg_draw_rectangle(menu_x, quit_y - 5, 80, 30, 0xE0E0FF);
  }
  draw_centered_paragraph(&quit_text, 1, quit_y, spacing, 1);
}

void handle_menu_input(GameState *state) {
  // Keyboard Input
  if (scancode != 0) {
    switch (scancode) {
      case UP_ARROW:
        selected_option = MENU_OPTION_START;
        break;
      case DOWN_ARROW:
        selected_option = MENU_OPTION_QUIT;
        break;
      case ENTER_KEY:
        if (selected_option == MENU_OPTION_START) {
          *state = INSTRUCTIONS;
        }
        else if (selected_option == MENU_OPTION_QUIT) {
          *state = EXIT_GAME;
        }
        break;
      case ESC_KEY:
        *state = EXIT_GAME;
        break;
    }

    scancode = 0;
  }

  // Mouse Input
  static int prev_lb = 0;

  uint16_t menu_x = x_resolution / 2 - 50;
  
  uint16_t start_btn_x = menu_x - 10;
  uint16_t start_btn_y = 295;
  uint16_t start_btn_w = 100;
  uint16_t start_btn_h = 30;

  uint16_t quit_btn_x = menu_x;
  uint16_t quit_btn_y = 345;
  uint16_t quit_btn_w = 80;
  uint16_t quit_btn_h = 30;
  
  if (mouse_packet.lb && !prev_lb) {
    if (mouse_x >= start_btn_x && mouse_x <= start_btn_x + start_btn_w &&
        mouse_y >= start_btn_y && mouse_y <= start_btn_y + start_btn_h) {
      selected_option = MENU_OPTION_START;
      *state = INSTRUCTIONS;
    }
    else if (mouse_x >= quit_btn_x && mouse_x <= quit_btn_x + quit_btn_w &&
             mouse_y >= quit_btn_y && mouse_y <= quit_btn_y + quit_btn_h) {
      selected_option = MENU_OPTION_QUIT;
      *state = EXIT_GAME;
    }
  }
  prev_lb = mouse_packet.lb;
}
