#include "view.h"

#include "sprite_manager.h"

#include "../io/video/vg.h"
#include "../io/rtc/rtc.h"
#include "../io/rtc/rtc.c"

static uint8_t *pool_buffer;
static uint8_t *menu_buffer;
uint16_t refresh_time = 0;
extern date_t datetime;

int init_view() {

  if (vg_init(0x103) == NULL) {
    vg_exit();
    return 1;
  }

  if (load_sprites() != 0) {
    vg_exit();
    return 1;
  }

  pool_buffer = (uint8_t*) malloc(vsize);
  menu_buffer = (uint8_t*) malloc(vsize);

  if (pool_buffer == NULL || menu_buffer == NULL) {
    vg_exit();
    return 1;
  }

  memset(video_back, 0, vsize);
  vg_draw_sprite(table, 0, 100);

  memcpy(pool_buffer, video_back, vsize);

  memset(video_back, 0, vsize);
  vg_draw_sprite(menu, 0, 0);

  memcpy(menu_buffer, video_back, vsize);

  return 0;
}

int stop_view() {

  if (vg_exit() != 0) {
    return 1;
  }

  unload_sprites();

  free(pool_buffer);
  free(menu_buffer);

  return 0;
}

int render_game_view(pool_game *game, vec_2d mouse) {

  if (game == NULL) {
    return 1;
  }

  memcpy(video_back, pool_buffer, vsize);
  
  if(game->state == GAME_OVER){
    sprite* game_over = game->turn ? player2_win : player1_win;
    vg_draw_sprite(game_over, 270, 50);
  }
  else vg_draw_sprite(game_header, 0, 0);

  if ((game->state == WAITING_FOR_HIT || game->state == PREPARING_HIT) && game->state != GAME_OVER) {
    uint8_t time = 20 - game->current_turn_time / 60;
    uint8_t decimal = time / 10;
    uint8_t unit = time % 10;
    vg_draw_sprite(numbers[decimal], 355, 20);
    vg_draw_sprite(numbers[unit], 385, 20);
    vg_draw_sprite(numbers[10], 415, 20);
  }

  if(game->state != GAME_OVER) {
    uint16_t x = game->turn == 0 ? 20 : 760;
    uint16_t y = 15;
    vg_draw_sprite(ball_turn, x, y);
  }
  
  uint8_t radius = game->balls[0].radius;
  for (int i = 0; i < 16; i++) {
    if (game->in_pocket[i]) continue;
    vec_2d v = game->balls[i].pos;
    vg_draw_sprite(balls[i], v.x - radius, v.y - radius);
  }

  if (game->state == PREPARING_HIT) {
    sprite* st = game->turn ? stick1 : stick2;
    vec_2d cue_pos = game->balls[0].pos;
    float padding = get_cue_padding(cue_pos, mouse);
    vg_draw_sprite_rotated(st, cue_pos.x, cue_pos.y, get_cue_angle(&game->balls[0], mouse), (uint8_t)padding);
  }

  if(game->state != GAME_OVER) vg_draw_sprite(cursor, mouse.x, mouse.y);

  return vg_flip_buffer();
}

int render_menu_view(int current_menu_item) {
    memcpy(video_back, menu_buffer, vsize);

    vg_draw_rectangle(0, 0, vg_get_hres(), vg_get_vres(), 34);

    int x_start = vg_get_hres() / 6;
    int y_start = vg_get_vres() / 2.5;
    int y_offset = 50;
    vg_draw_sprite(menu, 0, 0);

    vec_2d clear_pos = (vec_2d){x_start, y_start + y_offset * current_menu_item};
    
    sprite* local = current_menu_item == 0 ? local_game_arrow : local_game;
    sprite* about_sprite = current_menu_item == 1 ? about_arrow : about; 
    
    vg_draw_rectangle(clear_pos.x, clear_pos.y, 150, 27, 34);
    vg_draw_sprite(local, x_start, y_start);
    vg_draw_sprite(about_sprite, x_start, y_start + y_offset);


    if(refresh_time >= 3600) {
      if(update_datetime() != 0) return 1;
      refresh_time = 0;
    }

    // draw current time (hours + seconds)
    uint8_t hours = datetime.hour;
    uint8_t minutes = datetime.minute;
  
    // get each digit
    uint8_t hours_decimal = hours / 10;
    uint8_t hours_unit = hours % 10;
    uint8_t minutes_decimal = minutes / 10;
    uint8_t minutes_unit = minutes % 10;

    // draw hours
    vg_draw_sprite(numbers[hours_decimal], 353, 400);
    vg_draw_sprite(numbers[hours_unit], 393, 400);
    
    // draw separator
    vg_draw_sprite(two_points, 425, 405);

    // draw seconds
    vg_draw_sprite(numbers[minutes_decimal], 440, 400);
    vg_draw_sprite(numbers[minutes_unit], 480, 400);
    
    return vg_flip_buffer();
}


int render_about_view() {
    memcpy(video_back, menu_buffer, vsize);
    vg_draw_rectangle(0, 0, vg_get_hres(), vg_get_vres(), 34);

    vg_draw_sprite(about_screen,0,0);

    return vg_flip_buffer();
}
