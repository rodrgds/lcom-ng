#ifndef _LCOM_IO_CONTROLER_H
#define _LCOM_IO_CONTROLER_H

#include <lcom/lcf.h>
// #include "src/drivers/Keyboard_and_Mouse/mouse.h"
#include "IO_sprites.h"
#include "src/drivers/Graphics/graphics.h"
#include "src/game/app_controller.h"
#include "src/game/game_config.h"


typedef enum {
  STAY_BT,
  SPLIT_BT,
  HIT_BT,
  DOUBLE_BT,
  STAY_BT_N,
  SPLIT_BT_N,
  HIT_BT_N,
  DOUBLE_BT_N,
} buttons_active;

typedef struct {
  buttons_active buttons[MAX_BUTTONS]; // Up to 16 active buttons
  int count;
} buttons_active_list;

int IO_controller_display_frame();

int IO_controller_init();

int IO_controller_display_menu();

int IO_controller_display_table();

int IO_controller_close();

int IO_controller_init_sprites();

Sprite *IO_get_card_sprite(card_t card);

int IO_controller_display_player_hands();

int IO_controller_display_dealer_hand();

int IO_controller_keyboard_handler();

int IO_controller_mouse_handler();

void test_display_all_cards();

int IO_controller_display_ready_button();

int IO_controller_display_bet_buttons();

int IO_controller_display_turn_buttons();

int IO_controller_display_wining_players();

int IO_controller_display_money();

int IO_controller_display_current_player_money();

int IO_controller_display_bet_selector();
#endif // IO_CONTROLER_H
