#include "controllers/keyboard/i8042.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/timer/i8254.h"
#include "controllers/graphicsCard/graphics.h"
#include "sprites/sprites.h"
#include "game/game.h"
#include "xpm/hero.xpm"
#include "xpm/youwin.xpm"
#include "xpm/youlose.xpm"
#include "xpm/spaceInvaders.xpm"
#include "xpm/crusader.xpm"
#include "xpm/LVL.xpm"
#include "xpm/SCR.xpm"
#include "xpm/HP.xpm"
#include "global.h"
#include "menu/menu.h"
#include "xpm/numbers.xpm"
#include "view/game/gameView.h"
#include "view/menu/menuView.h"

Sprite* lvl = NULL;
Sprite* hp = NULL;
Sprite* scr = NULL;

int score =0;
int level =1;
int health =3;

xpm_map_t numbers[10] = {
  draw0_xpm, draw1_xpm, draw2_xpm, draw3_xpm, draw4_xpm,
  draw5_xpm, draw6_xpm, draw7_xpm, draw8_xpm, draw9_xpm
};



int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1; 
  lcf_cleanup();

  return 0;
}


int gameStart(){
  scr=create_static(drawscr_xpm,250,15);
  hp=create_static(drawhp_xpm,600,15);
  lvl=create_static(drawlvl_xpm,5,15);
  finalLost = create_static(drawYouLose_xpm, 250,350);
  finalWin = create_static(drawYouWin_xpm, 250,350);
  while (state != EXIT_MODE){
   
    if (state == MENU_MODE){
      if(menu_loop() != OK){return 1;}
    }
    
    if (state == PLAY_MODE){
      memset(double_buffer, 0, get_frame_size());
      update_screen();
      loadGame();
      if(drawGame() != OK){
        printf("Error DrawGame");
        return 1;
      }

      if(controllGameInterrupts()!=OK){
        printf("Error GameInts");
        return 1;
      }
      memset(double_buffer, 0, get_frame_size());
      update_screen();

      if (state == WON_MODE){
        drawWinButton();
      }
      if (state == LOST_MODE){
        drawLoseButton();
      }
      update_screen();
      keyboard_scan();
      destroygame();
      state = MENU_MODE;
    }
  }
  destroy_sprite(finalWin);
  destroy_sprite(finalLost);
  destroy_sprite(lvl);
  destroy_sprite(hp);
  destroy_sprite(scr);
  return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {
  state = MENU_MODE;
  if(set_frame(MODE_1024_768_IC)!=OK) return 1;
  if(set_video_mode(MODE_1024_768_IC)!=OK) return 1;
  memset(double_buffer, 0, get_frame_size());
  update_screen();
  if(gameStart() != OK) return 1;
  if (double_buffer != NULL) {
    free(double_buffer);
    double_buffer = NULL;
  }  
  vg_exit();
  return 0;
}

