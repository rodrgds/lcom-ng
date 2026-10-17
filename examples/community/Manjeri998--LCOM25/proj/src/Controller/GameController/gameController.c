#include "gameController.h"

uint16_t time_of_play = 000;
uint8_t flags = 35;
uint8_t holes = 0;
bool board_cleared = false;
bool exploded = false;

void buildGameBoard() {
    firstMove = true;
    for (uint8_t x = 0; x < 15; x++) {
        for (uint8_t y = 0; y < 15; y++) {
            gameBoard[x][y].color = ((x + y) % 2 == 0) ? GREEN_SQUARE_1 : GREEN_SQUARE_2;
            gameBoard[x][y].dirt_color = ((x + y) % 2 == 0) ? DIRT_SQUARE_1 : DIRT_SQUARE_2;
            gameBoard[x][y].pos.x = (x * 40);
            gameBoard[x][y].pos.y = (y * 40);
            gameBoard[x][y].hasMine = false;
            gameBoard[x][y].nCloseMines = 0;
            gameBoard[x][y].isFlagged = false;
            gameBoard[x][y].full = true;
        }
    }
}

bool positionIsRestricted(uint8_t pos_x, uint8_t pos_y, uint8_t x, uint8_t y) {
    return (pos_x >= (x - 1) && pos_x <= (x + 1) && pos_y >= (y - 1) && pos_y <= (y + 1));
}

void deployBombs(uint8_t dig_x, uint8_t dig_y) {
    Position freePositions[15 * 15];
    int i = 0;
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 15; y++) {
            if (!positionIsRestricted(x, y, dig_x, dig_y)) {
                freePositions[i].x = x;
                freePositions[i].y = y;
                i++;
            }
        } 
    }
    srand(time(NULL));
    for (int r = 0; r < i; r++) {
      int j = rand() % i;
      Position temp = freePositions[r];
      freePositions[r] = freePositions[j];
      freePositions[j] = temp;
    }

    for (int n = 0; n < NUMBER_OF_BOMBS; n++) {
      int x = freePositions[n].x;
      int y = freePositions[n].y;
      gameBoard[x][y].hasMine = true;
      setProximityAlert(x, y);
      DrawMine(x, y);
    }
}

void setProximityAlert(uint8_t bomb_col, uint8_t bomb_row) {
    if (bomb_col > 0) {
      gameBoard[bomb_col - 1][bomb_row].nCloseMines++;
    }
    if (bomb_col < 14) {
      gameBoard[bomb_col + 1][bomb_row].nCloseMines++;
    }
    if (bomb_col > 0 && bomb_row < 14) {
      gameBoard[bomb_col - 1][bomb_row + 1].nCloseMines++;
    }
    if (bomb_col < 14 && bomb_row < 14) {
      gameBoard[bomb_col + 1][bomb_row + 1].nCloseMines++;
    }
    if (bomb_col > 0 && bomb_row > 0) {
      gameBoard[bomb_col - 1][bomb_row - 1].nCloseMines++;
    }
    if (bomb_col < 14 && bomb_row > 0) {
      gameBoard[bomb_col + 1][bomb_row - 1].nCloseMines++;
    }
    if (bomb_row > 0) {
      gameBoard[bomb_col][bomb_row - 1].nCloseMines++;
    }
    if (bomb_row < 14) {
      gameBoard[bomb_col][bomb_row + 1].nCloseMines++;
    }
}

void dig(uint8_t col, uint8_t row) {
  if (col < 0 || row < 0 || col >= 15 || row >= 15) return;
    
  if (!gameBoard[col][row].full) return;
    
  if (gameBoard[col][row].full && gameBoard[col][row].nCloseMines > 0) {
    gameBoard[col][row].full = false;
    DrawDig(col, row);
    holes++;
    printf("%d", holes);
    DrawProximityCounter(col, row);
    if (holes >= (15 * 15 - NUMBER_OF_BOMBS)) {
      board_cleared = true;
    }
    return;
  }
  if (gameBoard[col][row].full && gameBoard[col][row].nCloseMines == 0) {
    gameBoard[col][row].full = false; 
    DrawDig(col, row);
    holes++;
    dig(col - 1, row - 1); 
    dig(col - 1, row); 
    dig(col - 1, row + 1); 
    dig(col, row - 1); 
    dig(col, row + 1); 
    dig(col + 1, row - 1); 
    dig(col + 1, row); 
    dig(col + 1, row + 1); 
    if (holes >= (15 * 15 - NUMBER_OF_BOMBS)) {
      board_cleared = true;
    }
    return;
  }
  
}
