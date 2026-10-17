// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "i8042.h"
#include "gamehandler.h"
#include "rtc.h"

/** @defgroup proj proj
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief main function
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
 * @brief Project main loop
 * @return 0 if no errors occured, 1 otherwise
 */
int (proj_main_loop)(int argc, char *argv[]) {
  uint16_t mode = 0x117;

  if(vg_init_function(mode) != OK) return 1;

  mainMenu();

  if(vg_exit() != OK) return 1;

  return 0;
}
