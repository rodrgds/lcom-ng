#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include "graphics.h"
#include "macros.h"
#include "pen_pal.h"
#include "CodeToCharacter.h"

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

  /* Initialize the graphics card mode */
  if (vgcard_init(DIRECT_MODE_16M) != OK){
    printf("Error: Failed to initialize graphics card mode\n");
    return 1;
  }

  /* Preemptively, create a list of breakcode - character associations */
  fill_code_list();

  int ret = play_game();

  vg_exit();

  return ret;
}
