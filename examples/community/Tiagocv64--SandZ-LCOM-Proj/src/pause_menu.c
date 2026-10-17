#include <lcom/lcf.h>
#include "libs/video.h"
#include "libs/rtc.h"
#include "Bitmap.h"
#include "enum.h"
#include "over_menu.h"
#include "button.h"
#include "program.h"
#include "mouse_proj.h"
#include "UI_game.h"
#include "game.h"
#include "pause_menu.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

Bitmap *title_pause;

struct Button but_resume;
struct Button but_quit;

extern enum program_state p_state;

int img[1024][1280];
int blue[1024][1280];
int red[1024][1280];
int green[1024][1280];
int blur[1024][1280];

bool back_done = false;


void func_button_resume(){
  back_done = false;
  p_state = PLAYING;
  destroy_pause_menu();
  resume_game();
}

void func_button_quit(){
  back_done = false;
  p_state = END_GAME;
  destroy_pause_menu();
  init_over_menu();
}

void init_pause_menu(){
  init_mouse();
  init_button();
  title_pause = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/title_pause.bmp");
  but_resume = create_button(h_res/2, v_res/2 - 50, "Resume", 6, func_button_resume);
  but_quit = create_button(h_res/2, v_res/2 + 100, "Quit game", 9, func_button_quit);
}

void destroy_pause_menu(){
  destroy_mouse();
  destroy_button();
  deleteBitmap(title_pause);
}

void load_background_blur(){
  for (int y = 0; y < 1024; y++)
    for (int x = 0; x < 1280; x++){
      img[y][x] = *(aux_v_mem + (y * 1280 + x)*2);
      img[y][x] += *(aux_v_mem + (y * 1280 + x)*2 + 1) << 8;
    }
  for (int y = 0; y < 1024; y++)
    for (int x = 0; x < 1280; x++){
      blue[y][x] = img[y][x] & 0x1F;
      green[y][x] = (img[y][x] >> 5) & 0x3F;
      red[y][x] = (img[y][x] >> 11) & 0x1F;
    }
  for (int y = 1; y < 1024; y++)
    for (int x = 0; x < 1280; x++){
      blue[y][x] += blue[y-1][x];
      green[y][x] += green[y-1][x];
      red[y][x] += red[y-1][x];
    }

  for (int y = 0; y < 1024; y++)
    for (int x = 1; x < 1280; x++){
      blue[y][x] += blue[y][x-1];
      green[y][x] += green[y][x-1];
      red[y][x] += red[y][x-1];
    }

  int r = 10;
  int w = (r*2+1)*(r*2+1);

  for (int y = 0; y < 1024; y++)
    for (int x = 0; x < 1280; x++){
      int l = max(x-r-1, 0), ri = min(x+r, 1279), u = max(0, y-r-1), d = min(1023, y+r);
      blur[y][x] = ((blue[u][l]-blue[d][l]-blue[u][ri]+blue[d][ri])/w) & 0x1F;
      blur[y][x] += (((green[u][l]-green[d][l]-green[u][ri]+green[d][ri])/w) << 5) & 0x07E0;
      blur[y][x] += (((red[u][l]-red[d][l]-red[u][ri]+red[d][ri])/w) << 11) & 0xF800;
    }
  // for (int y = 0; y < 1024; y++)
  //   for (int x = 0; x < 1280; x++){
  //     green[y][x] = green[y][x] >> 1;
  //     blur[y][x] = (blue[y][x] + red[y][x] + green[y][x])/3;
  //   }
}

void draw_background_blur(){
  for (int y = 0; y < 1024; y++)
    for (int x = 0; x < 1280; x++){
      *(aux_v_mem+(y*h_res+x)*2) = (u_char)(blur[y][x]) & 0xFF;
      *(aux_v_mem+(y*h_res+x)*2+1) = (u_char)(blur[y][x] >> 8) & 0xFF;
    }
}

void update_pause_menu(){
  if (!back_done){
    load_background_blur();
    back_done = true;
  }

  draw_background_blur();
  draw_title(title_pause, 200);
  draw_button(but_resume);
  draw_button(but_quit);
  draw_mouse();

  update_button(&but_resume);
  update_button(&but_quit);
  
  flip_buff();
}
