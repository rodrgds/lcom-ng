#include "view.h"

int drawButton(Sprite *normalSprite, Sprite *pressedSprite, int xStart, int yStart){
  int x = mouse_loc.x;
  int y = mouse_loc.y;

  bool selected = (x > xStart && x < xStart + normalSprite->width && y > yStart && y < yStart + normalSprite->height);

  Sprite *spriteToDraw = selected ? pressedSprite : normalSprite;

  // Draw the sprite
  return drawSprite(spriteToDraw, xStart, yStart);
}

int drawCursor(int f) {
  bool isMouseInside =  (mouse_loc.x > COINS_AREA_LEFT) && 
                        (mouse_loc.x < COINS_AREA_RIGHT) &&
                        (mouse_loc.y > COINS_AREA_TOP) && 
                        (mouse_loc.y < COINS_AREA_BOTTOM);
  bool isMousePressed = mouse_data.lb || mouse_data.rb || mouse_data.mb;

  Sprite* spriteToDraw;
  if ((isMouseInside && f) || (f == 2)) {
    spriteToDraw = isMousePressed ? handCloused : hand;
  }
  else {
    spriteToDraw = isMousePressed ? cursorClick : cursor;
  }

  if (drawSprite(spriteToDraw, mouse_loc.x, mouse_loc.y)) {
    return 1;
  }

  return 0;
}

int drawText(int x, int y, const char text[MAX_LENGTH]) {
  //Sprite* targetSprite;

  for (size_t i = 0; text[i] != '\0'; ++i) {
    if (text[i] >= 'a' && text[i] <= 'z') {
      if (drawSprite(letters[text[i] - 'a'], x, y)) return 1;
    }
    else {
      if (drawSprite(numbers[text[i] - '0'], x, y)) return 1;
    }
    x += LETTER_SPACE;
  }

  return 0;
}

int drawTime(int x, int y, int sec) {
  int minutes = sec / 60;
  int seconds = sec % 60;
  char time[3];
  snprintf(time, sizeof(time), "%02d", minutes);

  if (drawText(x, y, time)) return 1;
  x += 2 * LETTER_SPACE;

  if (drawSprite(doubleDot, x, y)) return 1;
  x += SPACE;

  snprintf(time, sizeof(time), "%02d", seconds);
  if (drawText(x, y, time)) return 1;
  
  return 0;
}
