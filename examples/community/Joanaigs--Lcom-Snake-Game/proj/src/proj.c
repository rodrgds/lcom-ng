#include "proj.h"
#include "game.h"
#include "menu.h"
#include "instructions.h"
#include "gameOver.h"
#include "graphics.h"
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

void single_player() {
  if(singlePlayerMode()){
    printf("An error occurred\n");
    baseState = leave;
  }
  baseState = game_over;
}

void multi_player() {
  if( multiPlayerMode()){
    printf("An error occurred\n");
    baseState = leave;
  }
  baseState = game_over;
}

void gameOver() {
    gameOverPage();
}

void main_Menu(){
    mainMenuLoop();
}

void instructions_Menu(){
    instructionsLoop();
}

int(proj_main_loop)(int argc, char *argv[]) {
  bool running = true;
  if (vbe_get_mode_info(0x115, &vmi_p))
    return 1;
  vramMap();
  if (setMode(0x115))
    return 1;
  baseState = mainMenu;
  while (running) {
    switch (baseState) {
      case mainMenu:
        main_Menu();
        break;
      case singlePlayer:
        single_player();
        break;
      case multiPlayer:
        multi_player();
        break;
      case instructions:
          instructions_Menu();
        break;
      case game_over:
        gameOver();
        break;
      case leave:
        running = false;
        break;
    }
  }
  if (vg_exit())
    return 1;
  return 0;
}

