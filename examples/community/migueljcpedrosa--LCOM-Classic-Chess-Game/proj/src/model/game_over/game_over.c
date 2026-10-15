#include "game_over.h"
#include "model/game/game.h"

GameOver* gameOver;

void create_game_over(){

  Button playButtonMenu;
  Button exitButtonMenu;

  playButtonMenu.x = 650;
  playButtonMenu.y = 500;
  playButtonMenu.sprite = playButton;
  playButtonMenu.height = playButton->img.height;
  playButtonMenu.width = playButton->img.width;

  exitButtonMenu.x = 650;
  exitButtonMenu.y = 400;
  exitButtonMenu.sprite = exitButton;
  exitButtonMenu.height = exitButton->img.height;
  exitButtonMenu.width = exitButton->img.width;

  gameOver = malloc(sizeof(GameOver));
  gameOver->result = game_result;
  gameOver->winner = game_winner;
  gameOver->playButton = playButtonMenu;
  gameOver->exitButton = exitButtonMenu;
}

bool game_over_clicked_play(CursorInput* input){

  if (gameOver->playButton.x <= input->x && input->x <= gameOver->playButton.x +  gameOver->playButton.width &&
      gameOver->playButton.y <= input->y && input->y <= gameOver->playButton.y + gameOver->playButton.height)
    return true;

  return false;
}

bool game_over_clicked_exit(CursorInput* input){

  if (gameOver->exitButton.x <= input->x && input->x <= gameOver->exitButton.x + gameOver->exitButton.width &&
      gameOver->exitButton.y <= input->y && input->y <= gameOver->exitButton.y + gameOver->exitButton.height)
    return true;

  return false;
}

void destroy_game_over(){

  free(gameOver);
}
