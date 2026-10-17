#include <lcom/lcf.h>
#include "libs/video.h"
#include "Bitmap.h"
#include "obstacle.h"
#include <stdint.h>
#include <stdlib.h>

Bitmap* obstacles[7]; // 1-7

struct Obstacle list_obsts[5];

extern int h_res;
extern int v_res;

struct Obstacle create_obj(){
  struct Obstacle obst;
  obst.x = rand() % (h_res - 200);
  obst.y = rand() % (v_res - 200);
  obst.obsBit = obstacles[rand() % 7];
  obst.size_x = obst.obsBit->bitmapInfoHeader.width;
  obst.size_y = obst.obsBit->bitmapInfoHeader.height;
  return obst;
}

void init_obstacles(){  // srand(time(NULL)); on ZOMBIES, cant repeat
  char buf[100];
  for (int i = 0; i < 7; i++){
    snprintf(buf, sizeof(buf), "/home/lcom/labs/lcom1819-t4g13/proj/assets/obstacle/obj%d.bmp", i+1);
    obstacles[i] = loadBitmap(buf);
  }
  for (int i = 0; i < 5; i++){
    struct Obstacle tmp;
    tmp = create_obj();
    bool valid = true;
    for (int j = 0; j < i; j++){
      if (tmp.x + tmp.size_x >= list_obsts[j].x && tmp.x <= list_obsts[j].x + list_obsts[j].size_x && // COLLISION
          tmp.y + tmp.size_y >= list_obsts[j].y && tmp.y <= list_obsts[j].y + list_obsts[j].size_y){
        valid = false;
        i--;
        break;
      }
    }
    if (valid)
      list_obsts[i] = tmp;
  }
}

void destroy_obstacles(){
  for (int i = 0; i < 7; i++){
    deleteBitmap(obstacles[i]);
  }
}

void draw_obstacles(){
  for (int i = 0; i < 5; i++){
    drawBitmap(list_obsts[i].obsBit, list_obsts[i].x, list_obsts[i].y);
  }
}
