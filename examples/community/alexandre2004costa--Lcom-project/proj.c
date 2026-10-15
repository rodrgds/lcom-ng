#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "model.h"

/** @defgroup proj proj
 * @{
 */

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


/**
 * @brief Main loop
 */
int (proj_main_loop)(int argc, char *argv[]){
  if (map_memory(0X14C) != 0) return 1;
  if (change_graphic_mode(0x14C) != 0) return 1;
  loadSprites();
  timer_set_frequency(0, 60);
  if (enableInterrupts() != 0)return 1;
  waitForInterrupt();
  return EndDevices();    
}

/**@}*/
