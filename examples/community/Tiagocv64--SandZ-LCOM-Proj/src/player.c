#include <lcom/lcf.h>
#include "libs/i8042.h"
#include "libs/video.h"
#include "player.h"
#include "Bitmap.h"
#include <string.h>
#include "enum.h"
#include "obstacle.h"
#include "bullet.h"
#include "weapon.h"
#include "UI_game.h"


int player_y = 200;
int player_x = 200;
uint32_t speed = 3;
uint32_t counter_reload_done = 0;

uint8_t bitmapState = 0;
uint8_t stateTime = 0;

Bitmap* soldierN[4];
Bitmap* soldierW[4];
Bitmap* soldierE[4];
Bitmap* soldierS[4];
Bitmap* soldierNE[4];
Bitmap* soldierSE[4];
Bitmap* soldierNW[4];
Bitmap* soldierSW[4];

struct Player_state player_state;

extern int points_game;
extern int points_adv;
extern struct Obstacle list_obsts[5];
extern int h_res;
extern int v_res;
extern unsigned long long counter;
extern struct Weapon shotgun;
extern struct Weapon assault_rifle;
extern struct Weapon sniper;
extern bool two_player;

bool detect_player_collision(int x, int y){
  for (int i = 0; i < 5; i++){
    if (x + 40 >= list_obsts[i].x && x - 40 <= list_obsts[i].x + list_obsts[i].size_x && // COLLISION
        y + 40 >= list_obsts[i].y && y - 40 <= list_obsts[i].y + list_obsts[i].size_y){
      return true;
    }
  }
  return false;
}

void init_player(){
  player_state.godMode = false;
  player_state.leftPressed = false;
  player_state.rightPressed = false;
  player_state.upPressed = false;
  player_state.downPressed = false;
  player_state.directionY = MOVING_DOWN;
  player_state.directionX = NOT_MOVING;
  player_state.lives = 3;
  player_state.weaponEquiped = assault_rifle;

  player_y = 200;
  player_x = 200;
  points_game = 0;
  points_adv = 0;
  speed = 3;
  counter_reload_done = 0;

  for (int i = 0; i < 4; i++){
    int tmp = i;
    if (i == 0) tmp = 2; // Image in 0 state is equal to image in 2 state
    char buf[100];
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dN.bmp", tmp);
    soldierN[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dE.bmp", tmp);
    soldierE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dS.bmp", tmp);
    soldierS[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dW.bmp", tmp);
    soldierW[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dNE.bmp", tmp);
    soldierNE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dSE.bmp", tmp);
    soldierSE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dNW.bmp", tmp);
    soldierNW[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/soldier/soldier%dSW.bmp", tmp);
    soldierSW[i] = loadBitmap(buf);
  }

  do {
    player_x = (rand() % (h_res-200)) + 100;
    player_y = (rand() % (v_res-200)) + 100;
  } while(detect_player_collision(player_x, player_y));
  // if (soldierN == NULL || soldierS == NULL || soldierW == NULL || soldierE == NULL)
  //   return NULL;
}

void destroy_player(){
  for (int i = 0; i < 4; i++){
    deleteBitmap(soldierN[i]);
    deleteBitmap(soldierW[i]);
    deleteBitmap(soldierE[i]);
    deleteBitmap(soldierS[i]);
    deleteBitmap(soldierNE[i]);
    deleteBitmap(soldierSE[i]);
    deleteBitmap(soldierNW[i]);
    deleteBitmap(soldierSW[i]);
  }
}

void draw_player(){
  if (player_state.directionX == MOVING_RIGHT && player_state.directionY == MOVING_UP){
    drawBitmapWithRedFilter(soldierNE[bitmapState], player_x-50, player_y-70, player_state.godMode);
  } else if (player_state.directionX == MOVING_RIGHT && player_state.directionY == MOVING_DOWN){
    drawBitmapWithRedFilter(soldierSE[bitmapState], player_x-60, player_y-50, player_state.godMode);
  } else if (player_state.directionX == MOVING_LEFT && player_state.directionY == MOVING_DOWN){
    drawBitmapWithRedFilter(soldierSW[bitmapState], player_x-80, player_y-60, player_state.godMode);
  } else if (player_state.directionX == MOVING_LEFT && player_state.directionY == MOVING_UP){
    drawBitmapWithRedFilter(soldierNW[bitmapState], player_x-70, player_y-80, player_state.godMode);
  } else if (player_state.directionX == MOVING_LEFT){
    drawBitmapWithRedFilter(soldierW[bitmapState], player_x-80, player_y-40, player_state.godMode);
  } else if (player_state.directionX == MOVING_RIGHT){
    drawBitmapWithRedFilter(soldierE[bitmapState], player_x-40, player_y-40, player_state.godMode);
  } else if (player_state.directionY == MOVING_DOWN){
    drawBitmapWithRedFilter(soldierS[bitmapState], player_x-40, player_y-40, player_state.godMode);
  } else if (player_state.directionY == MOVING_UP){
    drawBitmapWithRedFilter(soldierN[bitmapState], player_x-40, player_y-80, player_state.godMode);
  }
  if (stateTime++ > 9){
    bitmapState = (bitmapState+1)%4;
    stateTime = 0;
  }
}

void change_player_state(uint8_t key_code){
  switch(key_code){
    case UP_ARROW_MAKE:
      player_state.upPressed = true;
      break;
    case UP_ARROW_BREAK:
      player_state.upPressed = false;
      break;
    case DOWN_ARROW_MAKE:
      player_state.downPressed = true;
      break;
    case DOWN_ARROW_BREAK:
      player_state.downPressed = false;
      break;
    case LEFT_ARROW_MAKE:
      player_state.leftPressed = true;
      break;
    case LEFT_ARROW_BREAK:
      player_state.leftPressed = false;
      break;
    case RIGHT_ARROW_MAKE:
      player_state.rightPressed = true;
      break;
    case RIGHT_ARROW_BREAK:
      player_state.rightPressed = false;
      break;
  }
}

void move_player(){ // -TODO change directions on chagestate //TODO let direction be diagonal when player is not moving
  bool movingY = true;
  bool movingX = true;
  if (player_state.leftPressed && !player_state.rightPressed){
    if (!detect_player_collision(player_x - speed, player_y))
      player_x -= speed;
    player_state.directionX = MOVING_LEFT;
  }else if (player_state.rightPressed && !player_state.leftPressed){
    if (!detect_player_collision(player_x + speed, player_y))
      player_x += speed;
    player_state.directionX = MOVING_RIGHT;
  } else {
    movingX = false;
    if (player_state.directionY != NOT_MOVING)
      player_state.directionX = NOT_MOVING;
  }
  if (player_state.upPressed && !player_state.downPressed){
    if (!detect_player_collision(player_x, player_y - speed))
      player_y -= speed;
    player_state.directionY = MOVING_UP;
  } else if (player_state.downPressed && !player_state.upPressed){
    if (!detect_player_collision(player_x, player_y + speed))
      player_y += speed;
    player_state.directionY = MOVING_DOWN;
  } else {
    movingY = false;
    if (player_state.directionX != NOT_MOVING)
      player_state.directionY = NOT_MOVING;
  }

  if (player_y - 40 < 0)
    player_y = 40;
  else if (player_y + 40 > v_res)
    player_y = v_res - 40;

  if (player_x - 40 < 0)
    player_x = 40;
  else if (player_x + 40 > h_res)
    player_x = h_res - 40;

  if (!movingX && !movingY){
    bitmapState = 0;
    stateTime = 0;
  }
}

void reload_player(){
  if (player_state.weaponEquiped.current_magazine == player_state.weaponEquiped.max_magazine)
  return;
  counter_reload_done = counter + 3 * 60;
  scheduleReloadingText();

}

void fire_player(){
  if (counter > counter_reload_done && player_state.weaponEquiped.current_magazine > 0){
    if(player_state.weaponEquiped.id == 1){
      for(int i = -1 ; i < 2; i++)
      create_bullet(player_x -5 , player_y, player_state.directionY, player_state.directionX, i);
      player_state.weaponEquiped.current_magazine--;
    } else {
      create_bullet(player_x, player_y, player_state.directionY, player_state.directionX, player_state.weaponEquiped.id + 3);
      player_state.weaponEquiped.current_magazine--;
    }
  }
}

void change_weapon(uint8_t key_code){
  if(counter > counter_reload_done){
    switch(player_state.weaponEquiped.id){
      case 0:
      assault_rifle = player_state.weaponEquiped;
      break;
      case 1:
      shotgun = player_state.weaponEquiped;
      break;
      case 2:
      sniper = player_state.weaponEquiped;
      break;
    }
    switch(key_code){
      case KEY_1:
      player_state.weaponEquiped = assault_rifle;
      break;
      case KEY_2:
      player_state.weaponEquiped = shotgun;
      break;
      case KEY_3:
      player_state.weaponEquiped = sniper;
      break;
    }
  }
}

void update_points(){
  switch(player_state.lives){
    case 3:
    points_game += 30;
    break;
    case 2:
    points_game += 20;
    break;
    case 1:
    points_game += 10;
    break;
  }
}

void update_player(){
  move_player();
  if (counter_reload_done == counter) // MAG FULL
    player_state.weaponEquiped.current_magazine = player_state.weaponEquiped.max_magazine;

  scheduleLives(player_state.lives);
  scheduleMagazine(player_state.weaponEquiped.current_magazine, player_state.weaponEquiped.max_magazine);
  scheduleWeaponSlot(player_state.weaponEquiped.id);

  if (two_player)
    schedulePointsTwo(points_game, points_adv);
  else
    schedulePoints(points_game);
}

void pause_player(){
  player_state.leftPressed = false;
  player_state.rightPressed = false;
  player_state.upPressed = false;
  player_state.downPressed = false;
  if (counter_reload_done > counter)
    counter_reload_done -= counter;
  else
    counter_reload_done = 0xFFFFFFFF;
}

void resume_player(){
  if (counter_reload_done == 0xFFFFFFFF)
    counter_reload_done = counter - 1;
  else
    counter_reload_done += counter;
}
