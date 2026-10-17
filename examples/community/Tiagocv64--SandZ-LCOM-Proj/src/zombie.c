#include <lcom/lcf.h>
#include "libs/video.h"
#include "UI_game.h"
#include "Bitmap.h"
#include "zombie.h"
#include "obstacle.h"
#include "blood.h"
#include "player.h"
#include "bullet.h"
#include "game.h"
#include "wait_winner_menu.h"
#include "over_menu.h"
#include <time.h>
#include <stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define abs(x) ((x)<0 ? -(x) : (x))
#define exp2(x) ((x)*(x))

Bitmap* zombieN[8];
Bitmap* zombieW[8];
Bitmap* zombieE[8];
Bitmap* zombieS[8];
Bitmap* zombieNE[8];
Bitmap* zombieSE[8];
Bitmap* zombieNW[8];
Bitmap* zombieSW[8];

unsigned long long GodModeTimer;
static unsigned int round = 0;
unsigned int zombies_total_round = 0;
unsigned int zombies_alive_round = 0;
unsigned int counter_ticks_write = 0;
static int zombies_max_hp = 100;

int speed_zombie = 2;
struct Zombie list_zombies[20];

extern int points_game;
extern struct Obstacle list_obsts[5];
extern int h_res;
extern int v_res;
extern unsigned long long counter;
extern bool two_player;
extern struct Player_state player_state;
extern int player_y;
extern int player_x;
extern enum program_state p_state;
extern struct Bullet list[30];
extern struct Weapon sniper;

void init_zombies(){
  struct Zombie zomb;
  zomb.alive = false;
  for (int i = 0; i < 20; i++)
    list_zombies[i] = zomb;
  srand(time(NULL));
  for (int i = 0; i < 8; i++){
    char buf[100];
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dN.bmp", i);
    zombieN[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dE.bmp", i);
    zombieE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dS.bmp", i);
    zombieS[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dW.bmp", i);
    zombieW[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dNE.bmp", i);
    zombieNE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dSE.bmp", i);
    zombieSE[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dNW.bmp", i);
    zombieNW[i] = loadBitmap(buf);
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/zombie/zombie%dSW.bmp", i);
    zombieSW[i] = loadBitmap(buf);
  }
  round = 0;
  zombies_total_round = 0;
  zombies_alive_round = 0;
  counter_ticks_write = 0;
}

void destroy_zombies(){
  for (int i = 0; i < 8; i++){
    deleteBitmap(zombieN[i]);
    deleteBitmap(zombieW[i]);
    deleteBitmap(zombieE[i]);
    deleteBitmap(zombieS[i]);
    deleteBitmap(zombieNE[i]);
    deleteBitmap(zombieSE[i]);
    deleteBitmap(zombieNW[i]);
    deleteBitmap(zombieSW[i]);
  }
}

bool detect_zombie_collision(int x, int y){
  for (int i = 0; i < 5; i++){
    if (x + 35 >= list_obsts[i].x && x - 35 <= list_obsts[i].x + list_obsts[i].size_x && // COLLISION
        y + 35 >= list_obsts[i].y && y - 35 <= list_obsts[i].y + list_obsts[i].size_y){
      return true;
    }
  }
  return false;
}

void create_zombie(){
  struct Zombie zomb;
  do {
    zomb.x = (rand() % (h_res-60)) + 30;
    zomb.y = (rand() % (v_res-60)) + 30;
  } while(detect_zombie_collision(zomb.x, zomb.y) || abs(zomb.x-(player_x+60)) + abs(zomb.y-(player_y+60)) < 300);
  zomb.directionX = NOT_MOVING;
  zomb.directionY = NOT_MOVING;
  zomb.alive = true;
  zomb.hp = zombies_max_hp;
  zomb.bitState = 0;
  zomb.bitTime = 0;
  zombies_total_round--;
  zombies_alive_round++;
  for (int i = 0; i < 20; i++){
    if (!list_zombies[i].alive){
      list_zombies[i] = zomb;
      return;
    }
  }
  // If cant make a zombie because there is 20 zombies already
  zombies_total_round++;
  zombies_alive_round--;
}

void attack_zombie(){
    GodModeTimer = counter + (1.5 * 60);
    player_state.godMode = true;

    player_state.lives--;

    if(player_state.lives <= 0){
      if (two_player) {
        p_state = WAIT_WINNER_TWO;
        init_wait_winner_menu();
      } else {
        p_state = END_GAME;
        init_over_menu();
      }
    }


  }


void update_zombies(){

  if (zombies_total_round == 0 && zombies_alive_round == 0){
    round++;
    zombies_max_hp += 10;
    zombies_total_round = round * 5;
    scheduleWave(round);
  }

  if (counter % 120 == 0 && zombies_total_round != 0){
    create_zombie();
  }
  for (int i = 0; i < 20; i++){
    if (list_zombies[i].alive){
      if (player_x < list_zombies[i].x){
        list_zombies[i].directionX = MOVING_LEFT;
        if (!detect_zombie_collision(list_zombies[i].x - speed_zombie, list_zombies[i].y))
          list_zombies[i].x -= min(speed_zombie, abs(player_x - list_zombies[i].x));
      } else if (player_x > list_zombies[i].x) {
        list_zombies[i].directionX = MOVING_RIGHT;
        if (!detect_zombie_collision(list_zombies[i].x + speed_zombie, list_zombies[i].y))
          list_zombies[i].x += min(speed_zombie, abs(player_x - list_zombies[i].x));
      } else {
        list_zombies[i].directionX = NOT_MOVING;
      }
      if (player_y < list_zombies[i].y){
        list_zombies[i].directionY = MOVING_UP;
        if (!detect_zombie_collision(list_zombies[i].x, list_zombies[i].y - speed_zombie))
          list_zombies[i].y -= min(speed_zombie, abs(player_y - list_zombies[i].y));
      } else if (player_y > list_zombies[i].y) {
        list_zombies[i].directionY = MOVING_DOWN;
        if (!detect_zombie_collision(list_zombies[i].x, list_zombies[i].y + speed_zombie))
          list_zombies[i].y += min(speed_zombie, abs(player_y - list_zombies[i].y));
      } else {
        list_zombies[i].directionY = NOT_MOVING;
      }
      list_zombies[i].y = list_zombies[i].y < 0 ? 0 : list_zombies[i].y;
      list_zombies[i].x = list_zombies[i].x < 0 ? 0 : list_zombies[i].x;
      for (int j = 0; j < 15; j++){
        if(list[j].alive){
          if (list[j].x >= list_zombies[i].x - 40 && list_zombies[i].x + 40 >= list[j].x &&
              list[j].y > list_zombies[i].y - 40 && list_zombies[i].y + 40 > list[j].y){
                if(player_state.weaponEquiped.id == 2)
                 list_zombies[i].hp -= sniper.damage;
                else{
                  list_zombies[i].hp -= player_state.weaponEquiped.damage;
                  list[j].alive = false;
                }
                if(list_zombies[i].hp <= 0){
                  list_zombies[i].alive = false;
                  points_game += 100 * round;
                  create_blood(list_zombies[i].x, list_zombies[i].y);
                  zombies_alive_round--;
                }
          }
        }
      }
      if (counter >= GodModeTimer)
        player_state.godMode = false;
      if(list_zombies[i].alive) {
        if(!player_state.godMode){
          if (exp2(list_zombies[i].x - player_x) + exp2(list_zombies[i].y - player_y) <= 3025)
            attack_zombie();
        }
      }
    }
  }
}



void draw_zombies(){
  for (int i = 0; i < 20; i++){
    if (list_zombies[i].alive){
      if (list_zombies[i].directionX == MOVING_RIGHT && list_zombies[i].directionY == MOVING_UP){
        drawBitmap(zombieNE[list_zombies[i].bitState], list_zombies[i].x - 45, list_zombies[i].y - 50);
      } else if (list_zombies[i].directionX == MOVING_RIGHT && list_zombies[i].directionY == MOVING_DOWN){
        drawBitmap(zombieSE[list_zombies[i].bitState], list_zombies[i].x - 50, list_zombies[i].y - 45);
      } else if (list_zombies[i].directionX == MOVING_LEFT && list_zombies[i].directionY == MOVING_DOWN){
        drawBitmap(zombieSW[list_zombies[i].bitState], list_zombies[i].x - 55, list_zombies[i].y - 50);
      } else if (list_zombies[i].directionX == MOVING_LEFT && list_zombies[i].directionY == MOVING_UP){
        drawBitmap(zombieNW[list_zombies[i].bitState], list_zombies[i].x - 50, list_zombies[i].y - 55);
      } else if (list_zombies[i].directionX == MOVING_LEFT){
        drawBitmap(zombieW[list_zombies[i].bitState], list_zombies[i].x - 40, list_zombies[i].y - 40);
      } else if (list_zombies[i].directionX == MOVING_RIGHT){
        drawBitmap(zombieE[list_zombies[i].bitState], list_zombies[i].x - 40, list_zombies[i].y - 40);
      } else if (list_zombies[i].directionY == MOVING_UP){
        drawBitmap(zombieN[list_zombies[i].bitState], list_zombies[i].x - 40, list_zombies[i].y - 40);
      } else {
        drawBitmap(zombieS[list_zombies[i].bitState], list_zombies[i].x - 40, list_zombies[i].y - 40);
      }
      if (list_zombies[i].bitTime++ > 7){
        list_zombies[i].bitState = (list_zombies[i].bitState+1) % 8;
        list_zombies[i].bitTime = 0;
      }
    }
  }
}

void draw_zombies_hp(){
  for (int i = 0; i < 20; i++){
    if(list_zombies[i].alive){
      if(list_zombies[i].hp < zombies_max_hp){
        vg_draw_rectangle(list_zombies[i].x - 40, list_zombies[i].y - 70 , (list_zombies[i].hp * 80 / zombies_max_hp), 10, GREEN);

        vg_draw_rectangle(list_zombies[i].x - 40 + (list_zombies[i].hp * 80 / zombies_max_hp) , list_zombies[i].y - 70 , ((zombies_max_hp - list_zombies[i].hp) * 80 / zombies_max_hp), 10, RED);
      }
    }
  }
}
