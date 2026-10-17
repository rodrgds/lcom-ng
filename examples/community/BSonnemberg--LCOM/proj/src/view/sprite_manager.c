#include "sprite_manager.h"

#include "xpm/table.xpm"
#include "xpm/stick1.xpm"
#include "xpm/stick2.xpm"
#include "xpm/ball0.xpm"
#include "xpm/ball1.xpm"
#include "xpm/ball2.xpm"
#include "xpm/ball3.xpm"
#include "xpm/ball4.xpm"
#include "xpm/ball5.xpm"
#include "xpm/ball6.xpm"
#include "xpm/ball7.xpm"
#include "xpm/ball8.xpm"
#include "xpm/ball9.xpm"
#include "xpm/ball10.xpm"
#include "xpm/ball11.xpm"
#include "xpm/ball12.xpm"
#include "xpm/ball13.xpm"
#include "xpm/ball14.xpm"
#include "xpm/ball15.xpm"
#include "xpm/menu.xpm"
#include "xpm/cursor.xpm"
#include "xpm/about.xpm"
#include "xpm/about_arrow.xpm"
#include "xpm/localgame.xpm"
#include "xpm/localgame_arrow.xpm"
#include "xpm/onlinegame.xpm"
#include "xpm/onlinegame_arrow.xpm"
#include "xpm/game_header.xpm"
#include "xpm/about_screen.xpm"
#include "xpm/number0.xpm"
#include "xpm/number1.xpm"
#include "xpm/number2.xpm"
#include "xpm/number3.xpm"
#include "xpm/number4.xpm"
#include "xpm/number5.xpm"
#include "xpm/number6.xpm"
#include "xpm/number7.xpm"
#include "xpm/number8.xpm"
#include "xpm/number9.xpm"
#include "xpm/seconds.xpm"
#include "xpm/ball_turn.xpm"
#include "xpm/twopoints.xpm"
#include "xpm/player1win.xpm"
#include "xpm/player2win.xpm"


int load_sprites() {
  
  balls = (sprite**) malloc(16 * sizeof(sprite*));
  if (balls == NULL) return 1;
  
  table = create_sprite((xpm_map_t)xpm_table);
  stick1 = create_sprite((xpm_map_t)xpm_stick1);
  stick2 = create_sprite((xpm_map_t)xpm_stick2);
  ball_turn = create_sprite((xpm_map_t)xpm_ball_turn);
  
  balls[0] = create_sprite((xpm_map_t)xpm_ball0);
  balls[1] = create_sprite((xpm_map_t)xpm_ball1);
  balls[2] = create_sprite((xpm_map_t)xpm_ball2);
  balls[3] = create_sprite((xpm_map_t)xpm_ball3);
  balls[4] = create_sprite((xpm_map_t)xpm_ball4);
  balls[5] = create_sprite((xpm_map_t)xpm_ball5);
  balls[6] = create_sprite((xpm_map_t)xpm_ball6);
  balls[7] = create_sprite((xpm_map_t)xpm_ball7);
  balls[8] = create_sprite((xpm_map_t)xpm_ball8);
  balls[9] = create_sprite((xpm_map_t)xpm_ball9);
  balls[10] = create_sprite((xpm_map_t)xpm_ball10);
  balls[11] = create_sprite((xpm_map_t)xpm_ball11);
  balls[12] = create_sprite((xpm_map_t)xpm_ball12);
  balls[13] = create_sprite((xpm_map_t)xpm_ball13);
  balls[14] = create_sprite((xpm_map_t)xpm_ball14);
  balls[15] = create_sprite((xpm_map_t)xpm_ball15);

  menu = create_sprite((xpm_map_t)xpm_menu);

  cursor = create_sprite((xpm_map_t)xpm_cursor);

  about = create_sprite((xpm_map_t)xpm_about);
  about_arrow = create_sprite((xpm_map_t)xpm_about_arrow);
  local_game = create_sprite((xpm_map_t)xpm_local_game);
  local_game_arrow = create_sprite((xpm_map_t)xpm_local_game_arrow);
  online_game = create_sprite((xpm_map_t)xpm_online_game);
  online_game_arrow = create_sprite((xpm_map_t)xpm_online_game_arrow);
  game_header = create_sprite((xpm_map_t)xpm_game_header);
  about_screen = create_sprite((xpm_map_t)xpm_about_screen);
  numbers = (sprite**) malloc(11 * sizeof(sprite*));
  two_points = create_sprite((xpm_map_t)xpm_twopoints);
  player1_win = create_sprite((xpm_map_t)xpm_player1_win);
  player2_win = create_sprite((xpm_map_t)xpm_player2_win);

  numbers[0] = create_sprite((xpm_map_t)xpm_number0);
  numbers[1] = create_sprite((xpm_map_t)xpm_number1);
  numbers[2] = create_sprite((xpm_map_t)xpm_number2);
  numbers[3] = create_sprite((xpm_map_t)xpm_number3);
  numbers[4] = create_sprite((xpm_map_t)xpm_number4);
  numbers[5] = create_sprite((xpm_map_t)xpm_number5);
  numbers[6] = create_sprite((xpm_map_t)xpm_number6);
  numbers[7] = create_sprite((xpm_map_t)xpm_number7);
  numbers[8] = create_sprite((xpm_map_t)xpm_number8);
  numbers[9] = create_sprite((xpm_map_t)xpm_number9);
  numbers[10] = create_sprite((xpm_map_t)xpm_seconds);

  if (menu == NULL) return 1;
  if (table == NULL) return 1;
  if (stick1 == NULL) return 1;
  if (stick2 == NULL) return 1;
  if (cursor == NULL) return 1;
  if (about == NULL) return 1;
  if (about_arrow == NULL) return 1;
  if (local_game == NULL) return 1;
  if (local_game_arrow == NULL) return 1;
  if (online_game == NULL) return 1;
  if (online_game_arrow == NULL) return 1;
  if (game_header == NULL) return 1;
  if (about_screen == NULL) return 1;
  if (ball_turn == NULL) return 1;
  if (two_points == NULL) return 1;
  if (player1_win == NULL) return 1;
  if (player2_win == NULL) return 1;

  for (int i = 0; i < 16; i++) {
    if (balls[i] == NULL) return 1;
  }

  for(int i = 0; i < 11; i++) {
    if (numbers[i] == NULL) {
      return 1;
    }
  }

  return 0;
}

void unload_sprites() {

  destroy_sprite(table);
  destroy_sprite(stick1);
  destroy_sprite(stick2);

  destroy_sprite(cursor);
  destroy_sprite(menu);
  destroy_sprite(about);
  destroy_sprite(about_arrow);
  destroy_sprite(local_game);
  destroy_sprite(local_game_arrow);
  destroy_sprite(online_game);
  destroy_sprite(online_game_arrow);
  destroy_sprite(game_header);
  destroy_sprite(about_screen);  
  destroy_sprite(ball_turn);
  destroy_sprite(two_points);
  destroy_sprite(player1_win);
  destroy_sprite(player2_win);

  for (int i = 0; i < 16; i++) {
    destroy_sprite(balls[i]);
  }

  for(int i = 0; i < 11; i++) {
    destroy_sprite(numbers[i]);
  }

  free(balls);
  free(numbers);
  
  table = NULL;
  stick1 = NULL;
  stick2 = NULL;
  balls = NULL;
  numbers = NULL;
  menu = NULL;
  cursor = NULL;
  about = NULL;
  about_arrow = NULL;
  local_game = NULL;
  local_game_arrow = NULL;
  online_game = NULL;
  online_game_arrow = NULL;
  game_header = NULL;
  about_screen = NULL;
  ball_turn = NULL;
  two_points = NULL;
  player1_win = NULL;
  player2_win = NULL;
}
