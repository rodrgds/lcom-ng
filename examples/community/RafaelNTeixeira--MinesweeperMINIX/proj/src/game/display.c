#include "display.h"

xpm_image_t faceGameOver, faceRestart, instructionsButton, playButton, quitButton,
bannerCell, mineCell, normalCell, pressCell, oneCell, twoCell, threeCell, fourCell, fiveCell, sixCell, sevenCell, eightCell, 
selected,
colon,
zero, one, two, three, four, five, six, seven, eight, nine,
play_16x16, play_20x16, play_28x20, grid_16x16, grid_20x16, grid_28x20, 
font, font_white, cursor, empty_cursor, title,
main_menu, instructionsPage, gameOver, gameWin;

int (load_xpms)() {
  xpm_load(faceGameOver_xpm, XPM_8_8_8, &faceGameOver);
  xpm_load(faceRestart_xpm, XPM_8_8_8, &faceRestart);
  xpm_load(instructionsButton_xpm, XPM_8_8_8, &instructionsButton);
  xpm_load(playButton_xpm, XPM_8_8_8, &playButton);
  xpm_load(quitButton_xpm, XPM_8_8_8, &quitButton);
  xpm_load(bannerCell_xpm, XPM_8_8_8, &bannerCell);
  xpm_load(mineCell_xpm, XPM_8_8_8, &mineCell);
  xpm_load(normalCell_xpm, XPM_8_8_8, &normalCell);
  xpm_load(pressCell_xpm, XPM_8_8_8, &pressCell);
  xpm_load(oneCell_xpm, XPM_8_8_8, &oneCell);
  xpm_load(twoCell_xpm, XPM_8_8_8, &twoCell);
  xpm_load(threeCell_xpm, XPM_8_8_8, &threeCell);
  xpm_load(fourCell_xpm, XPM_8_8_8, &fourCell);
  xpm_load(fiveCell_xpm, XPM_8_8_8, &fiveCell);
  xpm_load(sixCell_xpm, XPM_8_8_8, &sixCell);
  xpm_load(sevenCell_xpm, XPM_8_8_8, &sevenCell);
  xpm_load(eightCell_xpm, XPM_8_8_8, &eightCell);
  xpm_load(selected_xpm, XPM_8_8_8, &selected);
  xpm_load(menu_xpm, XPM_8_8_8, &main_menu);
  xpm_load(board_16x16_xpm, XPM_8_8_8, &play_16x16);
  xpm_load(grid_16x16_xpm, XPM_8_8_8, &grid_16x16);
  xpm_load(board_20x16_xpm, XPM_8_8_8, &play_20x16);
  xpm_load(grid_20x16_xpm, XPM_8_8_8, &grid_20x16);
  xpm_load(board_28x20_xpm, XPM_8_8_8, &play_28x20);
  xpm_load(grid_28x20_xpm, XPM_8_8_8, &grid_28x20);
  xpm_load(font_xpm, XPM_8_8_8, &font);
  xpm_load(font_white_xpm, XPM_8_8_8, &font_white);
  xpm_load(handCursor_xpm, XPM_8_8_8, &cursor);
  xpm_load(empty_cursor_xpm, XPM_8_8_8, &empty_cursor);
  xpm_load(instructionsPage_xpm, XPM_8_8_8, &instructionsPage);
  xpm_load(gameOver_xpm, XPM_8_8_8, &gameOver);
  xpm_load(win_xpm, XPM_8_8_8, &gameWin);
  xpm_load(colon_xpm, XPM_8_8_8, &colon);
  xpm_load(zero_xpm, XPM_8_8_8, &zero);
  xpm_load(one_xpm, XPM_8_8_8, &one);
  xpm_load(two_xpm, XPM_8_8_8, &two);
  xpm_load(three_xpm, XPM_8_8_8, &three);
  xpm_load(four_xpm, XPM_8_8_8, &four);
  xpm_load(five_xpm, XPM_8_8_8, &five);
  xpm_load(six_xpm, XPM_8_8_8, &six);
  xpm_load(seven_xpm, XPM_8_8_8, &seven);
  xpm_load(eight_xpm, XPM_8_8_8, &eight);
  xpm_load(nine_xpm, XPM_8_8_8, &nine);
  return 0;
}

void draw_main_menu() {
  draw_xpm(main_menu, 0, 0);
}

void draw_board_16x16() {
  draw_xpm(play_16x16, 0, 0);
}

void draw_board_20x16() {
  draw_xpm(play_20x16, 0, 0);
}

void draw_board_28x20() {
  draw_xpm(play_28x20, 0, 0);
}

void draw_grid_16x16(char board[16][16]) {
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      switch (board[x][y]) {
        case '0':
          draw_xpm(pressCell, 270+x*16, 262+y*16);
          break;

        case '1':
          draw_xpm(oneCell, 270+x*16, 262+y*16);
          break;
          
        case '2':
          draw_xpm(twoCell, 270+x*16, 262+y*16);
          break;

        case '3':
          draw_xpm(threeCell, 270+x*16, 262+y*16);
          break;

        case '4':
          draw_xpm(fourCell, 270+x*16, 262+y*16);
          break;

        case '5':
          draw_xpm(fiveCell, 270+x*16, 262+y*16);
          break;

        case '6':
          draw_xpm(sixCell, 270+x*16, 262+y*16);
          break;

        case '7':
          draw_xpm(sevenCell, 270+x*16, 262+y*16);
          break;

        case '8':
          draw_xpm(eightCell, 270+x*16, 262+y*16);
          break;
        
        case '*':
          draw_xpm(normalCell, 270+x*16, 262+y*16);
          break;

        case 'O':
          draw_xpm(mineCell, 270+x*16, 262+y*16);
          break;
        
        case 'f':
          draw_xpm(bannerCell, 270+x*16, 262+y*16);
          break;

        case 'F':
          draw_xpm(bannerCell, 270+x*16, 262+y*16);
          break;
        
        default:
          draw_xpm(normalCell, 270+x*16, 262+y*16);
          break;
      }
    }
  }
}

void draw_grid_20x16(char board[20][16]) {
  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 16; y++) {
      switch (board[x][y]) {
        case '0':
          draw_xpm(pressCell, 240+x*16, 250+y*16);
          break;

        case '1':
          draw_xpm(oneCell, 240+x*16, 250+y*16);
          break;
          
        case '2':
          draw_xpm(twoCell, 240+x*16, 250+y*16);
          break;

        case '3':
          draw_xpm(threeCell, 240+x*16, 250+y*16);
          break;

        case '4':
          draw_xpm(fourCell, 240+x*16, 250+y*16);
          break;

        case '5':
          draw_xpm(fiveCell, 240+x*16, 250+y*16);
          break;

        case '6':
          draw_xpm(sixCell, 240+x*16, 250+y*16);
          break;

        case '7':
          draw_xpm(sevenCell, 240+x*16, 250+y*16);
          break;

        case '8':
          draw_xpm(eightCell, 240+x*16, 250+y*16);
          break;
        
        case '*':
          draw_xpm(normalCell, 240+x*16, 250+y*16);
          break;

        case 'O':
          draw_xpm(mineCell, 240+x*16, 250+y*16);
          break;
        
        case 'f':
          draw_xpm(bannerCell, 240+x*16, 250+y*16);
          break;

        case 'F':
          draw_xpm(bannerCell, 240+x*16, 250+y*16);
          break;
        
        default:
          draw_xpm(normalCell, 240+x*16, 250+y*16);
          break;
      }
    }
  }
}

void draw_grid_28x20(char board[28][20]) {
  for (int x = 0; x < 28; x++) {
    for (int y = 0; y < 20; y++) {
      switch (board[x][y]) {
        case '0':
          draw_xpm(pressCell, 152+x*16, 270+y*16);
          break;

        case '1':
          draw_xpm(oneCell, 152+x*16, 270+y*16);
          break;
          
        case '2':
          draw_xpm(twoCell, 152+x*16, 270+y*16);
          break;

        case '3':
          draw_xpm(threeCell, 152+x*16, 270+y*16);
          break;

        case '4':
          draw_xpm(fourCell, 152+x*16, 270+y*16);
          break;

        case '5':
          draw_xpm(fiveCell, 152+x*16, 270+y*16);
          break;

        case '6':
          draw_xpm(sixCell, 152+x*16, 270+y*16);
          break;

        case '7':
          draw_xpm(sevenCell, 152+x*16, 270+y*16);
          break;

        case '8':
          draw_xpm(eightCell, 152+x*16, 270+y*16);
          break;
        
        case '*':
          draw_xpm(normalCell, 152+x*16, 270+y*16);
          break;

        case 'O':
          draw_xpm(mineCell, 152+x*16, 270+y*16);
          break;
        
        case 'f':
          draw_xpm(bannerCell, 152+x*16, 270+y*16);
          break;

        case 'F':
          draw_xpm(bannerCell, 152+x*16, 270+y*16);
          break;
        
        default:
          draw_xpm(normalCell, 152+x*16, 270+y*16);
          break;
      }
    }
  }
}

void draw_faceRestart_16x16() {
  draw_xpm(faceRestart, 357, 137);
}

void draw_faceRestart_20x16() {
  draw_xpm(faceRestart, 360, 114);
}

void draw_faceRestart_28x20() {
  draw_xpm(faceRestart, 337, 92);
}

void draw_pressCell(int x, int y) {
  draw_xpm(pressCell, x, y);
}

void draw_cursor(uint16_t x, uint16_t y) {
  draw_xpm(cursor, x, y);
}

void erase_cursor(uint16_t x, uint16_t y) {
  draw_xpm(empty_cursor, x, y);
}

void draw_instructions_menu() {
  draw_xpm(instructionsPage, 0, 0);
}

void draw_gameover_menu() {
  draw_xpm(gameOver, 0, 0);
}

void draw_number(int number, int x, int y) {
  switch (number) {
    case 0:
      draw_xpm(zero, x, y);
      break;
    case 1:
      draw_xpm(one, x, y);
      break;
    case 2:
      draw_xpm(two, x, y);
      break;
    case 3:
      draw_xpm(three, x, y);
      break;
    case 4:
      draw_xpm(four, x, y);
      break;
    case 5:
      draw_xpm(five, x, y);
      break;
    case 6:
      draw_xpm(six, x, y);
      break;
    case 7:
      draw_xpm(seven, x, y);
      break;
    case 8:
      draw_xpm(eight, x, y);
      break;
    case 9:
      draw_xpm(nine, x, y);
      break;
  }
}

void draw_colon(int x, int y) {
  draw_xpm(colon, x, y);
}

void draw_final_time(int m1, int m2, int s1, int s2) {
  draw_number(m1, 350, 320);
  draw_number(m2, 365, 320);
  draw_colon(378, 320);
  draw_number(s1, 391, 320);
  draw_number(s2, 406, 320);
}

void draw_win_menu() {
  draw_xpm(gameWin, 0, 0);
}

void draw_selected_cell(int x, int y) {
  draw_xpm(selected, x, y);
}
