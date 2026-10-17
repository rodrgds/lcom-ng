#include "view_menu.h"

int drawMainMenu() {
  
  if (drawSprite(background, SCREEN_START_X, SCREEN_START_Y)) return 1;
  if (drawButton(Start, StartPressed, START_BUTTON_X, START_BUTTON_Y)) return 1;
  if (drawButton(Exit, ExitPressed, EXIT_BUTTON_X, EXIT_BUTTON_Y)) return 1;
  if (drawButton(table, table_pressed, TABLE_BUTTON_X, TABLE_BUTTON_Y)) return 1;
  if (drawButton(multiplayer_not, multiplayer, MULTIPLAYER_BUTTON_X, MULTIPLAYER_BUTTON_Y)) return 1;
  if (drawCursor(0)) return 1;

  return 0;
}

int drawResultWithSprite(Sprite* sprite) {
  clearBuffer();
  if (drawSprite(sprite, SCREEN_START_X, SCREEN_START_Y)) return 1;
  char sum[5];
  snprintf(sum, sizeof(sum), "%04d", p1.money);
  if (drawText(MAX_WIDTH / 2, MAX_HEIGHT / 2, sum)) return 1;
  return 0;
}

int drawLoose() {
  if (drawCursor(0)) return 1;
  return drawResultWithSprite(Loss);
}

int drawWin() {
  if (drawCursor(0)) return 1;
  return drawResultWithSprite(Win);
}

int drawNoWinner() {
  if (drawCursor(0)) return 1;
  return drawResultWithSprite(Tie);
}

int draw_game(time_difference diff, const char name[MAX_LENGTH], int money, int x, int y) {
  drawText(x, y, name);
  x += MONEY_TEXT_X - NAME_TEXT_X;
  char _money[4];
  snprintf(_money, sizeof(_money), "%03d", money);
  drawText(x, y, _money);
  x += TIME_TEXT_X - MONEY_TEXT_X;

  char time[3];

  snprintf(time, sizeof(time), "%02d", diff.months);
  drawText(x, y, time);
  x += DIGIT_SPACE * 2;

  drawSprite(doubleDot, x, y);
  x += DIGIT_SPACE;

  snprintf(time, sizeof(time), "%02d", diff.days);
  drawText(x, y, time);
  x += DIGIT_SPACE * 2;

  drawSprite(doubleDot, x, y);
  x += DIGIT_SPACE;

  snprintf(time, sizeof(time), "%02d", diff.hours);
  drawText(x, y, time);
  x += DIGIT_SPACE * 2;

  drawSprite(doubleDot, x, y);
  x += DIGIT_SPACE;

  snprintf(time, sizeof(time), "%02d", diff.minutes);
  drawText(x, y, time);
  x += DIGIT_SPACE * 2;

  if(strcmp(name, p1.name) == 0){
    drawSprite(red_cross, x + 20, y - 10);
  }

  return 0;
}

int drawTable() {
  drawSprite(background, SCREEN_START_X, SCREEN_START_Y);
  drawText(NAME_TEXT_X, NAME_TEXT_Y, "name");
  drawText(MONEY_TEXT_X, NAME_TEXT_Y, "money");
  drawText(TIME_TEXT_X, NAME_TEXT_Y, "time");
  vg_draw_hline(NAME_TEXT_X, HLINE_Y, HLINE_LENGTH, LINE_COLOR);
  vg_draw_vline(VLINE1_X, NAME_TEXT_Y, VLINE_LENGTH, LINE_COLOR);
  vg_draw_vline(VLINE2_X, NAME_TEXT_Y, VLINE_LENGTH, LINE_COLOR);
  drawSprite(refrashe, REFRESH_SPRITE_X, REFRESH_SPRITE_Y);
  drawSprite(edit_icon, EDIT_ICON_X, EDIT_ICON_Y);

  int x = INITIAL_DRAW_GAME_X, y = INITIAL_DRAW_GAME_Y;

  for (int i = 0; i < person_count && i < MAX_PERSONS_DRAW; i++) {
    time_difference diff = calculate_time_difference(data, persons[i].time);
    if (diff.months != -1) {
      draw_game(diff, persons[i].name, persons[i].money, x, y);
      y += DRAW_GAME_Y_INCREMENT;
    }
  }

  return 0;
}

int drawTableDigit(){
  int x = TABLE_DIGIT_INITIAL_X, y = TABLE_DIGIT_INITIAL_Y;

  char name[MAX_LENGTH];

  // Extract name
  int i = 0, len = 0;
  while (to_save[i] != '\0' && isl(to_save[i])) {
    name[len++] = to_save[i++];
  }
  name[len] = '\0';
  drawText(x, y, name);
  x += LETTER_SPACE * strlen(name);

  len = 0;

  x = (x > 100) ? (x - 100) : TABLE_DIGIT_NUM_SPACE;
  y += TABLE_DIGIT_Y_INCREMENT;

  while(to_save[i] != '\0' && to_save[i] >= '0' && to_save[i] <= '9'){
    drawSprite(numbers[to_save[i++] - '0'], x, y); len++;
    x += LETTER_SPACE;
    if(len % 2 == 0){
      drawSprite(doubleDot, x, y);
      x += TABLE_DIGIT_NUM_SPACE;
    }
    if(len == 10){
      break;
    }
  }

  while(to_save[i] != '\0' && isd(to_save[i])){
    drawSprite(numbers[to_save[i++] - '0'], x, y);
    x += LETTER_SPACE;
  }
    
  uint8_t name_size = strlen(to_save);
  if( (isl(to_save[name_size - 1])) || name_size == 0 ){
    vg_draw_rectangle(LETTER_SPACE * name_size + TABLE_DIGIT_INITIAL_X, 
                      y - TABLE_DIGIT_Y_INCREMENT, 
                      SPACE, LETTER_HEIGHT, WIGHT);
  }else{
    if(x >= 0) vg_draw_rectangle(x, y, SPACE, LETTER_HEIGHT, WIGHT);
  }
    
  return 0;
}

int draw_wait(){
  if (drawSprite(background, SCREEN_START_X, SCREEN_START_Y)) return 1;
  if (drawSprite(wait, WAIT_SPRITE_X, WAIT_SPRITE_Y)) return 1;
  return 0;
}
