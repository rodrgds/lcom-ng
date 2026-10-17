#include <lcom/lcf.h>
#include "Bitmap.h"
#include "libs/video.h"
#include "life.h"
#include "obstacle.h"
#include "player.h"

#define exp2(x) ((x)*(x))

struct Lives list_lives[3];
Bitmap* heart;

extern struct Obstacle list_obsts[5];
extern struct Player_state player_state;
extern int player_y;
extern int player_x;

void init_lives(){
  struct Lives liv;
  liv.pickupable = false;
  for (int i = 0; i < 3;i++)
    list_lives[i] = liv;
  heart = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/heart.bmp");
}

void destroy_lives(){
  deleteBitmap(heart);
}

bool detect_live_collision(int x, int y){
  for (int i = 0; i < 5; i++){
    if (x + 25 >= list_obsts[i].x && x - 25 <= list_obsts[i].x + list_obsts[i].size_x && // COLLISION
        y + 25 >= list_obsts[i].y && y - 25 <= list_obsts[i].y + list_obsts[i].size_y){
      return true;
    }
  }
  return false;
}

void  create_life(){
  struct Lives liv;
  liv.pickupable = true;
  do {
    liv.x = (rand() % (h_res - 60)) + 30;
    liv.y = (rand() % (v_res - 60)) + 30;
  } while(detect_live_collision(liv.x, liv.y));
  for (int i = 0; i < 3;i++){
    if(!list_lives[i].pickupable){
      list_lives[i] = liv;
      return;
    }
  }
}

void draw_lives(){
  for (int i = 0; i < 3;i++){
    if(list_lives[i].pickupable)
     drawBitmap(heart, list_lives[i].x - heart->bitmapInfoHeader.width/2, list_lives[i].y - heart->bitmapInfoHeader.height/2);
  }
}

void pick_life(){
  player_state.lives++;
}

void update_lives(){
  for (int i = 0; i < 3;i++){
    if(list_lives[i].pickupable){
      if(player_state.lives < 3){
        if (exp2(list_lives[i].x - player_x) + exp2(list_lives[i].y - player_y) <= 3600){
          pick_life();
          list_lives[i].pickupable = false;
        }
      }
    }
  }
}
