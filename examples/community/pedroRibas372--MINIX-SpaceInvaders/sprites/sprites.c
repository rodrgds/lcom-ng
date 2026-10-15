#include "sprites.h"
#include "../xpm/intruder.xpm"
#include "../xpm/crusader.xpm"
#include "../xpm/bullets.xpm"
#include "../xpm/startbuttons.xpm"
#include "../xpm/exitbuttons.xpm"
#include "../xpm/levelbuttons.xpm"
#include "../global.h"
#include "../game/game.h"
#include "../controllers/graphicsCard/graphics.h"
#include "../view/game/gameView.h"

Sprite* enemies[25];
int indexEnemies = 0;
int totalEnemies = 0;
int indexBullets = 0;
int moveSelecter = 0;

unsigned int randomSeed = 696969;

Sprite* bullets[50];
int totalBullets = 50

;
Sprite* hero;

Sprite* start_button;

Sprite* finalWin=NULL;
Sprite* finalLost=NULL;



int create_enemy(xpm_map_t xpm, int x, int y) {
  totalEnemies++;
  if(indexEnemies >= 25) return 1;



  Sprite *sp = (Sprite*) malloc(sizeof(Sprite));
  sp->map = xpm_load(xpm, XPM_INDEXED, &sp->img);
  if(xpm == drawCrusader_xpm){
    sp->guidedBullet=1;
  }
  sp->x = x;
  sp->y = y;
  sp->team = 1;
  sp->dead = 0;

  sp->Xspeed = 20;
  
  enemies[indexEnemies] = sp;
  indexEnemies++;
  
  return 0;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL) return;

  if (sp->map != NULL)
    free(sp->map);   

  free(sp);
}


void loadEnemiesLevel1(){
  create_enemy(drawCrusader_xpm, 190, 90);
  create_enemy(drawIntruder_xpm, 290, 90);
  create_enemy(drawCrusader_xpm, 390, 90);
  create_enemy(drawCrusader_xpm, 490, 90);
  create_enemy(drawIntruder_xpm, 590, 90);
  create_enemy(drawCrusader_xpm, 690, 90);


  create_enemy(drawIntruder_xpm, 190, 180);
  create_enemy(drawIntruder_xpm, 290, 180);
  create_enemy(drawIntruder_xpm, 390, 180);
  create_enemy(drawIntruder_xpm, 490, 180);
  create_enemy(drawIntruder_xpm, 590, 180);
  create_enemy(drawIntruder_xpm, 690, 180);

  
  create_enemy(drawIntruder_xpm, 190, 270);
  create_enemy(drawIntruder_xpm, 290, 270);
  create_enemy(drawIntruder_xpm, 390, 270);
  create_enemy(drawIntruder_xpm, 490, 270);
  create_enemy(drawIntruder_xpm, 590, 270);
  create_enemy(drawIntruder_xpm, 690, 270);
}


void loadEnemiesLevel2(){

  create_enemy(drawCrusader_xpm, 190, 90);
  create_enemy(drawCrusader_xpm, 290, 90);
  create_enemy(drawCrusader_xpm, 390, 90);
  create_enemy(drawCrusader_xpm, 490, 90);
  create_enemy(drawCrusader_xpm, 590, 90);
  create_enemy(drawCrusader_xpm, 690, 90);
  create_enemy(drawCrusader_xpm, 90, 180);
  create_enemy(drawCrusader_xpm, 190, 180);
  create_enemy(drawIntruder_xpm, 290, 180);
  create_enemy(drawIntruder_xpm, 390, 180);
  create_enemy(drawIntruder_xpm, 490, 180);
  create_enemy(drawIntruder_xpm, 590, 180);
  create_enemy(drawCrusader_xpm, 690, 180);
  create_enemy(drawCrusader_xpm, 790, 180);
  create_enemy(drawIntruder_xpm, 190, 270);
  create_enemy(drawIntruder_xpm, 290, 270);
  create_enemy(drawIntruder_xpm, 390, 270);
  create_enemy(drawIntruder_xpm, 490, 270);
  create_enemy(drawIntruder_xpm, 590, 270);
  create_enemy(drawIntruder_xpm, 690, 270);
}




void loadEnemiesLevel3(){
  create_enemy(drawCrusader_xpm, 190, 90);
  create_enemy(drawCrusader_xpm, 290, 90);
  create_enemy(drawCrusader_xpm, 390, 90);
  create_enemy(drawCrusader_xpm, 490, 90);
  create_enemy(drawCrusader_xpm, 590, 90);
  create_enemy(drawCrusader_xpm, 690, 90);
  create_enemy(drawCrusader_xpm, 190, 180);
  create_enemy(drawCrusader_xpm, 290, 180);
  create_enemy(drawCrusader_xpm, 390, 180);
  create_enemy(drawCrusader_xpm, 490, 180);
  create_enemy(drawCrusader_xpm, 590, 180);
  create_enemy(drawCrusader_xpm, 690, 180);
  create_enemy(drawCrusader_xpm, 190, 270);
  create_enemy(drawCrusader_xpm, 290, 270);
  create_enemy(drawCrusader_xpm, 390, 270);
  create_enemy(drawCrusader_xpm, 490, 270);
  create_enemy(drawCrusader_xpm, 590, 270);
  create_enemy(drawCrusader_xpm, 690, 270);
}



void destroyEnemies(){
  for (int i=0; i<indexEnemies; i++){
    destroy_sprite(enemies[i]);
  }
}

Sprite* create_hero(xpm_map_t xpm, int x, int y){
  hero = (Sprite*) malloc(sizeof(Sprite));
  hero->map = xpm_load(xpm, XPM_INDEXED, &hero->img);
  hero->x = x;
  hero->y = y;
  hero->team = 0;
  hero->dead = 0;
  hero->Xspeed = 15;
  return hero;
}

Sprite* create_static(xpm_map_t xpm, int x, int y){
  Sprite *sp = (Sprite*) malloc(sizeof(Sprite));
  if (sp == NULL) return NULL;

  sp->map = xpm_load(xpm, XPM_INDEXED, &sp->img);
  sp->x = x;
  sp->y = y;
  sp->dead = 0;
  sp->Xspeed = 0;
  return sp;
}

void eraseSprite(Sprite * sp){
  if (sp != NULL){
    for (int h = 0 ; h < sp->img.height ; h++) {
      for (int w = 0 ; w < sp->img.width ; w++) {
          draw_pixel(sp->x + w, sp->y + h, 0);
        }
    }
  }
}

void moveLeft(Sprite * sp){
  if (sp->x - sp->Xspeed > 0 && !sp->dead){
    eraseSprite(sp);
    sp->x = sp->x - sp->Xspeed;
    draw_xpm(sp->map,sp->x,sp->y,sp->img);
  }
}

void moveRight(Sprite * sp){
  if (sp->x + sp->Xspeed + sp->img.width < 1024 && !sp->dead){
    eraseSprite(sp);
    sp->x = sp->x + sp->Xspeed;
    draw_xpm(sp->map,sp->x,sp->y,sp->img);
  }
}

void moveUp(Sprite * sp){
  if(sp != NULL){
    eraseSprite(sp);
    update_screen();
    sp->y = sp->y - sp->Yspeed;
    draw_xpm(sp->map,sp->x,sp->y,sp->img);
  }
}

void moveDown(Sprite * sp){
  if(sp != NULL){
    eraseSprite(sp);
    sp->y = sp->y + sp->Yspeed;
    if (sp->guidedBullet){
      if((sp->x - hero->x-40) < 0) {moveRight(sp);}
      else {moveLeft(sp);}
    }
    draw_xpm(sp->map,sp->x,sp->y,sp->img);
  }
}


void eraseEnemies(){
  for (int i=0; i<indexEnemies; i++){
    eraseSprite(enemies[i]);
  }
}

void moveEnemiesRight(){
  for (int i=0; i<indexEnemies; i++){
    if (!enemies[i]->dead){
      moveRight(enemies[i]);
    }
  }
}

void moveEnemiesLeft(){
  
  for (int i=0; i<indexEnemies; i++){
    if (!enemies[i]->dead){
      moveLeft(enemies[i]);
    }
  }
}

void moveEnemies(){
  eraseEnemies();

  if (moveSelecter == 3){
    moveSelecter = -3;
  }

  if (moveSelecter < 0){
    moveEnemiesLeft();
  }
  else{moveEnemiesRight();}

  moveSelecter++;
}

int heroShoot(){
  int xBullet = hero->x + hero->img.width/2;
  int yBullet = hero->y - 4;
  return (createBullet(xBullet, yBullet, 8, 0));
}

int createBullet(int x, int y, int bulletSpeedY, int team){
  if(indexBullets >= 50) indexBullets = 0;   
  // Like a reload, when checking array again, it checks if there is something there and destroys it to free memory again

  Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
  sp->map = xpm_load(drawBullet_xpm, XPM_INDEXED, &sp->img);
  sp->x = x;
  sp->y = y;
  sp->team = team;
  sp->dead = 0;
  sp->Yspeed = bulletSpeedY;
  sp->Xspeed = 0;
  sp->guidedBullet=0;
  if (bullets[indexBullets] != NULL){
    destroy_sprite(bullets[indexBullets]);
  }
  bullets[indexBullets] = sp;
  indexBullets++;

  return 0;
}


int createMissile(int x, int y, int bulletX, int bulletY){
  if(indexBullets >= 50) indexBullets = 0;
  // Like a reload, when checking array again, it checks if there is something there and destroys it to free memory again

  Sprite* sp = (Sprite*) malloc(sizeof(Sprite));
  sp->map = xpm_load(drawMissile_xpm, XPM_INDEXED, &sp->img);
  sp->x = x;
  sp->y = y;
  sp->team = 1;
  sp->dead = 0;
  sp->Yspeed = bulletY;
  sp->Xspeed = bulletX;
  sp->guidedBullet = 1;
  if (bullets[indexBullets] != NULL){
    destroy_sprite(bullets[indexBullets]);
  }
  bullets[indexBullets] = sp;
  indexBullets++;
  return 0;
}

void eraseBullets(){
  for (int i = 0; i < totalBullets; i++){
    eraseSprite(bullets[i]);
  }
}

void destroyBullets(){
  for (int i = 0; i < totalBullets; i++){
    destroy_sprite(bullets[i]);
    bullets[i]=NULL;
  }
}

int moveBullets(){  
  
  for(int i = 0; i < totalBullets; i++){
    if(bullets[i] == NULL) {
      continue;
    }
    if(bullets[i]->team == 0){
      moveUp(bullets[i]);
    }
    else{
      moveDown(bullets[i]);
    }
  }
  drawEnemies();
  return 0;
}


bool check_collision(Sprite* a, Sprite* b) {
  return (a->x < b->x + b->img.width &&
          a->x + a->img.width > b->x &&
          a->y < b->y + b->img.height &&
          a->y + a->img.height > b->y &&
        a->team != b->team);
}


// given a seed, generates a random number
int getRandomNumber(){
  if(indexEnemies == 0) return -1;

  randomSeed = (randomSeed * 1103515245 + 12345) & 0x7FFFFFFF;
  return randomSeed % indexEnemies;
}

int enemyShoot(){
  int index = getRandomNumber();
  int index2 = getRandomNumber();
  if(index < 0) return 1;
  if(index2 < 0) return 1;

  int x1 = enemies[index]->x;
  int y1 = enemies[index]->y;

  int x2 = enemies[index2]->x;
  int y2 = enemies[index2]->y;
  
  if (!enemies[index]->dead){
    if(enemies[index]->guidedBullet){
      createMissile(x1 + 25, y1 + 34 , 1 , 2);
      createMissile(x1 + 82, y1 + 34, 1 ,2);
    }
    else{
      createBullet(x1 + 25, y1 + 34, 4, 1);
      createBullet(x1 + 82, y1 + 34, 4, 1);
    }
  }

  if (!enemies[index2]->dead){
    if(enemies[index2]->guidedBullet){
      createMissile(x2 + 25, y2 + 34, 1 , 2);
      createMissile(x2 + 82, y2 + 34, 1 , 2);
    }
    else{
      createBullet(x2 + 25, y2 + 34, 4, 1);
      createBullet(x2 + 82, y2 + 34, 4, 1);
    }
  }
  return 0;
}

