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
#include "leaderboard_menu.h"
#include "wait_player_menu.h"
#include "main_menu.h"

Bitmap *title_over;

struct Button but_retry;
struct Button but_main_menu;

bool saved_highscore_already = false;

extern bool two_player;
extern enum program_state p_state;
extern int points_game;
extern char player_name[50];
extern int player_name_size;
extern char date_rtc[20];

void func_button_retry(){
  if (two_player){
    p_state = WAIT_PLAYER_TWO;
    destroy_over_menu();
    init_wait_player_menu();
  } else {
    p_state = PLAYING;
    destroy_over_menu();
    init_game();
  }
  saved_highscore_already = false;
}

void func_button_main_menu(){
  p_state = MAIN_MENU;
  saved_highscore_already = false;
  destroy_over_menu();
  init_main_menu();
}

void init_over_menu(){
  init_mouse();
  init_button();
  title_over = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/title_over.bmp");
  but_retry = create_button(h_res/2, v_res/2 - 50, "Retry", 5, func_button_retry);
  but_main_menu = create_button(h_res/2, v_res/2 + 100, "Main menu", 9, func_button_main_menu);
}

void destroy_over_menu(){
  destroy_mouse();
  destroy_button();
  deleteBitmap(title_over);
}

void save_highscore(){
  FILE *fptr;
  fptr = fopen("/home/lcom/labs/lcom1819-t4g13/proj/leaderboard.txt","r");
  struct LeaderBoardEntry leaders[6];
  int ctn = 0;
  if(fptr != NULL){
    char tmp[100];
    while (fgets (tmp, 100, fptr) != NULL ){
      if (sscanf(tmp, "%d %8s %s\n", &leaders[ctn].score, leaders[ctn].date, leaders[ctn].name) == 3)
        ctn++;
    }
    fclose(fptr);
  }

  fptr = fopen("/home/lcom/labs/lcom1819-t4g13/proj/leaderboard.txt","w");

  struct LeaderBoardEntry current;
  current.score = points_game;
  memset(current.date, 0, sizeof current.date);
  strncpy(current.date, &date_rtc[9], 8);
  memset(current.name, 0, sizeof current.name);
  strncpy(current.name, player_name, player_name_size);

  int index = 5;
  for (int i = 0; i < ctn; i++){
    if (leaders[i].score < points_game){
      index = i;
      break;
    }
  }
  if (index == 5)
    index = ctn;

  for (int i = 4; i >= index; i--){
    leaders[i+1] = leaders[i];
  }
  leaders[index] = current;

  for (int i = 0; i < 5 && i <= ctn; i++)
    fprintf(fptr,"%d %.8s %s\n", leaders[i].score, leaders[i].date, leaders[i].name);

  fclose(fptr);
}

void update_over_menu(){
  if (!saved_highscore_already){
    save_highscore();
    saved_highscore_already = true;
  }

  draw_background();
  draw_title(title_over, 200);
  draw_button(but_retry);
  draw_button(but_main_menu);
  draw_mouse();

  update_button(&but_retry);
  update_button(&but_main_menu);
  
  flip_buff();
}
