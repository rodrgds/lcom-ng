#include "src/include/states/SettingsState.h"
#include "src/include/Macros.h"
#include "src/include/controllers/video/projXPM.h"
#include "src/include/controllers/video/graphics.h"
#include "src/include/states/GameState.h"
#include "src/include/states/CharacterState.h"

SettingsState* settingsState = NULL;
Difficulty selectedDifficulty = Easy;

static xpm_image_t loaddedEasy;
static xpm_image_t loaddedMedium;
static xpm_image_t loaddedHard;
static xpm_image_t loaddedSettings;

static uint8_t* mappedEasy;
static uint8_t* mappedMedium;
static uint8_t* mappedHard;
static uint8_t* mappedSettings;

struct SettingsStateData {
  Difficulty selectedOption;
  int startX;
  int startY;
};

typedef struct SettingsStateData SettingsStateData;

static void destroySettingsData(SettingsStateData* data){
  if(data) free(data);
}

static void destroy(SettingsState* state){
  if(!state) return;
  SettingsStateData* data = state->data;
  destroySettingsData(data);
  free(state);
}

static Difficulty getSelectedDifficulty(SettingsState* state){ 
  SettingsStateData* data;
  getData(state, data, -1);
  return data->selectedOption;
}

static int draw(SettingsState* state) {
  reset_buffer();
  SettingsStateData* data;
  getData(state,data,-1);

  if(mappedEasy == NULL) load_xpm(XPMMapWordEASY, &loaddedEasy, &mappedEasy);
  if(mappedMedium == NULL) load_xpm(XPMMapWordMEDIUM, &loaddedMedium, &mappedMedium);
  if(mappedHard == NULL) load_xpm(XPMMapWordHARD, &loaddedHard, &mappedHard);
  if(mappedSettings == NULL) load_xpm(XPMMapWordSETTINGS, &loaddedSettings, &mappedSettings);

  draw_rectangle(data->startX - 5, data->startY - 10 + data->selectedOption*40, 120, 25, 13);

  draw_xpm(loaddedEasy, mappedEasy, data->startX, data->startY);
  draw_xpm(loaddedMedium, mappedMedium, data->startX, data->startY + 40);
  draw_xpm(loaddedHard, mappedHard, data->startX, data->startY+80);

  draw_xpm(loaddedSettings, mappedSettings, data->startX, data->startY-160);

  return 0;
}

static void (setUp)(struct SettingsState* state){
  SettingsStateData* data;
  getVoidData(state,data);
  data->selectedOption = ((int)data->selectedOption-1+3)%3;
  state->draw(state);
}
static void (setDown)(struct SettingsState* state){
  SettingsStateData* data;
  getVoidData(state,data);
  data->selectedOption = ((int)data->selectedOption+1)%3;
  state->draw(state);
}

static void (setMouse)(struct SettingsState* state, uint16_t x, uint16_t y){
  SettingsStateData* data;
  getVoidData(state,data);

  const int x0 = data->startX - 5;
  const int x1 = x0 + 120;
  const int y0 = data->startY - 10;
  const int boxH = 25;
  const int stepY = 40;

  for (int i = 0; i < 4; i++) {
    int Y0 = y0 + i * stepY;
    int Y1 = Y0 + boxH;
    if (x >= x0 && x <= x1 && y >= y0 && y <= Y1) {
      if (data->selectedOption != (Difficulty)i) {
        data->selectedOption = (Difficulty)i;
        state->draw(state);
      }
      return;
    }
  }

}

static void (selectOption)(struct SettingsState* state) {
  SettingsStateData* data;
  getVoidData(state, data);

  Difficulty previousDifficulty = selectedDifficulty;

  selectedDifficulty = data->selectedOption;

  if(previousDifficulty != selectedDifficulty) {
    reloadMap(getMapByDifficulty(selectedDifficulty));
    gameState = GameRunning;
  }
  
  gameState = GameMenu;
}

SettingsState* settingsStateConstructor() {
  SettingsState* state = malloc(sizeof(SettingsState));
  SettingsStateData* data = malloc(sizeof(SettingsStateData));

  data->selectedOption = selectedDifficulty;
  data->startX = 450;
  data->startY = 350;

  state->data = data;
  state->draw = draw;
  state->setUp = setUp;
  state->setDown = setDown;
  state->setMouse = setMouse;
  state->select = selectOption;
  state->getSelectedDifficulty = getSelectedDifficulty;
  state->destroy = destroy;

  return state;
}

char* getMapByDifficulty(Difficulty difficulty) {
  switch (difficulty){
    case Easy:
      return "/home/lcom/labs/proj/resources/levels/level1Easy.lvl";
      break;
    case Medium:
      return "/home/lcom/labs/proj/resources/levels/level2Medium.lvl";
      break;
    case Hard:
      return "/home/lcom/labs/proj/resources/levels/level3Hard.lvl";
      break;
    default:
      return "/home/lcom/labs/proj/resources/levels/level2Medium.lvl";
      break;
  }
}

