// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "i8042.h"
#include "keyboard.h"
#include "video_gr.h"
#include "mouse.h"
#include "game.h"
#include "timer.h"
#include "xpmHandler.h"

extern uint8_t key_scancode;
bool finalState = false;
extern int global_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

  start();
  sleep(5);
  
  int option = 0;
  while(option!=4){
  option = mainMenu();
    if(option==1)
      singlePlayer();
     if(option==2)
       multiplayer();
     if(option==3)
       highscores();
     if(option==4)
       finish();
  }

  return 0;

}
