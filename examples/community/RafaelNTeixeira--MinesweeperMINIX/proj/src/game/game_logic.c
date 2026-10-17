#include "game_logic.h"

Position cursos_pos;
int mines, x_tiles, y_tiles;
int y_matrix, x_matrix;
int selected_x = 0, selected_y = 0;

int getMatrixIndexX(int x) {
  return round((cursor_pos.x - x) / 16);
}

int getMatrixIndexY(int y) {
  return round((cursor_pos.y - y) / 16);
}

int getXCoordenatesPressedCell_16x16(int x) {
  int new_x = x;
  int x_cursor_click = cursor_pos.x;
  int i = -1;

  while (new_x < x_cursor_click) {
    new_x += 16;
    i++; 
  }

  return x + (i * 16); 
}

int getYCoordenatesPressedCell_16x16(int y) {
  int new_y = y;
  int y_cursor_click = cursor_pos.y;
  int i = -1;

  while (new_y < y_cursor_click) {
    new_y += 16;
    i++; 
  }

  return y + (i * 16);
}

/*---------------- 16 X 16 ---------------------------*/
void placeMines_16x16(char board[16][16], int mines) {
  int i, j, count = 0;

  while (count < mines) {
    i = rand() % x_tiles;
    j = rand() % y_tiles;
    if (board[i][j] != '*') {
      board[i][j] = '*';
      count++;
    }
  }
}

char countAdjacentMines_16x16(char board[16][16], int row, int col) {
  int count = 0;

  for (int i = row - 1; i <= row + 1; i++) {
    for (int j = col - 1; j <= col + 1; j++) {        
      if (i >= 0 && i < 16 && j >= 0 && j < 16) {      
        if (board[i][j] == '*' || board[i][j] == 'F') {
          count++;
        }
      }
    }
  }
  return (count + '0');
}

void initializeBoard_16x16(char board[16][16]) {
  for (int i = 0; i < x_tiles; i++) {
    for (int j = 0; j < y_tiles; j++) {
      board[i][j] = '-';
    }
  }
}

int winGame_16x16(char board[16][16]) {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      if (board[i][j] == '-' || board[i][j] == 'f') {
        return 0;
      }
    }
  }
  return 1;
}


void checkEmptyPlates_16x16(char board[16][16], int x, int y) {

  if (board[x][y] == '-') {
    int adjMines = countAdjacentMines_16x16(board, x, y);
    board[x][y] = adjMines;
  }

  if (board[x][y] == '0'){
    for (int i = x - 1; i <= x + 1; i++) {

      for (int j = y - 1; j <= y + 1; j++) {

        if (i >= 0 && i < 16 && j >= 0 && j < 16) {

          if (board[i][j] == '-') {
            checkEmptyPlates_16x16(board, i, j);
          }
        }
      }
    }
  }
}

/*---------------- 20 X 16 ---------------------------*/

void placeMines_20x16(char board[20][16], int mines) {
  int count = 0;

  while (count < mines) {
    int i = rand() % x_tiles;
    int j = rand() % y_tiles;
    if (board[i][j] != '*') {
        board[i][j] = '*';
        count++;
    }
  }
}

char countAdjacentMines_20x16(char board[20][16], int row, int col) {
  int count = 0;

  for (int i = row - 1; i <= row + 1; i++) {
    for (int j = col - 1; j <= col + 1; j++) {
      if (i >= 0 && i < 20 && j >= 0 && j < 16) {
        if (board[i][j] == '*' || board[i][j] == 'F') {
          count++;
        }
      }
    }
  }
  return (count + '0');
}

void initializeBoard_20x16(char board[20][16]) {
  for (int i = 0; i < x_tiles; i++) {
    for (int j = 0; j < y_tiles; j++) {
      board[i][j] = '-';
    }
  }
}

int winGame_20x16(char board[20][16]) {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 16; j++) {
      if (board[i][j] == '-' || board[i][j] == 'f') {
        return 0;
      }
    }
  }
  return 1;
}


void checkEmptyPlates_20x16(char board[20][16], int x, int y) {

  if (board[x][y] == '-'){
    int adjMines = countAdjacentMines_20x16(board, x, y);
    board[x][y] = adjMines;
  }

  if (board[x][y] == '0'){
    for (int i = x - 1; i <= x + 1; i++) {

      for (int j = y - 1; j <= y + 1; j++) {

        if (i >= 0 && i < 20 && j >= 0 && j < 16) {

          if (board[i][j] == '-') {
            checkEmptyPlates_20x16(board, i, j);
          }
        }
      }
    }
  }
}

/*---------------- 28 X 20 ---------------------------*/

void placeMines_28x20(char board[28][20], int mines) {
  int count = 0;

  while (count < mines) {
    int i = rand() % x_tiles;
    int j = rand() % y_tiles;
    if (board[i][j] != '*') {
      board[i][j] = '*';
      count++;
    }
  }
}

char countAdjacentMines_28x20(char board[28][20], int row, int col) {
  int count = 0;

  for (int i = row - 1; i <= row + 1; i++) {
    for (int j = col - 1; j <= col + 1; j++) {
      if (i >= 0 && i < 28 && j >= 0 && j < 20) {
        if (board[i][j] == '*' || board[i][j] == 'F') {
          count++;
        }
      }
    }
  }
  return (count + '0');
}

void initializeBoard_28x20(char board[28][20]) {
  for (int i = 0; i < x_tiles; i++) {
    for (int j = 0; j < y_tiles; j++) {
        board[i][j] = '-';
    }
  }
}

int winGame_28x20(char board[28][20]) {
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 20; j++) {
      if (board[i][j] == '-' || board[i][j] == 'f') {
        return 0;
      }
    }
  }
  return 1;
}


void checkEmptyPlates_28x20(char board[28][20], int x, int y) {
  if (board[x][y] == '-'){
    int adjMines = countAdjacentMines_28x20(board, x, y);
    board[x][y] = adjMines;
  }

  if (board[x][y] == '0'){
    for (int i = x - 1; i <= x + 1; i++) {

      for (int j = y - 1; j <= y + 1; j++) {

        if (i >= 0 && i < 28 && j >= 0 && j < 20) {

          if (board[i][j] == '-') {
            checkEmptyPlates_28x20(board, i, j);
          }
        }
      }
    }
  }
}
