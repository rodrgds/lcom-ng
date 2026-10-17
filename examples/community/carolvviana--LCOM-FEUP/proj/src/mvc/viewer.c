#include "viewer.h"

extern struct Cursor cursor;
extern struct Arena arena;
extern game_state_t game_state;
extern struct Player player;
extern struct Player player2;
extern struct Player AIplayer;
extern real_time_info rt_info;

void(draw_menu)() {
  draw_xpm(images[MAIN_MENU], 0, 0, 0);
  draw_xpm(images[getMenuOption()], 0, 0, 0);
}

void(draw_mouse)() {
  draw_xpm(images[CURSOR], cursor.x, cursor.y, 0);
}

void(draw_arena)() {
  draw_xpm(images[ARENA], arena.x_min, arena.y_min, 0);
}

void(draw_arena_multiplayer)() {
  draw_xpm(images[ARENA], arena.x_min, arena.y_min, 1);
}

void(draw_credits)() {
  draw_xpm(images[CREDITS_INSTRUCTIONS], 0, 0, 0);
}

void(draw_background)(uint32_t *color) {
  draw_rect(0, 0, 800, 600, *color);
}

void(draw_player)(struct Player *player) {
  draw_rect(player->x_pos, player->y_pos, player->width, player->height, player->color);
}

void(draw_ball)(struct Ball *ball) {
    draw_xpm(images[BALL], ball->x_pos, ball->y_pos, 0);
}

int(getScoreImage)(int score) {
  return score + 7;
}

int(getNumberImage)(int number) {
  return number + 21;
}

void(draw_scores)() {

  switch (game_state) {
    case SINGLE_PLAYER_GAME:
      draw_xpm(images[getScoreImage(player.score)], 280, 0, 0);
      draw_xpm(images[getScoreImage(AIplayer.score)], 440, 0, 0);
      draw_xpm(images[DASH], 360, 0, 0);
      break;
    case MULTI_PLAYER_GAME:
      if (player.x_pos < player2.x_pos) {
        draw_xpm(images[getScoreImage(player.score)], 280, 0, 0);
        draw_xpm(images[getScoreImage(player2.score)], 440, 0, 0);
        draw_xpm(images[DASH], 360, 0, 0);
      }
      else {
        draw_xpm(images[getScoreImage(player2.score)], 280, 0, 0);
        draw_xpm(images[getScoreImage(player.score)], 440, 0, 0);
        draw_xpm(images[DASH], 360, 0, 0);
      }
      break;
    default:
      break;
  }
}

void(draw_time)() {
  // 12:40
  draw_xpm(images[getNumberImage(rt_info.hrs / 10)], 0, 550, 0);
  draw_xpm(images[getNumberImage(rt_info.hrs % 10)], 40, 550, 0);
  draw_xpm(images[COLON], 80, 550, 0);
  draw_xpm(images[getNumberImage(rt_info.mins / 10)], 120, 550, 0);
  draw_xpm(images[getNumberImage(rt_info.mins % 10)], 160, 550, 0);
}

void(draw_blocked)() {
  draw_xpm(images[BLOCKED_SCREEN], 0, 0, 0);
}

void(draw_game_over)() {
  draw_xpm(images[GAME_OVER_SCREEN], 0, 0, 0);
}

void(draw_game_win)() {
  draw_xpm(images[GAME_WIN_SCREEN], 0, 0, 0);
}
