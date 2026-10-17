#include <lcom/lcf.h>
#include "Bitmap.h"
#include "libs/video.h"
#include "leaderboard_menu.h"
#include "enum.h"
#include "program.h"
#include "mouse_proj.h"
#include "button.h"
#include "text.h"
#include "UI_game.h"
#include "main_menu.h"

Bitmap *title_leaderboard;
Bitmap *board_leaderboard;

struct Button but_return;
bool populated_leaderboard = false;
struct LeaderBoardEntry leaders[5];

extern enum program_state p_state;

void func_button_return(){
  p_state = MAIN_MENU;
  populated_leaderboard = false;
  destroy_leaderboard_menu();
  init_main_menu();
}

void init_leaderboard_menu(){
  init_mouse();
  init_button();
  title_leaderboard = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/title_leaderboard.bmp");
  board_leaderboard = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/board_leaderboard.bmp");
  but_return = create_button(h_res/2, v_res - 125, "Return", 6, func_button_return);
}

void destroy_leaderboard_menu(){
  destroy_mouse();
  destroy_button();
  deleteBitmap(title_leaderboard);
  deleteBitmap(board_leaderboard);
}

void populate_leaderboard(){
  FILE *fptr;
  fptr = fopen("/home/lcom/labs/lcom1819-t4g13/proj/leaderboard.txt","r");
  int ctn = 0;
  if(fptr != NULL){
    char tmp[100];
    while (fgets (tmp, 100, fptr) != NULL ){
      if (sscanf(tmp, "%d %8s %s\n", &leaders[ctn].score, leaders[ctn].date, leaders[ctn].name) == 3)
        ctn++;
    }
    fclose(fptr);
  }
  for (int i = ctn; i < 5; i++){
    leaders[i].score = 0;
    memset(leaders[i].date, 0, sizeof leaders[i].date);
    strncpy(leaders[i].date, "---", 3);
    memset(leaders[i].name, 0, sizeof leaders[i].name);
    strncpy(leaders[i].name, "---", 3);
  }
  // for (int i = 0; i < 5; i++)
  //   printf("%d %s %s\n", leaders[i].score, leaders[i].date, leaders[i].name);
}

void draw_leaderboard(){
  int y = 390;
  int x = 180;
  int y_inc = 80;
  char tmp[30];
  uint32_t tmp_size;
  for (int i = 0; i < 5; i++){
    sprintf(tmp, "%d", i+1);
    print_string(tmp, 1, y + i*y_inc, x, YELLOW, MEDIUM, ALIGN_LEFT);
    tmp_size = sprintf(tmp, "%s", leaders[i].name);
    tmp_size = (tmp_size > 13)? 13 : tmp_size;
    print_string(tmp, tmp_size, y + i*y_inc, x + 100, WHITE, MEDIUM, ALIGN_LEFT);
    tmp_size = sprintf(tmp, "%s", leaders[i].date);
    print_string(tmp, tmp_size, y + i*y_inc, x + 435, YELLOW, MEDIUM, ALIGN_LEFT);
    tmp_size = sprintf(tmp, "%d", leaders[i].score);
    print_string(tmp, tmp_size, y + i*y_inc, x + 935, WHITE, MEDIUM, ALIGN_RIGHT);
  }
}

void update_leaderboard_menu(){ // 105 260
  if (!populated_leaderboard){
    populate_leaderboard();
    populated_leaderboard = true;
  }

  draw_background();
  draw_title(title_leaderboard, 150);
  drawBitmap(board_leaderboard, 105, 260);
  draw_leaderboard();
  draw_button(but_return);
  draw_mouse();

  update_button(&but_return);
  
  flip_buff();
}
