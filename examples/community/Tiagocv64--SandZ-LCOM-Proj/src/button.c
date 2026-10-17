#include <lcom/lcf.h>
#include "libs/video.h"
#include "Bitmap.h"
#include "enum.h"
#include "button.h"
#include "text.h"
#include "mouse_proj.h"

Bitmap* button_up;
Bitmap* button_down;

extern int x_mouse;
extern int y_mouse;
extern bool left_press_mouse;

void init_button(){
  button_up = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/button_up.bmp");
  button_down = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/button_down.bmp");
}

void destroy_button(){
  deleteBitmap(button_up);
  deleteBitmap(button_down);
}

struct Button create_button(int x, int y, char* text, int t_size, void (*func)(void)){
  struct Button but;
  but.x = x;
  but.y = y;
  strncpy(but.text, text, t_size);
  but.t_size = t_size;
  but.func = func;
  but.mouseOver = false;
  return but;
}

void update_button(struct Button* but){
  if (but->x - 150 < x_mouse && but->x + 150 > x_mouse && but->y - 50 < y_mouse && but->y + 50 > y_mouse)
    but->mouseOver = true;
  else
    but->mouseOver = false;
  if (left_press_mouse && but->mouseOver)
    but->func();
}

void draw_button(struct Button but){
  if (but.mouseOver){
    drawBitmap(button_down, but.x - 150, but.y - 50);
  } else {
    drawBitmap(button_up, but.x - 150, but.y - 50);
  }
  char tmp[25];
  sprintf(tmp, "%s", but.text);
  print_string(tmp, but.t_size, but.y, but.x, WHITE, MEDIUM, ALIGN_CENTER);
}
