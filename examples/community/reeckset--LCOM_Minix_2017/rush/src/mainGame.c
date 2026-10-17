#include "mainGame.h"
#include "macros.h"
#include "video_gr.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "rtc.h"
#include "textRender.h"

#define g_maxRoadSpeed 8;
static double lastScore = 0;
static double score = 0;
static double scoreSpeed = 1;
static bool isNight = false;
static bool wasNight = false;
static bool wasPlayBtnHovered = false;
static bool wasMenuBtnHovered = false;
static double g_roadSpeed = g_maxRoadSpeed;
static double prevRoadSpeed = g_maxRoadSpeed;
static double g_collidedRoadSpeed = 1;
static double mouseSensitivity = 0.7;
static double startY = 100;
static double mainCarDesacceleration = 1.8;
static double carBoostSpeed = 2;
static double g_hitStrength = 0.6;
static int g_roadLimitLeft = 390;
static int g_roadLimitRight = 635;
static char* g_titleBmp = "/home/rush/res/title.bmp";
static char* g_roadBmp = "/home/rush/res/road.bmp";
static char* g_carBmpNorth = "/home/rush/res/Audi.bmp";
static char* g_carBmpSouth = "/home/rush/res/Audi.bmp";
static char* g_carMainBmp = "/home/rush/res/Ambulance.bmp";
static short g_carTransparencyColor = 0xf81f;
static char* g_exitBtnBmp = "/home/rush/res/exit.bmp";
static char* g_pauseBackgroundBmp = "/home/rush/res/pause.bmp";
static char* g_btnBmp = "/home/rush/res/play-btn.bmp";
static char* g_btnHoverBmp = "/home/rush/res/play-btn-hovered.bmp";
static char* g_btnDayBmp = "/home/rush/res/play-btn.bmp";
static char* g_btnHoverDayBmp = "/home/rush/res/play-btn-hovered.bmp";
static char* g_btnNightBmp = "/home/rush/res/play-btn-night.bmp";
static char* g_btnHoverNightBmp = "/home/rush/res/play-btn-hovered-night.bmp";
static char* g_menuBackgroundBmp = "/home/rush/res/menu-bg-day.bmp";
static char* g_menuBackgroundDayBmp = "/home/rush/res/menu-bg-day.bmp";
static char* g_menuBackgroundNightBmp = "/home/rush/res/menu-bg-night.bmp";
static char* g_cursorBmp = "/home/rush/res/cursor.bmp";
static char* g_gameOverBackgroundBmp = "home/rush/res/gameOver_bg.bmp";
static char* g_mainMenuBtnBmp = "home/rush/res/mainMenuBtn.bmp";
static char* g_mainMenuBtnHoveredBmp = "home/rush/res/mainMenuBtn_hovered.bmp";
static char* g_heartBmp = "home/rush/res/heart.bmp";

static int dayStartTime = 7;
static int dayEndTime = 19;

void startup(ev_queue_st* events, gameState_st* startState){
  //MAIN GAME------------------------------------------------------------------------------------------

  printf("Startup\n");

    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 577, 0, 0, 4, g_carBmpNorth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 577, 0 - (int) getVerResolution(), 0, 4, g_carBmpNorth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 527, 0, 0, 3, g_carBmpNorth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 527, 0 - (int) getVerResolution(), 0, 3, g_carBmpNorth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 457, 0, 0, 2.5, g_carBmpSouth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 457, 0 - (int) getVerResolution(), 0, 2.5f, g_carBmpSouth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 397, 0, 0, 5, g_carBmpSouth, g_carTransparencyColor));
    zIndexPush(getState(GAME_PLAY)->objects, 1, createGameObject("car", 397, 0 - (int) getVerResolution(), 0, 5, g_carBmpSouth, g_carTransparencyColor));

    printf("startup:: pushed objects for gameplay state (cars)\n");
    //Main Character

    zIndexPush(getState(GAME_PLAY)->objects, 2, createGameObject("mainCar", getHorResolution() / 2, (int) getVerResolution() - startY, 0, 0, g_carMainBmp, g_carTransparencyColor));
    printf("startup:: pushed objects for gameplay state(main car)\n");

    printf("StartState objs size: %d\n",startState->objects->size);

    //Road
    GameObject road = createGameObject("road", 0, 0, 0, g_roadSpeed, g_roadBmp, 0);
    printf("Adding road1\n");
    zIndexPush(getState(GAME_PLAY)->objects, 0, road);
    GameObject road2 = createGameObject("road", 0, 0 - (int) getVerResolution(), 0, g_roadSpeed, g_roadBmp, 0);
    printf("Adding road2\n");
    zIndexPush(getState(GAME_PLAY)->objects, 0, road2);

    printf("startup:: pushed objects for gameplay state(road)\n");

    //PAUSE MENU-----------------------------------------------------------------------------------------
    GameObject pauseMenuObj = createGameObject("PAUSE", 0, 0, 0, 0, g_pauseBackgroundBmp, 0);
    zIndexPush(getState(PAUSE_MENU)->objects, 0, pauseMenuObj);
    printf("startup:: pushed objects for pause state\n");

    //MAIN MENU-----------------------------------------------------------------------------------------
    zIndexPush(getState(MAIN_MENU)->objects, 0, createGameObject("MainMenu_bg", 0, 0, 0, 0, g_menuBackgroundDayBmp, 0));

    zIndexPush(getState(MAIN_MENU)->objects, 1, createGameObject("play-btn", 262, 434, 0, 0, g_btnBmp, g_carTransparencyColor));

    zIndexPush(getState(MAIN_MENU)->objects, 1, createGameObject("exit-btn", 924, 700, 0, 0, g_exitBtnBmp, g_carTransparencyColor));
    zIndexPush(getState(MAIN_MENU)->objects, 1, createGameObject("title-logo", 112, 100, 0, 0, g_titleBmp, g_carTransparencyColor));

    GameObject mainMenuCursor = createGameObject("MainMenu_cursor", 0, 0, 0, 0, g_cursorBmp, g_carTransparencyColor);
    zIndexPush(getState(MAIN_MENU)->objects, 2, mainMenuCursor);
    printf("startup:: pushed objects for mainmenu state\n");

    //GAME OVER-----------------------------------------------------------------------------------------
    zIndexPush(getState(GAME_OVER)->objects, 0, createGameObject("GameOver_bg", 0, 0, 0, 0, g_gameOverBackgroundBmp, 0));

    zIndexPush(getState(GAME_OVER)->objects, 1, createGameObject("menu-btn", 305, 500, 0, 0, g_mainMenuBtnBmp, g_carTransparencyColor));

    zIndexPush(getState(GAME_OVER)->objects, 1, createGameObject("exit-btn", 924, 700, 0, 0, g_exitBtnBmp, g_carTransparencyColor));

    GameObject gameOverCursor = createGameObject("GameOver_cursor", 0, 0, 0, 0, g_cursorBmp, g_carTransparencyColor);
    zIndexPush(getState(GAME_OVER)->objects, 2, gameOverCursor);
    printf("startup:: pushed objects for gameover state\n");

    displayHP(MAX_HP, getState(GAME_PLAY)->objects);
}

void update(ev_queue_st* events, gameState_st* currentState){
    updatePositions(currentState); //update objects' positions based on their speed

    //Night/Day cycle based on rtc
    unsigned long hours;
  	rtc_getHours(&hours);
    hours = convertBCDtoBinary(hours);
  	if(hours >= dayStartTime && hours < dayEndTime) isNight = false;
    else isNight = true;

    //MAIN GAME
    if(currentState->state == GAME_PLAY){
      score += 0.01 * scoreSpeed;
      clearText(currentState->objects);
      renderNumber(currentState->objects, (int) score, 950, 10, ALIGN_RIGHT);
      //printf("CURRENT STATE IS GAME_PLAY\n");
      double maxRoadSpeed = g_maxRoadSpeed;
      if(g_roadSpeed < maxRoadSpeed){
           g_roadSpeed += 0.05;
      }
      else g_roadSpeed = g_maxRoadSpeed;
      //Cars
      int i;
      for(i = 0; i < currentState->objects[1].size; i++){
        GameObject *g = &(currentState->objects[1].gameObjects[i]);
        if(g->y > (int) getVerResolution()){
          GameObject car = createGameObject("car", g->x, g->y - 2* (int) getVerResolution(), 0, g->vy, g->bmpPath, g->transparentColor);
          zIndexPush(currentState->objects, 1, car);
          zIndexRemove(currentState->objects, g);
        }
      }
      //ROAD
      for(i = 0; i < currentState->objects[0].size; i++){
        GameObject *g = &(currentState->objects[0].gameObjects[i]);
        if(g->y > (int) getVerResolution()){
          g->y -= 2* (int) getVerResolution();
        }
        g->vy = g_roadSpeed;
      }

      //Clean propCar's
      if(getObjectFromLabel(currentState->objects, "propCar") != NULL){
        GameObject* g = getObjectFromLabel(currentState->objects, "propCar");
        if(g->x > (int) getHorResolution() || g->y > (int) getVerResolution() || g->x + g->sizeX < 0 || g->y + g->sizeY < 0)
        zIndexRemove(currentState->objects, g);
      }

      //KEYBOARD
      if(currentState->isKeyDown_SPACE) { // If user is pressing space the car is boosted
        //printf("SPACE_DOWN\n");
        getObjectFromLabel(currentState->objects, "mainCar")->vy = -carBoostSpeed;
      } else { // otherwise, if the car is in original position its velocity is 0, but if it's not, it desaccelerates until it gets there
        //printf("SPACE_UP\n");
        if(getObjectFromLabel(currentState->objects, "mainCar")->y < (int) getVerResolution() - startY){
          getObjectFromLabel(currentState->objects, "mainCar")->vy = mainCarDesacceleration;
        } else {
          getObjectFromLabel(currentState->objects, "mainCar")->vy = 0;
        }
      }
      checkCollisions(currentState->objects);

      //Change cars' speed according to g_roadSpeed
      if(g_roadSpeed - prevRoadSpeed != 0){
        int a;
        for(a = 0; a < currentState->objects[1].size; a++){
          currentState->objects[1].gameObjects[a].vy -= prevRoadSpeed - g_roadSpeed;
        }
      }
      prevRoadSpeed = g_roadSpeed;
      //Limit car
      GameObject* mainCar = getObjectFromLabel(currentState->objects, "mainCar");
      if(mainCar->x < g_roadLimitLeft) mainCar->x = g_roadLimitLeft;
      if(mainCar->x + mainCar->sizeX > g_roadLimitRight) mainCar->x = g_roadLimitRight - mainCar->sizeX;
      if(mainCar->y < 0) mainCar->y = 0;

      event_st* noHP_ev = createEvent(NO_HP);
      if(currentState->currentHP < 1){
        ev_queue_push(events, *noHP_ev);
        clearText(getState(GAME_OVER)->objects);
        renderNumber(getState(GAME_OVER)->objects, score, 500, 400, ALIGN_RIGHT);
      }

    }
    //MAIN MENUf
    else if(currentState->state == MAIN_MENU){
      //Change Image Paths according to time of day
      GameObject* playBtn = getObjectFromLabel(currentState->objects, "play-btn");
      bool hoverPlayBtn = cursorIsInside(getObjectFromLabel(currentState->objects, "MainMenu_cursor"), playBtn);
      if((!isNight && wasNight) || (isNight && !wasNight)){
        if(isNight && !wasNight){ //make Day
          g_btnBmp = g_btnNightBmp;
          g_btnHoverBmp = g_btnHoverNightBmp;
          g_menuBackgroundBmp = g_menuBackgroundNightBmp;
          wasNight = true;
        }
        else if (!isNight && wasNight){ //make Night
          g_btnBmp = g_btnDayBmp;
          g_btnHoverBmp = g_btnHoverDayBmp;
          g_menuBackgroundBmp = g_menuBackgroundDayBmp;
          wasNight = false;
        }
        //Apply Day Changes
        GameObject* bg = getObjectFromLabel(currentState->objects, "MainMenu_bg");
        zIndexPush(getState(MAIN_MENU)->objects, bg->zIndex, createGameObject(bg->label, bg->x, bg->y, 0, 0, g_menuBackgroundBmp, g_carTransparencyColor));
        zIndexRemove(currentState->objects, bg);
        //if(!hoverUL && !hoverUR && !hoverBL && !hoverBR) printf("cursor not hovering obj\n");
      }
      if(hoverPlayBtn && !wasPlayBtnHovered){
        zIndexPush(getState(MAIN_MENU)->objects, playBtn->zIndex, createGameObject("play-btn", playBtn->x, playBtn->y, 0, 0, g_btnHoverBmp, g_carTransparencyColor));
        zIndexRemove(currentState->objects, playBtn);
        wasPlayBtnHovered = true;
      }else if(!hoverPlayBtn && wasPlayBtnHovered){
        zIndexPush(getState(MAIN_MENU)->objects, playBtn->zIndex, createGameObject("play-btn", playBtn->x, playBtn->y, 0, 0, g_btnBmp, g_carTransparencyColor));
        zIndexRemove(currentState->objects, playBtn);
        wasPlayBtnHovered = false;
      }

    }
    else if(currentState->state == GAME_OVER){
        GameObject* btn = getObjectFromLabel(currentState->objects, "menu-btn");
        bool hoverBtn = cursorIsInside(getObjectFromLabel(currentState->objects, "GameOver_cursor"), btn);
        if(hoverBtn && !wasMenuBtnHovered){
          zIndexPush(getState(GAME_OVER)->objects, btn->zIndex, createGameObject("menu-btn", btn->x, btn->y, 0, 0, g_mainMenuBtnHoveredBmp, g_carTransparencyColor));
          zIndexRemove(currentState->objects, btn);
          wasMenuBtnHovered = true;
        }else if(!hoverBtn && wasMenuBtnHovered){
          zIndexPush(getState(GAME_OVER)->objects, btn->zIndex, createGameObject("menu-btn", btn->x, btn->y, 0, 0, g_mainMenuBtnBmp, g_carTransparencyColor));
          zIndexRemove(currentState->objects, btn);
          wasMenuBtnHovered = false;
        }
    }

}

void onKbdEvent(gameState_st* currentState, event_st event, ev_queue_st* gameEvents){
  switch(currentState->state){
    case GAME_PLAY:
      //KEYBOARD TRIGGERED CHANGES IN GAME_PLAY
      switch(event.keyCode){
        case KEY_ESC:
          updateCurrentGameState(PAUSE_MENU);
        break;
        case KEY_SPACE_DOWN:
          printf("changing space bool to down\n");
          currentState->isKeyDown_SPACE = true;
          //getObjectFromLabel(currentState->objects, "mainCar")->vy = -carBoostSpeed;
        break;
        case KEY_SPACE_UP:
          currentState->isKeyDown_SPACE = false;
          //getObjectFromLabel(currentState->objects, "mainCar")->vy = -carBoostSpeed;
        break;
        default:
        break;
      }
      break;
    case PAUSE_MENU:
      switch(event.keyCode){
        case KEY_ESC:
          updateCurrentGameState(GAME_PLAY);
          break;
        case KEY_SPACE_UP:
          updateCurrentGameState(MAIN_MENU);
          resetGame(gameEvents);
          //
          // ev_queue_free(gameEvents);
          // stateMachine_free();
          // ev_queue_st* newGameEvents = ev_queue_create();
          // startStates(newGameEvents, MAX_HP);
          break;
        default:
          break;
      }
      break;
    case MAIN_MENU:
      switch(event.keyCode){
        case KEY_ESC:
          updateCurrentGameState(GAME_EXIT);
          break;
        case KEY_SPACE_UP:
          updateCurrentGameState(GAME_PLAY);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}

void onMouseEvent(gameState_st* currentState, event_st event, ev_queue_st* gameEvents){
  int x = event.mouseX;
  int y = event.mouseY;
  bool lmb = event.lmb;
  bool rmb = event.rmb;

  switch(currentState->state){
    case GAME_PLAY:
      if(x != 0){
        //MOVE CAR
        //currentState->objects[2].gameObjects[i].vx += (double)x/100;
        getObjectFromLabel(currentState->objects, "mainCar")->x += x * mouseSensitivity;
      }
      break;
    case MAIN_MENU:
      if(x != 0 || y != 0 || lmb || rmb){
        //MOVE CURSOR

        //currentState->objects[2].gameObjects[i].vx += (double)x/100;
        getObjectFromLabel(currentState->objects, "MainMenu_cursor")->x += x * mouseSensitivity;
        getObjectFromLabel(currentState->objects, "MainMenu_cursor")->y -= y * mouseSensitivity;
        if(getObjectFromLabel(currentState->objects, "MainMenu_cursor")->x < 0) getObjectFromLabel(currentState->objects, "MainMenu_cursor")->x = 0;
        if(getObjectFromLabel(currentState->objects, "MainMenu_cursor")->y < 0) getObjectFromLabel(currentState->objects, "MainMenu_cursor")->y = 0;
        if(getObjectFromLabel(currentState->objects, "MainMenu_cursor")->y + getObjectFromLabel(currentState->objects, "MainMenu_cursor")->sizeY > (int) getVerResolution()) getObjectFromLabel(currentState->objects, "MainMenu_cursor")->y = (int) getVerResolution() - getObjectFromLabel(currentState->objects, "MainMenu_cursor")->sizeY;
        if(getObjectFromLabel(currentState->objects, "MainMenu_cursor")->x + getObjectFromLabel(currentState->objects, "MainMenu_cursor")->sizeX > (int) getHorResolution()) getObjectFromLabel(currentState->objects, "MainMenu_cursor")->x = (int) getHorResolution() - getObjectFromLabel(currentState->objects, "MainMenu_cursor")->sizeX;

        if(cursorIsInside(getObjectFromLabel(currentState->objects, "MainMenu_cursor"), getObjectFromLabel(currentState->objects, "play-btn"))) {
          if(lmb) {
            //printf("LMB PRESSED\n");
            updateCurrentGameState(GAME_PLAY);
          }
        }
        if(cursorIsInside(getObjectFromLabel(currentState->objects, "MainMenu_cursor"), getObjectFromLabel(currentState->objects, "exit-btn"))) {
          if(lmb) {
            //printf("LMB PRESSED\n");
            updateCurrentGameState(GAME_EXIT);
          }
        }
      }
      break;
    case GAME_OVER:
      if(x != 0 || y != 0 || lmb || rmb){
        //MOVE CURSOR

        //currentState->objects[2].gameObjects[i].vx += (double)x/100;
        getObjectFromLabel(currentState->objects, "GameOver_cursor")->x += x * mouseSensitivity;
        getObjectFromLabel(currentState->objects, "GameOver_cursor")->y -= y * mouseSensitivity;
        if(getObjectFromLabel(currentState->objects, "GameOver_cursor")->x < 0) getObjectFromLabel(currentState->objects, "GameOver_cursor")->x = 0;
        if(getObjectFromLabel(currentState->objects, "GameOver_cursor")->y < 0) getObjectFromLabel(currentState->objects, "GameOver_cursor")->y = 0;
        if(getObjectFromLabel(currentState->objects, "GameOver_cursor")->y + getObjectFromLabel(currentState->objects, "GameOver_cursor")->sizeY > (int) getVerResolution()) getObjectFromLabel(currentState->objects, "GameOver_cursor")->y = (int) getVerResolution() - getObjectFromLabel(currentState->objects, "GameOver_cursor")->sizeY;
        if(getObjectFromLabel(currentState->objects, "GameOver_cursor")->x + getObjectFromLabel(currentState->objects, "GameOver_cursor")->sizeX > (int) getHorResolution()) getObjectFromLabel(currentState->objects, "GameOver_cursor")->x = (int) getHorResolution() - getObjectFromLabel(currentState->objects, "GameOver_cursor")->sizeX;

        if(cursorIsInside(getObjectFromLabel(currentState->objects, "GameOver_cursor"), getObjectFromLabel(currentState->objects, "menu-btn"))) {
          if(lmb) {
            //printf("LMB PRESSED\n");
            resetGame(gameEvents);
          }
        }
        if(cursorIsInside(getObjectFromLabel(currentState->objects, "GameOver_cursor"), getObjectFromLabel(currentState->objects, "exit-btn"))) {
          if(lmb) {
            //printf("LMB PRESSED\n");
            updateCurrentGameState(GAME_EXIT);
          }
        }
      }
      break;
    default:
      break;
  }
}

void checkCollisions(ObjLayer* objects){
  //Car
  GameObject* mainCar = getObjectFromLabel(objects, "mainCar");
  int i;
  for(i = 0; i < objects[1].size; i++){
    char* carLabel = "car";
    GameObject* curObj = &objects[1].gameObjects[i];
    if(strcmp(curObj->label, carLabel) == 0){
      if(isColliding(mainCar,curObj)){
        g_roadSpeed = g_collidedRoadSpeed;
        GameObject* propCar = zIndexPush(getState(GAME_PLAY)->objects, 3, createGameObject("propCar", curObj->x, curObj->y, 0, 0, g_carBmpNorth, g_carTransparencyColor));
        curObj->y -= 2* (int) getVerResolution();
        int mainCarCenterX = mainCar->x + mainCar->sizeX/2;
        int propCarCenterX = propCar->x + propCar->sizeX/2;
        int mainCarCenterY = mainCar->y + mainCar->sizeY/2;
        int propCarCenterY = propCar->y + propCar->sizeY/2;
        propCar->vx = g_hitStrength*(propCarCenterX - mainCarCenterX);
        propCar->vy = g_hitStrength*(propCarCenterY - mainCarCenterY);
        getState(GAME_PLAY)->currentHP -=1;
        displayHP(getState(GAME_PLAY)->currentHP, getState(GAME_PLAY)->objects);
      }
    }
  }
}

bool isColliding(GameObject* obj1, GameObject* obj2){
  //Check right
  if(obj1->x + obj1->sizeX > obj2->x && obj1->x + obj1->sizeX < obj2->x + obj2->sizeX){//POR DETECAO HORIZONTAL
    //Check top
    if(obj1->y > obj2->y && obj1->y < obj2->y + obj2->sizeY){
      //Collided top
      return true;
    }
    //Check bottom
    if(obj1->y + obj1->sizeY > obj2->y && obj1->y + obj1->sizeY < obj2->y + obj2->sizeY){
      //Collided bottom
      return true;
    }
  }
  //Check right
  if(obj1->x > obj2->x && obj1->x < obj2->x + obj2->sizeX){
    //Check top
    if(obj1->y > obj2->y && obj1->y < obj2->y + obj2->sizeY){
      //Collided top
      return true;
    }
    //Check bottom
    if(obj1->y + obj1->sizeY > obj2->y && obj1->y + obj1->sizeY < obj2->y + obj2->sizeY){
      //Collided bottom
      return true;
    }
  }

  return false;
}

void updatePositions(gameState_st* currentState){
  ObjLayer* objects = currentState->objects;
  int z;
  for(z = 0; z < Z_INDEX_MAX; z++){
    unsigned int obj;
    for(obj = 0; obj < objects[z].size; obj++){
      objects[z].gameObjects[obj].x += objects[z].gameObjects[obj].vx;
      objects[z].gameObjects[obj].y += objects[z].gameObjects[obj].vy;
    }
  }
}

void resetGame(ev_queue_st* gameEvents) {
  /* The event loop owns this queue for the whole session. Discard pending
   * events from the old round without invalidating the caller's pointer. */
  while(ev_queue_begin(gameEvents) != NULL) ev_queue_pop(gameEvents);
  stateMachine_free();
  startStates(gameEvents, MAX_HP);
  //Reset global vars
  lastScore = score;
  score = 0;
  isNight = false;
  wasNight = false;
  wasPlayBtnHovered = false;
  wasMenuBtnHovered = false;
  g_roadSpeed = g_maxRoadSpeed;
  prevRoadSpeed = g_maxRoadSpeed;
}

void displayHP(int hp, ObjLayer* gameObjects) {
  GameObject hp1 = createGameObject("HP1", 50, 50, 0, 0, g_heartBmp, g_carTransparencyColor);
  GameObject hp2 = createGameObject("HP2", 100, 50, 0, 0, g_heartBmp, g_carTransparencyColor);
  GameObject hp3 = createGameObject("HP3", 150, 50, 0, 0, g_heartBmp, g_carTransparencyColor);
  switch(hp) {
    case 0:
      zIndexRemove(gameObjects, getObjectFromLabel(gameObjects, "HP1"));
      break;
    case 1:
      zIndexRemove(gameObjects, getObjectFromLabel(gameObjects, "HP2"));
      break;
    case 2:
      zIndexRemove(gameObjects, getObjectFromLabel(gameObjects, "HP3"));
      break;
    case 3:
      zIndexPush(gameObjects, 5, hp1);
      zIndexPush(gameObjects, 5, hp2);
      zIndexPush(gameObjects, 5, hp3);
      break;
  }
}
