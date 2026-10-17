#include "menu.h"

int currentOption = 0;

/**
 * GETTERS AND SETTERS OF MENU VARIABLES
 */

int (getCurrentOption)() {
  return currentOption;
}

void (setCurrentOption)(int option) {
  currentOption = option;
}

void(increaseCurrentOption)() {
  if (currentOption >= 2) {
    currentOption = 0;
  }
  else {
    currentOption++;
  }

}

void(decreaseCurrentOption)() {
  if (currentOption <= 0) {
    currentOption = 2;
  }
  else {
    currentOption--;
  }
}

/**
 * TIME FUNCTION
 */

int (readTime)(rtc_time *timeRTC) {

  int tempHours = timeRTC->hours;
  int tempMinutes = timeRTC->minutes;

  for (int i = 0; i < 2; i++) {
    uint8_t digit = tempHours % 10;
    tempHours /= 10;
    hours[i] = numbers[digit];
  }
  for (int i = 0; i < 2; i++) {
    uint8_t digit = tempMinutes % 10;
    tempMinutes /= 10;
    minutes[i] = numbers[digit];
  }
  return EXIT_SUCCESS;
}

void (draw_menu)(bool isDay) {

  if (isDay) {
    (draw_background)(dayDesert->map);
  }
  else {
    (draw_background)(nightDesert->map);
  }

  int startX = MAX_X - 170;

  for (int i = 0; i < 2; i++) {
    (draw_sprite)(hours[i], startX, MAX_Y - 65);
    startX -= 50;
  }

  (draw_sprite)(dots, startX + 145, MAX_Y - 65);

  startX = MAX_X - 30;
  for (int i = 0; i < 2; i++) {
    (draw_sprite)(minutes[i], startX, MAX_Y - 65);
    startX -= 50;
  }
  draw_sprite(title, MAX_X / 2, 150);
  draw_sprite(singleplayer, MAX_X / 2, 400);
  draw_sprite(multiplayer, MAX_X / 2, 500);
  draw_sprite(quit, MAX_X / 2, 600);

  switch (getCurrentOption()) {
    case SINGLEPLAYER:
      vg_draw_rectangle(MAX_X / 2 - 250, 440, 502, 6, 0x000000);
      break;
    case MULTIPLAYER:
      vg_draw_rectangle(MAX_X / 2 - 240, 540, 480, 6, 0x000000);
      break;
    case QUIT:
      vg_draw_rectangle(MAX_X / 2 - 85, 640, 170, 6, 0x000000);
      break;
    default:
      break;
  }

  vg_page_flipping();
}
