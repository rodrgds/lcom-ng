#include "gameView.h"

void drawGameBoardToBackground() {
    for (uint8_t x = 0; x < 15; x++) {
        for (uint8_t y = 0; y < 15; y++) {
            drawRectangleToBackground(gameBoard[x][y].color, gameBoard[x][y].pos.x, gameBoard[x][y].pos.y, 40, 40);
        }
    }
}

void drawGameSideBar() {
    drawSpriteToBackground(side_bar, 600, 0);
}

void drawFlag(uint8_t s_col, uint8_t s_row) {
    drawSpriteToBackground(flag, s_col * 40, s_row * 40);
}

void loadGameSprites() {
    flag = generate_Sprite((xpm_map_t) GameFlag);
    mine = generate_Sprite((xpm_map_t) GameMine);
    small_digits[0] = generate_Sprite((xpm_map_t) S_One);
    small_digits[1] = generate_Sprite((xpm_map_t) S_Two);
    small_digits[2] = generate_Sprite((xpm_map_t) S_Three);
    small_digits[3] = generate_Sprite((xpm_map_t) S_Four); 
    small_digits[4] = generate_Sprite((xpm_map_t) S_Five);
    small_digits[5] = generate_Sprite((xpm_map_t) S_Six);
    small_digits[6] = generate_Sprite((xpm_map_t) S_Seven);
    small_digits[7] = generate_Sprite((xpm_map_t) S_Eigth);
    time_digits[0] = generate_Sprite((xpm_map_t) Zero);
    time_digits[1] = generate_Sprite((xpm_map_t) One);
    time_digits[2] = generate_Sprite((xpm_map_t) Two);
    time_digits[3] = generate_Sprite((xpm_map_t) Three);
    time_digits[4] = generate_Sprite((xpm_map_t) Four);
    time_digits[5] = generate_Sprite((xpm_map_t) Five);
    time_digits[6] = generate_Sprite((xpm_map_t) Six);
    time_digits[7] = generate_Sprite((xpm_map_t) Seven);
    time_digits[8] = generate_Sprite((xpm_map_t) Eigth);
    time_digits[9] = generate_Sprite((xpm_map_t) Nine);
    side_bar = generate_Sprite((xpm_map_t) GameSideBar);
}

void DrawMine(uint8_t s_col, uint8_t s_row) {
    DrawDig(s_col, s_row);
    drawSpriteToBackground(mine, s_col * 40, s_row * 40);
}

void DrawProximityCounter(uint8_t s_col, uint8_t s_row) {
    drawSpriteToBackground(small_digits[(gameBoard[s_col][s_row].nCloseMines) - 1], (s_col * 40) + 13, (s_row * 40) + 10);
}

void DrawFlagCounter() {
    uint8_t first_digit_flag = flags / 10;
    uint8_t second_digit_flag = flags % 10;
    draw_Sprite(time_digits[first_digit_flag], 643, 325);
    draw_Sprite(time_digits[second_digit_flag], 683, 325);
    drawSpriteToBackground(flag, 723, 320);
}

void DrawDig(uint8_t s_col, uint8_t s_row) {
    drawRectangleToBackground(gameBoard[s_col][s_row].dirt_color, gameBoard[s_col][s_row].pos.x, gameBoard[s_col][s_row].pos.y, 40, 40);
}

void DrawTime(uint16_t time) {
    uint8_t first_digit = time / 100;
    uint8_t second_digit = (time / 10) % 10;
    uint8_t third_digit = time % 10;
    draw_Sprite(time_digits[first_digit], 643, 225);
    draw_Sprite(time_digits[second_digit], 683, 225);
    draw_Sprite(time_digits[third_digit], 723, 225);
}


