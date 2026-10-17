#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// The following header files are the ones corresponding to the I/O Devices used
// except the mouse, which is only handled in screen.c, the reason why we only
// included screen.h here and not mouse.h
#include "vbe.h"
#include "screen.h"
#include "timer.h"
#include "kbd.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages to PT-PT
  lcf_set_language("PT-PT");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

// Keyboard Variables
int irq_set_kbd;
int hook_id_kbd = 2;

// Timer Variables
extern int hook_id_timer;
int irq_set_timer;
uint8_t bit_no;

int(proj_main_loop)(int argc, char *argv[]) {

  /* Initialization of video mode */
  video_init(0x115);

  /* Subscription of Keyboard Interrupts */
  irq_set_kbd = BIT(hook_id_kbd);
  sys_irqsetpolicy(KBD_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &hook_id_kbd);
  
  /* Subscription of Timer Interrupts */
  irq_set_timer = BIT(hook_id_timer);
  timer_subscribe_int(&bit_no);

  /* Start of the game */
  int menu_select = 0;

  while(menu_select != 1) { // While the User doesn't press Exit in Main Menu
    menu_select = home_screen();
  }

  /* Unsubscription of Timer Interrupts */
  timer_unsubscribe_int();

  /* Unsubscription of Keyboard Interrupts */
  sys_irqrmpolicy(&hook_id_kbd);

  /* Returns to text mode */
  vg_exit();

  return 0;
}
