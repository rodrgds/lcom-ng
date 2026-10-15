#include "menuView.h"


int drawStartButton() {
    if (startButton == NULL) return 1;

    if (draw_xpm(startButton->map, startButton->x, startButton->y, startButton->img) != OK) {
        return 1;
    }

    return 0;
}



int drawLevelButton() {
    if (levelButton == NULL) return 1;

    if (draw_xpm(levelButton->map, levelButton->x, levelButton->y, levelButton->img) != OK) {
        return 1;
    }

    return 0;
}

int drawExitButton() {
    if (exitButton == NULL) return 1;

    if (draw_xpm(exitButton->map, exitButton->x, exitButton->y, exitButton->img) != OK) {
        return 1;
    }

    return 0;
}

int drawSpaceInvaders() {
    if (SpaceInvaders == NULL) return 1;

    if (draw_xpm(SpaceInvaders->map,SpaceInvaders->x,SpaceInvaders->y, SpaceInvaders->img) != OK) {
        return 1;
    }
    return 0;
}

int drawLoseButton() {
  if (finalLost == NULL) return 1;

  if (draw_xpm(finalLost->map, finalLost->x, finalLost->y, finalLost->img) != OK) {
      return 1;
  }

  return 0;
}

int drawWinButton() {
    if (finalWin == NULL) return 1;
  
    if (draw_xpm(finalWin->map, finalWin->x, finalWin->y, finalWin->img) != OK) {
        return 1;
    }

    return 0;
}

int drawMenu() {
    if (drawSpaceInvaders() != OK) return 1;
    if (drawStartButton() != OK) return 1;
    if (drawLevelButton() != OK) return 1;
    if (drawExitButton() != OK) return 1;
    update_screen();
    return 0;
}
