// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"
#include "gameOver.h"


bool running = true;
extern bool game;
extern bool menu;
extern bool exitOpt;
extern int hook_id_timer;
extern int hook_id_keyboard;
extern int hook_id_mouse;
extern uint16_t mode;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
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

/**
 * makes the subscrition of all the devices
 * @return
 */
int (ready_devices)(){
  //keyboard
  uint8_t aux_keyboard = (uint8_t)hook_id_keyboard;
  //mouse
  uint8_t aux_mouse = (uint8_t)hook_id_mouse;
  //timer
  uint8_t aux_timer = (uint8_t)hook_id_timer;

  if (subscribe_all(aux_timer, aux_keyboard, aux_mouse) != OK) return 1;
  
  return 0;
}

/**
 * unsubscribes all the devices
 * @return
 */
int (shutdown_devices)(){
  if (unsubscribe_all() != OK) return 1;
  return 0;
}

/**
 * main function of the project
 * @param argc
 * @param argv
 * @return
 */
int (proj_main_loop)(int argc, char* argv[])
{ 
  /* Available modes:  */
  //mode = 0x105;
  //mode = 0x110;
  mode = 0x115;
  //mode = 0x11A;
  //mode = 0x14C;
  /* ------------------ */
  /* 
  To choose a different mode just uncomment the mode you wish to use, comment the previous
  one and recompile the code.  
  */
  /* ------------------ */
  uint32_t irq_set_keyboard = BIT(hook_id_keyboard);
  uint32_t irq_set_mouse = BIT(hook_id_mouse);
  uint32_t irq_set_timer = BIT(hook_id_timer);
  if (ready_devices() != OK) return 1;

  while(running){
    menu_loop(irq_set_keyboard, irq_set_mouse, irq_set_timer);
    
    if (game) game_loop(irq_set_keyboard, irq_set_mouse, irq_set_timer, mode);
    if (menu) continue;
    if (!exitOpt) gameOver_loop(irq_set_keyboard, irq_set_mouse, irq_set_timer,mode);
    if (menu) continue;
    running = false;    
  }
  if (shutdown_devices() != OK) return 1;

  return 0;
}


