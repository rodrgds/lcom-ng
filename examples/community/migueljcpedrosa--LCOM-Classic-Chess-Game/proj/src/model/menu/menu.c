
#include "menu.h"
#include <stdbool.h>

Menu* menu;

void create_menu(){

  menu = malloc(sizeof(Menu));

  Title title;

  title.x = 600;
  title.y = 100;
  title.words = malloc(2 * sizeof(char*));
  title.words[0] = "CHESS";
  title.words[1] = "GAME";
  title.num_words = 2;
  title.font = alphabet;

  Button playButtonMenu;
  Button exitButtonMenu;

  playButtonMenu.x = 650;
  playButtonMenu.y = 350;
  playButtonMenu.sprite = playButton;
  playButtonMenu.height = playButton->img.height;
  playButtonMenu.width = playButton->img.width;
  
  exitButtonMenu.x = 650;
  exitButtonMenu.y = 450;
  exitButtonMenu.sprite = exitButton;
  exitButtonMenu.height = exitButton->img.height;
  exitButtonMenu.width = exitButton->img.width;

  menu->title = title;
  menu->playButton = playButtonMenu;
  menu->exitButton = exitButtonMenu;

  menu->background = chessBackground;
}

void destroy_menu(){

  free(menu->title.words);
  free(menu);
}

bool menu_clicked_play(CursorInput* cursor){

  if (menu->playButton.x <= cursor->x && cursor->x <= menu->playButton.x + menu->playButton.width &&
      menu->playButton.y <= cursor->y && cursor->y <= menu->playButton.y + menu->playButton.height)
    return true;

  return false;
}

bool menu_clicked_exit(CursorInput* cursor){

  if (menu->exitButton.x <= cursor->x && cursor->x <= menu->exitButton.x + menu->exitButton.width &&
      menu->exitButton.y <= cursor->y && cursor->y <= menu->exitButton.y + menu->exitButton.height)
    return true;

  return false;
}
