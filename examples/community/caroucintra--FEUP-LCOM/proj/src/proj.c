#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "utils/keyboard.h"
#include "utils/mouse.h"
#include "utils/i8042.h"
#include "utils/video.h"
#include "images/soap_button_small.xpm"
#include <lcom/pixmap.h>
#include <lcom/xpm.h>
#include "utils/game.h"

int main(int argc, char *argv[]) 
{
  lcf_set_language("EN-US");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup(); 
  return 0;
}

uint8_t bitno_timer, bitno_keyboard, bitno_mouse;

int(proj_main_loop)(int argc, char *argv[]) {

  if (timer_subscribe_int(&bitno_timer)) {
    printf("timer_subscribe_int error in proj.c\n");
    return 1;
  }

  if (kbd_subscribe_int(&bitno_keyboard)) {
    printf("kbd_subscribe_int error in proj.c\n");
    return 1;
  }

  if (mouse_subscribe_int(&bitno_mouse)) {
    printf("mouse_subscribe_int error in proj.c\n");
    return 1;
  }

  if(enable_data_reporting()) return 1;

  if (init_vg(VBE_MODE)) {
    vg_exit();
    return 1;
  }

  start_game();

  play_game();

  if (kbd_unsubscribe_int()) {
    printf("kbd_unsubscribe_int error in proj.c\n");
    return 1;
  }

  if (timer_unsubscribe_int()) {
    printf("timer_unsubscribe_int error in proj.c\n");
    return 1;
  } 

  if (mouse_unsubscribe_int()) {
    printf("mouse_unsubscribe_int error in proj.c\n");
    return 1;
  } 

  if(disable_data_reporting()) return 1;

  if (vg_exit())
    return 1;

  return 0;
}
