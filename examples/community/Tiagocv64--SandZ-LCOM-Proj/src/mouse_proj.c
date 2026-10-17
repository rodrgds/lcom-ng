#include <lcom/lcf.h>
#include "Bitmap.h"
#include "mouse_proj.h"
#include "libs/video.h"

int x_mouse = 200;
int y_mouse = 200;
bool left_mouse = false;
bool left_press_mouse = false;

Bitmap* mouse_icon;

extern int h_res;
extern int v_res;

void init_mouse(){
  x_mouse = h_res/2;
  y_mouse = v_res/2;
  mouse_icon = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/mouse_icon.bmp");
}

void destroy_mouse(){
  deleteBitmap(mouse_icon);
}

void draw_mouse(){
  drawBitmap(mouse_icon, x_mouse, y_mouse);
}

void update_mouse(struct packet pak){
  x_mouse += pak.delta_x;
  y_mouse -= pak.delta_y;
  x_mouse = (x_mouse < 0)? 0 : x_mouse;
  x_mouse = (x_mouse > h_res)? h_res : x_mouse;
  y_mouse = (y_mouse < 0)? 0 : y_mouse;
  y_mouse = (y_mouse > v_res)? v_res : y_mouse;
  left_press_mouse = false;
  if (left_mouse != pak.lb && pak.lb)
    left_press_mouse = true;
  left_mouse = pak.lb;
}
