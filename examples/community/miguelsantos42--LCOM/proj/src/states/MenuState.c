#include "src/include/states/MenuState.h"
#include "src/include/controllers/video/graphics.h"
#include "src/include/controllers/video/projXPM.h"
#include "src/include/controllers/rtc/rtc.h"
#include "src/include/states/GameState.h"
#include "src/include/Macros.h"
#include <string.h>

MenuState* menuState = NULL;

static xpm_image_t loaddedStart;
static xpm_image_t loaddedSettings;
static xpm_image_t loaddedExit;
// static xpm_image_t loaddedLeaderboard;
static xpm_image_t loaddedInstructions;
static xpm_image_t loaddedMenu;
static xpm_image_t loaddedNumbers[10];
static uint8_t* mappedNumbers[10] = {NULL};
static uint8_t* mappedStart = NULL;
static uint8_t* mappedSettings = NULL;
static uint8_t* mappedExit = NULL;
// static uint8_t* mappedLeaderboard = NULL;
static uint8_t* mappedInstructions = NULL;
static uint8_t* mappedMenu = NULL;

static xpm_image_t loaddedColon;
static xpm_image_t loaddedSlash;
static uint8_t* mappedColon = NULL;
static uint8_t* mappedSlash = NULL;

struct MenuStateData {
  MenuOption selectedOption;
  int startX;
  int startY;
  int exitX;
  int exitY;
};
  
typedef struct MenuStateData MenuStateData;

static void loadNumberSprites() {
  if (mappedColon == NULL) {
    load_xpm(XPMMapColon, &loaddedColon, &mappedColon);
  }
  
  if (mappedSlash == NULL) {
    load_xpm(XPMMapSlash, &loaddedSlash, &mappedSlash);
  }
  
  for (int i = 0; i < 10; i++) {
    if (mappedNumbers[i] == NULL) {
      const xpm_row_t *number_xpm;
      switch(i) {
        case 0: number_xpm = XPMMapNumber0; break;
        case 1: number_xpm = XPMMapNumber1; break;
        case 2: number_xpm = XPMMapNumber2; break;
        case 3: number_xpm = XPMMapNumber3; break;
        case 4: number_xpm = XPMMapNumber4; break;
        case 5: number_xpm = XPMMapNumber5; break;
        case 6: number_xpm = XPMMapNumber6; break;
        case 7: number_xpm = XPMMapNumber7; break;
        case 8: number_xpm = XPMMapNumber8; break;
        case 9: number_xpm = XPMMapNumber9; break;
        default: continue;
      }
      load_xpm(number_xpm, &loaddedNumbers[i], &mappedNumbers[i]);
    }
  }
}


static void drawColon(int x, int y) {
  if (mappedColon != NULL) {
    draw_xpm(loaddedColon, mappedColon, x, y);
  }
}

static void drawSlash(int x, int y) {
  if (mappedSlash != NULL) {
    draw_xpm(loaddedSlash, mappedSlash, x, y);
  }
}

static void drawNumber(int number, int x, int y) {
  if (number >= 0 && number <= 9 && mappedNumbers[number] != NULL) {
    draw_xpm(loaddedNumbers[number], mappedNumbers[number], x, y);
  }
}

static void drawTwoDigitNumber(int number, int x, int y) {
  int tens = number / 10;
  int units = number % 10;
  
  drawNumber(tens, x, y);
  drawNumber(units, x + 20, y); 
}

static void drawTimeAndDate(int x, int y) {
  rtc_time_t current_time;
  
  if (rtc_get_time(&current_time) == 0) {
    drawTwoDigitNumber(current_time.day, x, y);
    drawSlash(x + 40, y);
    drawTwoDigitNumber(current_time.month, x + 56, y);
    drawSlash(x + 96, y);
    drawTwoDigitNumber(current_time.year, x + 112, y);
    
    drawTwoDigitNumber(current_time.hours, x, y + 40);
    drawColon(x + 40, y + 40);
    drawTwoDigitNumber(current_time.minutes, x + 56, y + 40);
  }
}

static void destroyMenuData(MenuStateData* data) {
  if(data) free(data);
}

static void destroy(MenuState* state) {
  if(!state) return;
  MenuStateData* data = state->data;
  if(data) destroyMenuData(data);
  free(state);
}

static MenuOption getSelectedOption(MenuState* state) {
  MenuStateData* data;
  getData(state,data,-1);
  return data->selectedOption;
}

static int draw(MenuState* state) {
  reset_buffer();
  MenuStateData* data;
  getData(state,data,-1);
  if(mappedStart == NULL) load_xpm(XPMMapWordSTART, &loaddedStart, &mappedStart);
  if(mappedSettings == NULL) load_xpm(XPMMapWordSETTINGS, &loaddedSettings, &mappedSettings);
  if(mappedExit == NULL) load_xpm(XPMMapWordEXIT, &loaddedExit, &mappedExit);
  // if(mappedLeaderboard == NULL) load_xpm(XPMMapWordLEADERBOARD, &loaddedLeaderboard, &mappedLeaderboard);
  if(mappedInstructions == NULL) load_xpm(XPMMapWordINSTRUCTIONS, &loaddedInstructions, &mappedInstructions);
  if(mappedMenu == NULL) load_xpm(XPMMapWordMENU, &loaddedMenu, &mappedMenu);
  
  loadNumberSprites();

  draw_rectangle(data->startX - 5, data->startY - 10 + data->selectedOption*40, 190, 25, 13);

  draw_xpm(loaddedExit, mappedExit, data->startX, data->startY+120);
  draw_xpm(loaddedStart, mappedStart, data->startX, data->startY);
  draw_xpm(loaddedSettings, mappedSettings, data->startX, data->startY+80);
  draw_xpm(loaddedInstructions, mappedInstructions, data->startX, data->startY+40);
  // draw_xpm(loaddedLeaderboard, mappedLeaderboard, data->startX, data->startY+40);

  draw_xpm(loaddedMenu, mappedMenu, data->startX, data->startY-160);

  drawTimeAndDate(700, 50);

  return 0;
}

static void (setUp)(struct MenuState* state){
  MenuStateData* data;
  getVoidData(state,data);
  data->selectedOption = ((int)data->selectedOption-1+4)%4;
  state->draw(state);
}

static void (setDown)(struct MenuState* state){
  MenuStateData* data;
  getVoidData(state,data);
  data->selectedOption = ((int)data->selectedOption+1)%4;
  state->draw(state);
}

static void (setMouse)(struct MenuState* state, uint16_t x, uint16_t y){
  MenuStateData* data;
  getVoidData(state,data);

  const int x0 = data->startX - 5;
  const int x1 = x0 + 190;
  const int y0 = data->startY - 10;
  const int boxH = 25;
  const int stepY = 40;

  for (int i = 0; i < 4; i++) {
    int Y0 = y0 + i * stepY;
    int Y1 = Y0 + boxH;
    if (x >= x0 && x <= x1 && y >= y0 && y <= Y1) {
        if (data->selectedOption != (MenuOption)i) {
              data->selectedOption = (MenuOption)i;
              state->draw(state);
          }
          return;
        }
    }

}

static void (menuSelect)(struct MenuState* state){
  MenuStateData* data;
  getVoidData(state,data);
  switch (data->selectedOption)
  {
  case MenuOptionStart:
    gameState = GameRunning;
    break;
  case MenuOptionExit:
    gameState = GameStopping;
    break;
  case MenuOptionInstructions:
    gameState = GameInstructions;
    break;
  case MenuOptionSettings:
    gameState = GameSettings;
    break;
  default:
    break;
  }
}

MenuState* menuStateConstructor() {
  MenuState* state = malloc(sizeof(MenuState));
  if(!state) return NULL;

  MenuStateData* data = malloc(sizeof(MenuStateData));
  if(!data) {
    free(state);
    return NULL;
  }

  data->selectedOption = MenuOptionStart;
  data->startX = 450;
  data->startY = 350;

  data->exitX = 450;
  data->exitY = 500;

  state->data = data;
  state->draw = draw;
  state->setUp = setUp;
  state->setDown = setDown;
  state->setMouse = setMouse;
  state->select = menuSelect;
  state->getSelectedOption = getSelectedOption;
  state->destroy = destroy;

  return state;
}

