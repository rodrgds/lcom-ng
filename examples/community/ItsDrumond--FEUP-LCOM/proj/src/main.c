/**
 * @file main.c
 * 
 */

#include <stdio.h>
#include <lcom/lcf.h>
#include "devices/ih.h"

extern bool finished;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (proj_main_loop)(){

    int flag = init_game();

    while(!finished){
        flag = proj_int();
        if (flag){
          
            vg_exit();
            return 1;
        } 
    }

   return close_game();
}
