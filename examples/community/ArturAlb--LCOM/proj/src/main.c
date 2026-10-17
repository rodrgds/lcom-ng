/**
 * @file main.c
 *
 */

#include <stdio.h>

#include "game/game.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("home/lcom/labs/proj/src/debug/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("home/lcom/labs/proj/src/debug/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int disable_video(int flag){
    video_stop();
    return flag;
}

int (proj_main_loop)(){
    int flag = game_start();
    if (flag) return disable_video(flag);

    flag = game_loop();
    if (flag) return disable_video(flag);

    flag = game_stop();
    if (flag) return disable_video(flag);

    return flag;
}
