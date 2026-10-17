#include <lcom/lcf.h>
#include "libs/video.h"
#include "libs/timer.h"
#include "Bitmap.h"
#include "bullet.h"
#include "obstacle.h"
#include "enum.h"

uint32_t speed_bullet = 15;
struct Bullet list[30];

extern int h_res;
extern int v_res;
extern unsigned long long counter;
extern struct Obstacle list_obsts[5];

Bitmap* bulletN;
Bitmap* bulletS;
Bitmap* bulletE;
Bitmap* bulletW;
Bitmap* bulletNE;
Bitmap* bulletNW;
Bitmap* bulletSE;
Bitmap* bulletSW;
Bitmap* pellet;

void init_bullets(){
  struct Bullet bul;
  bul.alive = false;
  for (int i = 0; i < 30; i++)
    list[i] = bul;
  bulletN = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletN.bmp");
  bulletW = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletW.bmp");
  bulletE = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletE.bmp");
  bulletS = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletS.bmp");
  bulletNE = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletNE.bmp");
  bulletNW = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletNW.bmp");
  bulletSE = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletSE.bmp");
  bulletSW = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/bulletSW.bmp");
  pellet = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/bullet/pellet.bmp");
}

void destroy_bullets(){
  deleteBitmap(bulletW);
  deleteBitmap(bulletN);
  deleteBitmap(bulletE);
  deleteBitmap(bulletS);
  deleteBitmap(bulletSE);
  deleteBitmap(bulletNE);
  deleteBitmap(bulletNW);
  deleteBitmap(bulletSW);
  deleteBitmap(pellet);
}

void create_bullet(int x, int y, int directionY, int directionX, int id){
  struct Bullet bul;
  bul.id = id;
  bul.disapearCounter = counter + 20;
  bul.x = x;
  bul.y = y;
  bul.directionX = directionX;
  bul.directionY = directionY;
  if (bul.directionY == MOVING_UP){
    if (bul.directionX == MOVING_LEFT){
      bul.x = bul.x - 45;
      bul.y = bul.y - 65;
    } else if (bul.directionX == MOVING_RIGHT){
      bul.x = bul.x + 60;
      bul.y = bul.y - 45;
    } else {
      bul.x = bul.x + 10;
      bul.y = bul.y - 80;
    }
  } else if (bul.directionY == MOVING_DOWN){
    if (bul.directionX == MOVING_LEFT){
      bul.x = bul.x - 70;
      bul.y = bul.y + 45;
    } else if (bul.directionX == MOVING_RIGHT){
      bul.x = bul.x + 45;
      bul.y = bul.y + 65;
    } else {
      bul.x = bul.x - 15;
      bul.y = bul.y + 80;
    }
  } else if (bul.directionX == MOVING_LEFT){
    bul.x = bul.x - 80;
    bul.y = bul.y - 15;
  } else if (bul.directionX == MOVING_RIGHT){
    bul.x = bul.x + 80;
    bul.y = bul.y + 10;
  }
  bul.alive = true;
  for (int i = 0; i < 30; i++){
    if (!list[i].alive){
      list[i] = bul;
      return;
    }
  }
}

bool detect_bullet_collision(int x, int y){
  for (int i = 0; i < 5; i++){
    if (x >= list_obsts[i].x && x <= list_obsts[i].x + list_obsts[i].size_x && // COLLISION
        y >= list_obsts[i].y && y <= list_obsts[i].y + list_obsts[i].size_y){
      return true;
    }
  }
  return false;
}

void update_bullets(){
  for (int i = 0; i < 30; i++){
    if (list[i].alive){
      if(list[i].id == -1 || list[i].id == 0 || list[i].id == 1) {

        if (list[i].directionX == MOVING_LEFT){
          if (list[i].directionY == MOVING_UP){
            list[i].y -= speed_bullet;
            list[i].x -= 5*list[i].id;
            list[i].x -= speed_bullet;
            list[i].y += 5*list[i].id;
          } else if (list[i].directionY == MOVING_DOWN){
            list[i].x -= speed_bullet;
            list[i].y += 5*list[i].id;
            list[i].y += speed_bullet;
            list[i].x += 5*list[i].id;
          } else {
            list[i].x -= speed_bullet;
            list[i].y += 5*list[i].id;
          }
        } else if (list[i].directionX == MOVING_RIGHT){
          if (list[i].directionY == MOVING_UP){
            list[i].y -= speed_bullet;
            list[i].x += 5*list[i].id;
            list[i].x += speed_bullet;
            list[i].y += 5*list[i].id;
          } else if (list[i].directionY == MOVING_DOWN){
            list[i].x += speed_bullet;
            list[i].y += 5*list[i].id;
            list[i].y += speed_bullet;
            list[i].x -= 5*list[i].id;
          } else {
            list[i].x += speed_bullet;
            list[i].y += 5*list[i].id;
          }
        } else if (list[i].directionY == MOVING_UP){
          list[i].x += 5*list[i].id;
          list[i].y -= speed_bullet;
        } else if (list[i].directionY == MOVING_DOWN){
          list[i].x += 5*list[i].id;
          list[i].y += speed_bullet;
        }

        if (list[i].disapearCounter < counter)
          list[i].alive = false;
        if ( list[i].x > h_res || list[i].y > v_res)
          list[i].alive = false;
        if (detect_bullet_collision(list[i].x, list[i].y))
          list[i].alive = false;
      }
       else {
        if (list[i].directionX == MOVING_LEFT){
          list[i].x -= speed_bullet;
        } else if (list[i].directionX == MOVING_RIGHT){
          list[i].x += speed_bullet;
        }
        if (list[i].directionY == MOVING_UP){
          list[i].y -= speed_bullet;
        } else if (list[i].directionY == MOVING_DOWN){
          list[i].y += speed_bullet;
        }
        if ( list[i].x > h_res || list[i].y > v_res)
          list[i].alive = false;
        if (detect_bullet_collision(list[i].x, list[i].y))
          list[i].alive = false;
      }
    }
  }
}

void draw_bullets(){
  for (int i = 0; i < 30; i++){
    if (list[i].alive){
      if(list[i].id == -1 || list[i].id == 0 || list[i].id == 1)
        drawBitmap(pellet, list[i].x, list[i].y);
      else {
        if (list[i].directionY == MOVING_UP){
          if (list[i].directionX == MOVING_LEFT){
            drawBitmap(bulletNW, list[i].x, list[i].y);
          } else if (list[i].directionX == MOVING_RIGHT){
            drawBitmap(bulletNE, list[i].x, list[i].y);
          } else {
            drawBitmap(bulletN, list[i].x, list[i].y);
          }
        } else if (list[i].directionY == MOVING_DOWN){
          if (list[i].directionX == MOVING_LEFT){
            drawBitmap(bulletSW, list[i].x, list[i].y);
          } else if (list[i].directionX == MOVING_RIGHT){
            drawBitmap(bulletSE, list[i].x, list[i].y);
          } else {
            drawBitmap(bulletS, list[i].x, list[i].y);
          }
        } else if (list[i].directionX == MOVING_LEFT){
          drawBitmap(bulletW, list[i].x, list[i].y);
        } else if (list[i].directionX == MOVING_RIGHT){
          drawBitmap(bulletE, list[i].x, list[i].y);
        }
      }
    }
  }
}
