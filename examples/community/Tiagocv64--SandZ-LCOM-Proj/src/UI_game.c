#include <lcom/lcf.h>
#include "libs/rtc.h"
#include "libs/video.h"
#include "UI_game.h"
#include "Bitmap.h"
#include "text.h"
#include "enum.h"


bool draw_lives_UI = false;
bool draw_points_UI = false;
bool draw_points_two_UI = false;
bool draw_timer_UI = false;
bool draw_magazine_UI = false;
bool draw_weapon_slot_UI = false;
unsigned int drawNextWaveCount_UI = 0;
unsigned int drawReloadingText_UI = 0;

int currentLives = 0;
int currentPoints = 0;
int currentPointsTwo = 0;
int currentTimer = 0;
int currentWave = 0;
int currentMagazine = 0;
int magazineMax = 0;
int currentWeaponId = 0;

Bitmap* heart;
Bitmap* background_day;
Bitmap* background_night;

Bitmap* weapon_slot;
Bitmap* weapon_slot_selected;
Bitmap* weapon1;
Bitmap* weapon2;
Bitmap* weapon3;

extern int h_res;
extern int v_res;
extern unsigned long long counter;
extern bool isDay;
extern char date_rtc[20];

void init_background(){
  if (background_day == NULL)
    background_day = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/background.bmp");
  if (background_night == NULL)
    background_night = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/background_night.bmp");
}

void initUI(){
  heart = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/heart.bmp");
  weapon_slot = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/weapon/weapon_slot.bmp");
  weapon_slot_selected = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/weapon/weapon_slot_selected.bmp");
  weapon1 = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/weapon/weapon1.bmp");
  weapon2 = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/weapon/weapon2.bmp");
  weapon3 = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/weapon/weapon3.bmp");
}

void destroyUI(){
  deleteBitmap(heart);
  deleteBitmap(weapon_slot);
  deleteBitmap(weapon_slot_selected);
  deleteBitmap(weapon1);
  deleteBitmap(weapon2);
  deleteBitmap(weapon3);
}

void clearUI(){
  draw_lives_UI = false;
  draw_points_UI = false;
  draw_points_two_UI = false;
  draw_timer_UI = false;
  draw_magazine_UI = false;
  draw_weapon_slot_UI = false;
}

void draw_background(){
  if (isDay)
    drawBitmap(background_day, 0, 0);
  else
    drawBitmap(background_night, 0, 0);
}

void draw_title(Bitmap* bmp, int y){
  drawBitmap(bmp, h_res/2 - bmp->bitmapInfoHeader.width/2, y - bmp->bitmapInfoHeader.height/2);
}

void drawLives(){
  switch(currentLives){
    case 3:
     drawBitmap(heart, 1100, 20);
     drawBitmap(heart, 1150, 20);
     drawBitmap(heart, 1200, 20);
     break;
    case 2:
     drawBitmap(heart, 1200, 20);
     drawBitmap(heart, 1150, 20);
     break;
    case 1:
     drawBitmap(heart, 1200, 20);
     break;
  }
}

void drawPoints(){
  char tmp[20];
  uint32_t size_points = sprintf(tmp, "%010d", currentPoints);
  print_string(tmp, size_points, 20, h_res/2-75, WHITE, SMALL, ALIGN_LEFT);
}

void drawPointsTwo(){
  char tmp1[25], tmp2[25];
  uint32_t size_points1 = sprintf(tmp1, "YOU: %010d", currentPoints);
  uint32_t size_points2 = sprintf(tmp2, "P2: %010d", currentPointsTwo);
  print_string(tmp1, size_points1, 20, h_res/2-175, WHITE, SMALL, ALIGN_LEFT);
  print_string(tmp2, size_points2, 20, h_res/2+25, WHITE, SMALL, ALIGN_LEFT);
}

void drawWave(){
  char text_round[40];
  uint32_t text_size = sprintf(text_round, "All clear, wave %d incoming!", currentWave);
  print_string(text_round, text_size, v_res - 200, h_res/2 - 200, WHITE, SMALL, ALIGN_LEFT);
}

void drawTimer(){
  char tmp[10];
  uint32_t size_time = sprintf(tmp, "%02d:%02d", currentTimer/60, currentTimer%60);
  print_string( tmp, size_time, 10, 10, WHITE, SMALL, ALIGN_LEFT);
}

void drawMagazine(){
  char tmp[15];
  uint32_t size_magazine = sprintf(tmp, "Ammo: %d/%d", currentMagazine, magazineMax);
  print_string( tmp, size_magazine, v_res - 40, 15, WHITE, SMALL, ALIGN_LEFT);
}

void drawReloadingText(){
  char text_round[25];
  uint32_t text_size = sprintf(text_round, "Reloading weapon...");
  print_string(text_round, text_size, v_res - 230, h_res/2 - 125, WHITE, SMALL, ALIGN_LEFT);
}

void drawTimeRTC(){
  print_string(date_rtc, 17, v_res - 40, h_res - 15, WHITE, SMALL, ALIGN_RIGHT);
}

void drawWeaponSlot(){
  drawBitmap(weapon_slot, h_res/2 - 120, v_res - 80);
  drawBitmap(weapon_slot, h_res/2 - 40, v_res - 80);
  drawBitmap(weapon_slot, h_res/2 + 40, v_res - 80);
  if (currentWeaponId == 0)
    drawBitmap(weapon_slot_selected, h_res/2 - 124, v_res - 84);
  else if (currentWeaponId == 1)
    drawBitmap(weapon_slot_selected, h_res/2 - 44, v_res - 84);
  else if (currentWeaponId == 2)
    drawBitmap(weapon_slot_selected, h_res/2 + 36, v_res - 84);
  drawBitmap(weapon1, h_res/2 - 108, v_res - 68);
  drawBitmap(weapon2, h_res/2 - 28, v_res - 68);
  drawBitmap(weapon3, h_res/2 + 52, v_res - 68);
}

void scheduleLives(int lives){
  currentLives = lives;
  draw_lives_UI = true;
}

void schedulePoints(int points){
  currentPoints = points;
  draw_points_UI = true;
}

void schedulePointsTwo(int points1, int points2){
  currentPoints = points1;
  currentPointsTwo = points2;
  draw_points_two_UI = true;
}

void scheduleWave(int wave){
  currentWave = wave;
  drawNextWaveCount_UI = counter + 2 * 60;
}

void scheduleTimer(int timer_s){
  currentTimer = timer_s;
  draw_timer_UI = true;
}

void scheduleMagazine(int current, int max){
  currentMagazine = current;
  magazineMax = max;
  draw_magazine_UI = true;
}

void scheduleReloadingText(){
  drawReloadingText_UI = counter + 3 * 60;
}

void scheduleWeaponSlot(int active_id){
  currentWeaponId = active_id;
  draw_weapon_slot_UI = true;
}

void drawUI(){
  if (draw_lives_UI)
    drawLives();
  if (draw_timer_UI)
    drawTimer();
  if (draw_points_UI)
    drawPoints();
  if (draw_points_two_UI)
    drawPointsTwo();
  if (draw_magazine_UI)
    drawMagazine();
  if (draw_weapon_slot_UI)
    drawWeaponSlot();
  if (counter < drawNextWaveCount_UI)
    drawWave();
  if (counter < drawReloadingText_UI)
    drawReloadingText();
  drawTimeRTC();
  clearUI();
}
