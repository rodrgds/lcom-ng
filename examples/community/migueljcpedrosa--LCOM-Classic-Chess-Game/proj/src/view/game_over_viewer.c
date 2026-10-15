/**
 * @file game_over_viewer
 * @brief This file has the implementation of the functions related to the game over menu.
 */

#include "game_over_viewer.h"
#include "viewer.h"
#include "model/game_over/game_over.h"

int (draw_game_over_title)(){

  int x = 500;
  int y = 100;
  
  draw_word(alphabet, x, y, gameOver->result);

  y += 100;

  draw_word(alphabet, x, y, gameOver->winner);

  return 0;
}

int (draw_game_over_buttons)(){

  if(draw_sprite(gameOver->playButton.sprite, gameOver->playButton.x, gameOver->playButton.y)){
       return 1;
  }

  if(draw_sprite(gameOver->exitButton.sprite, gameOver->exitButton.x, gameOver->exitButton.y)){
       return 1;
  }

  return 0;
}

int (draw_game_over)(){

  if(draw_sprite(chessBackground, 0, 0))  return 1;

  if(draw_game_over_title()) return 1;

  if (draw_game_over_buttons()) return 1;

  return 0;
}

int (game_over_screenshot)(){

  if (clean_screen()) return 1;

  if(draw_game_over()) return 1;

  if (copy_buffer_to_screenshot())
    return 1;

  return 0;
}

int (game_over_refresh)(){

  if(erase_cursor()) return 1;

  if (draw_cursor()) return 1;

  if (render_screen()) return 1;

  return 0;
}

