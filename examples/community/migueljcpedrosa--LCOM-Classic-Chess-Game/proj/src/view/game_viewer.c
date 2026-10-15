#include "game_viewer.h" 
#include "../drivers/video_card/gpu.h"
#include "../model/cursor/cursor.h"
#include "../model/game/game.h"

static unsigned int selected_piece_old_x = 0;
static unsigned int  selected_piece_old_y = 0;

static unsigned int square_color = 0xeeeed2; 
static unsigned int square_color_alt = 0x769656; 

int (game_screenshot)(){

  clean_screen();

  board_start = (v_res - (8 * square_size))  / 2;
  draw_board();
  draw_pieces();

  if (copy_buffer_to_screenshot())
    return 1;
  
  return 0;
}

int (game_refresh)(){

  if (erase_cursor())
    return 1;

  if (erase_timers())
    return 1;

  if (draw_timers())
    return 1;

  if (draw_cursor())
    return 1;

  if (selected_piece != NULL){

    if (draw_screenshot_to_buffer(selected_piece_old_x, selected_piece_old_y, square_size, square_size))
      return 1;

    int x = cursor->x - square_size / 2;
    int y = cursor->y - square_size / 2;
    
    if (draw_sprite(selected_piece->sprite, x, y))
      return 1;

    selected_piece_old_x = x;
    selected_piece_old_y = y;
  }

  if (render_screen())
    return 1;

  return 0;
}

int (draw_board)() {
  
  uint32_t color;
  for (int w = 0; w < 8; w++) {
    for (int h = 0; h < 8; h++) {
      if((w+h) % 2 == 0)
        color = square_color;
      else
        color = square_color_alt;

      int x = board_start + (w * square_size);
      int y = board_start + (h * square_size);

      if (draw_rectangle(x, y, square_size, square_size, color))
        return 1;
    }
  }
  return 0;
}

int (draw_piece)(Sprite* sprite, uint16_t x, uint16_t y) {
  
  x = board_start + (x * square_size);
  y = board_start + (y * square_size);
  if (draw_sprite(sprite, x, y))
    return 1;

  return 0;
}

int (draw_pieces)(){

  if (selected_piece != NULL){

    for (int i = 0; i < selected_piece->num_moves; i++) {
      Move move = selected_piece->moves[i];
      int x = board_start + (move.destination.x * square_size);
      int y = board_start + (move.destination.y * square_size);
      if (draw_rectangle(x, y, square_size, square_size, 0xFF0000))
        return 1;
    }
  }

  for (int i = 0; i < 64; i++) {
    if (game->board->squares[i] != NULL) {

      Piece* piece = game->board->squares[i];
      if (piece != NULL){
        if (piece->status == ALIVE && piece != selected_piece)
          if (draw_piece(piece->sprite, piece->position.x, piece->position.y))
            return 1;
      }
    }
  }

  return 0;
}

int (draw_timer)(char timer[], int x, int y){

  for (int i = 0; i < 5; i++){

    if (draw_number(numbers->img, numbers->addr, x, y, timer[i]))
      return 1;
    
    x += numbers->img.width / 12;
  }

  return 0;
}

int (draw_timers)(){

  int x = h_res - numbers->img.width / 12 * 5 - 30;
  int y = board_start;

  int count = game->black_player->time_counter;
  int count_in_seconds = (count + 59) / 60;

  int minutes = count_in_seconds / 60;
  int seconds = count_in_seconds % 60;

  char time[6];

  sprintf(time, "%02d:%02d", minutes, seconds);

  if (draw_timer(time, x, y))
    return 1;

  y = v_res - board_start - numbers->img.height;

  count = game->white_player->time_counter;
  count_in_seconds = (count + 59) / 60;

  minutes = count_in_seconds / 60;
  seconds = count_in_seconds % 60;

  sprintf(time, "%02d:%02d", minutes, seconds);

  if (draw_timer(time, x, y))
    return 1;

  return 0;
}

int (erase_timers)(){

  int x = h_res - numbers->img.width / 12 * 5 - 30;
  int y = board_start;
  unsigned int width = 50 * 5;
  unsigned int height = numbers->img.height;

  if (draw_screenshot_to_buffer(x, y, width, height))
    return 1;

  y = v_res - board_start - height;

  if (draw_screenshot_to_buffer(x, y, width, height))
    return 1;
  
  return 0;
}
