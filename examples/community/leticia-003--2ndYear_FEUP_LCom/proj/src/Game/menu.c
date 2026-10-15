/**
 * @file menu.c
 * @brief Implements the main menu logic, drawing, and input handling for the game.
 */

#include "menu.h"
#include "Drivers/VBE/vbe.h"
#include "Drivers/Keyboard/i8042.h"
#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <stdio.h>
#include <string.h>
#include "Drivers/Keyboard/keyboard.h"
#include "Sprites/start_sprite.h"
#include "game_state.h"
#include "game_logic.h"

bool needs_redraw = true;

static uint16_t play_w = 0, play_h = 0;

static xpm_image_t play_img;
static uint8_t *play_map = NULL;

static xpm_image_t logoff_img;
static uint8_t *logoff_map = NULL;

static xpm_image_t instructions_img;
static uint8_t *instructions_map = NULL;

static xpm_image_t instructions_hovered_img;
static uint8_t *instructions_hovered_map = NULL;

static xpm_image_t title_img;
static uint8_t *title_map = NULL;

static xpm_image_t instructions_text_img;
static uint8_t *instructions_text_map = NULL;

static vbe_mode_info_t mode_info;

static bool pattern_drawn = false;

static xpm_image_t back_img;
static uint8_t *back_map = NULL;

/**
 * @enum MenuOption
 * @brief Represents the selectable options in the main menu.
 */
typedef enum {
  MENU_OPTION_START,        
  MENU_OPTION_INSTRUCTIONS,  
  MENU_OPTION_EXIT,          
  MENU_OPTION_COUNT      
} MenuOption;

static MenuOption selected_option = MENU_OPTION_START;

/**
 * @brief Handles keyboard input for the menu, updating selection and state.
 * @param scancode Pointer to the scancode array.
 * @param scancode_size Size of the scancode array.
 */
void menu_update_keyboard(uint8_t *scancode, uint8_t scancode_size);

/**
 * @brief Loads all menu assets (sprites, images, etc).
 * @param info Pointer to VBE mode info.
 * @return 0 on success, 1 on failure.
 */
int menu_assets_init(const vbe_mode_info_t *info);

/**
 * @brief Frees all loaded menu assets.
 */
void menu_assets_destroy(void);

/**
 * @brief Draws a menu button, with hover effect if selected.
 * @param x X position.
 * @param y Y position.
 * @param normal_img Pointer to normal image.
 * @param normal_map Pointer to normal image map.
 * @param hover_img Pointer to hovered image.
 * @param hover_map Pointer to hovered image map.
 * @param selected Whether the button is selected.
 */
static void draw_button(uint16_t x, uint16_t y,
  const xpm_image_t *normal_img, uint8_t *normal_map,
  const xpm_image_t *hover_img , uint8_t *hover_map ,
  bool selected);

/**
 * @brief Draws the main menu or instructions screen.
 */
void menu_draw(void);

void menu_update_keyboard(uint8_t *scancode, uint8_t scancode_size) {
  GameState current_state = game_get_state();

  if (current_state == STATE_INSTRUCTIONS) {
    if ((scancode_size == 2 && scancode[0] == 0xE0 && scancode[1] == ESC_MAKE_CODE) ||
        (scancode_size == 1 && scancode[0] == ESC_MAKE_CODE) ||
        (scancode_size == 2 && scancode[0] == 0xE0 && (scancode[1] == B_MAKE_CODE || scancode[1] == ESC_MAKE_CODE))) {
      game_set_state(STATE_MENU);
      needs_redraw = true;
    }
    return;
  }

  if (scancode_size == 2 && scancode[0] == 0xE0) {
    switch (scancode[1]) {
      case 0x50: 
        selected_option = (selected_option + 1) % MENU_OPTION_COUNT;
        printf("Selected option: %d\n", selected_option);
        needs_redraw = true;
        break;
      case 0x48:
        selected_option = (selected_option + MENU_OPTION_COUNT - 1) % MENU_OPTION_COUNT;
        printf("Selected option: %d\n", selected_option);
        needs_redraw = true;
        break;
      case 0x1C:
        printf("Enter pressed\n");
        printf("Selected option: %d\n", selected_option);
        if (selected_option == MENU_OPTION_START){
          game_set_state(STATE_PLAYING);
          reset_game();
        }else if (selected_option == MENU_OPTION_INSTRUCTIONS)
          game_set_state(STATE_INSTRUCTIONS);
        else
          game_set_state(STATE_EXIT);
        needs_redraw = true;
        printf("Current state: %d\n", game_get_state());
        break;
    }
  } else {
    switch (scancode[0]) {
      case W_MAKE_CODE:
        selected_option = (selected_option + MENU_OPTION_COUNT - 1) % MENU_OPTION_COUNT;
        needs_redraw = true;
        break;
      case S_MAKE_CODE:
        selected_option = (selected_option + 1) % MENU_OPTION_COUNT;
        needs_redraw = true;
        break;
      case ENTER_MAKE_CODE:
        if (selected_option == MENU_OPTION_START) {
          game_set_state(STATE_PLAYING);
          reset_game();
        }
        else if (selected_option == MENU_OPTION_INSTRUCTIONS) {
          printf("Switching to instructions state\n");
          game_set_state(STATE_INSTRUCTIONS);
        } else
          game_set_state(STATE_EXIT);
        needs_redraw = true;
        break;
      case ESC_MAKE_CODE:
        game_set_state(STATE_EXIT);
        needs_redraw = true;
        break;
    }
  }
}

int menu_assets_init(const vbe_mode_info_t *info) {
  mode_info = *info;
  pattern_drawn = false;

  title_map = xpm_load(title_xpm, XPM_8_8_8_8, &title_img);
  if (!title_map) return 1;

  play_map = xpm_load(play_button_xpm, XPM_8_8_8_8, &play_img);
  if (!play_map) return 1;

  play_hovered_map = xpm_load(play_button_hovered_xpm, XPM_8_8_8_8, &play_hovered_img);
  if (!play_hovered_map) { free(play_map); return 1; }

  logoff_map = xpm_load(logoff_button_xpm, XPM_8_8_8_8, &logoff_img);
  if (!logoff_map) { free(play_map); free(play_hovered_map); return 1; }

  instructions_map = xpm_load(instructions_button_xpm, XPM_8_8_8_8, &instructions_img);
  if (!instructions_map) { free(play_map); free(play_hovered_map); return 1; }

  instructions_hovered_map = xpm_load(instructions_button_hovered_xpm, XPM_8_8_8_8, &instructions_hovered_img);
  if (!instructions_hovered_map) {
    free(play_map); free(play_hovered_map); free(logoff_map); free(instructions_map);
    return 1;
  }

  instructions_text_map = xpm_load(instructions_text_xpm, XPM_8_8_8_8, &instructions_text_img);
  if (!instructions_text_map) return 1;

  back_map = xpm_load(back_button_xpm, XPM_8_8_8_8, &back_img);
  if (!back_map) return 1;

  play_w = play_img.width;
  play_h = play_img.height;

  return 0;
}

void menu_assets_destroy(void) {
  if (play_map) free(play_map);
  if (play_hovered_map) free(play_hovered_map);
  if (logoff_map) free(logoff_map);
  if (instructions_map) free(instructions_map);
  if (instructions_hovered_map) free(instructions_hovered_map);
  if (instructions_text_map) free(instructions_text_map);
  if (back_map) free(back_map);
}

static void draw_button(uint16_t x, uint16_t y,
  const xpm_image_t *normal_img, uint8_t *normal_map,
  const xpm_image_t *hover_img , uint8_t *hover_map ,
  bool selected) {
  vg_draw_xpm(x, y, normal_img->width, normal_img->height, normal_map, false);
  if (selected)
    vg_draw_xpm(x, y, hover_img->width, hover_img->height, hover_map, false);
}

void menu_draw(void) {
  vg_clear_screen();
  vg_draw_rectangle(0, 0, h_res, v_res, 0xB2DAFA);

  GameState current_state = game_get_state();

  if (current_state == STATE_INSTRUCTIONS) {
    vg_draw_xpm(10, 10, back_img.width, back_img.height, back_map, false);

    if (instructions_text_map == NULL) {
      printf("Instructions XPM not loaded!\n");
      return;
    }
    uint16_t x = (h_res - instructions_text_img.width) / 2;
    uint16_t y = (v_res - instructions_text_img.height) / 2;
    vg_draw_xpm(x, y, instructions_text_img.width, instructions_text_img.height, instructions_text_map, false);

    needs_redraw = false;
    return;
  }

  uint16_t x_play = h_res / 2 - play_w / 2;
  uint16_t y_play = (v_res - play_h) / 2 + 210;
  uint16_t x_instr = h_res / 2 - instructions_img.width / 2;
  uint16_t y_instr = y_play + play_h + 20;
  uint16_t x_title = (h_res - title_img.width) / 2;
  uint16_t y_title = 0;

  draw_button(x_play, y_play, &play_img, play_map, &play_hovered_img, play_hovered_map, selected_option == MENU_OPTION_START);
  draw_button(x_instr, y_instr, &instructions_img, instructions_map, &instructions_hovered_img, instructions_hovered_map, selected_option == MENU_OPTION_INSTRUCTIONS);

  vg_draw_xpm(10, 10, logoff_img.width, logoff_img.height, logoff_map, false);
  vg_draw_xpm(x_title, y_title, title_img.width, title_img.height, title_map, false);

  needs_redraw = false;
}

