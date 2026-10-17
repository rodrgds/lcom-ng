#include "mouseController.h"

extern struct packet p;


void handlePacket(Mouse* mouse) {
  updatePosition(mouse);
  if (p.rb) {
    handleRightClick(mouse);
  }
  if (p.lb) {
    handleLeftClick(mouse);
  }
}

void updatePosition(Mouse* mouse) {
  updatePositionX(mouse);
  updatePositionY(mouse);
}

void updatePositionX(Mouse* mouse) {
  if (((mouse->pos.x + p.delta_x) <= 0) && (!p.x_ov)) {
    mouse->pos.x = 0;
  }
  else if (((mouse->pos.x + p.delta_x) >= 800 - mouse->sprite->w - 1) && (!p.x_ov)) {
    mouse->pos.x = 800 - mouse->sprite->w - 1;
  }
  else if (!p.x_ov){
    mouse->pos.x += p.delta_x;
  }
}

void updatePositionY(Mouse* mouse) {
  if (((mouse->pos.y - p.delta_y) <= 0) && (!p.y_ov)) {
    mouse->pos.y = 0;
  }
  else if (((mouse->pos.y - p.delta_y) >= 600 - mouse->sprite->h - 1) && (!p.y_ov)) {
    mouse->pos.y = 600 - mouse->sprite->h - 1;
  }
  else if (!p.y_ov){
    mouse->pos.y -= p.delta_y;
  }
}

uint8_t getRowInGame(Mouse* mouse) {
  return (mouse->pos.y / 40);
}

uint8_t getColInGame(Mouse* mouse) {
  return (mouse->pos.x / 40);
}

void handleRightClick(Mouse* mouse) {
  uint8_t x = getColInGame(mouse);
  uint8_t y = getRowInGame(mouse);
  if (x <= 14 && y <= 14) {
    if (gameBoard[x][y].isFlagged == false && flags > 0) {
      flags--;
      gameBoard[x][y].isFlagged = true;
      drawFlag(x, y);
    }
    else {
      flags++;
      gameBoard[x][y].isFlagged = false;
      drawRectangleToBackground(gameBoard[x][y].color, x * 40, y * 40, 40, 40); 
    }
  }
}

void handleLeftClick(Mouse* mouse) {
  if (getColInGame(mouse) >= 15 || getRowInGame(mouse) >= 15) return;
  if (firstMove) {
    deployBombs(getColInGame(mouse), getRowInGame(mouse));
    dig(getColInGame(mouse), getRowInGame(mouse));
    firstMove = false;  
  }
  else {
    if (gameBoard[getColInGame(mouse)][getRowInGame(mouse)].hasMine) {
      for (uint8_t x = 0; x < 15; x++) {
        for (uint8_t y = 0; y < 15; y++) {
          if (gameBoard[x][y].hasMine) {
            DrawMine(x, y);
          }
        }
      }
      startedExplosionTimer = true;
      flags = NUMBER_OF_BOMBS;
    }
    else {
      dig(getColInGame(mouse), getRowInGame(mouse));
    }
  }
}




