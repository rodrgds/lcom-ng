// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/pixmap.h>

#include <stdbool.h>
#include <stdint.h>
#include <time.h>


// Any header files included below this line should have been created by you
#include "headers/images.h"
#include "headers/videocard.h"
#include "headers/keyboard.h"
#include "headers/mouse.h"
#include "headers/fruit.h"
#include "headers/gestures.h"
#include "headers/rtc.h"
#include "headers/serialport.h"
#include "headers/screen.h"
#include "headers/game.h"







extern uint8_t data[2];
extern bool kbd_valid;
extern bool two_byte_code;
extern uint32_t timer_counter;
extern struct packet pp;
extern unsigned mouse_counter;
extern enum mouse_state_t cutting_state;
extern int rand_seed;
Screen *current_screen, *main_screen, *singleplayer_screen, *endsp_game_screen, *help_screen, *multiplayer_screen;
Fruit* mouse_pointer;
uint32_t score, combo;

extern xpm_image_t digits[10];
extern xpm_image_t game_images[NUM_IMAGES];

double cut_slope;
uint16_t cut_xi, cut_xf, cut_yi, cut_yf;

bool spectator;

bool ser_read;


#define MAX_FRUITS 10

/**
 * @file proj.c
 * @brief Main proj file
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

/*
static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}
*/

/**
 * MAIN
*/
int(proj_main_loop)(int argc, char *argv[]) {
  if (argc == 1) {
    if (strcmp(argv[0],"player") == 0) {
      spectator = false;
    } else if (strcmp(argv[0],"spectator") == 0) {
      spectator = true;
    } else {
      printf("Invalid arguments. Only 'player' or 'spectator' allowed\n");
      return 1;
    }
  } else {
    printf("Invalid arguments. Only 'player' or 'spectator' allowed\n");
    return 1;
  }
  
  map_video_mem(VIDEO_MODE);

  if (set_vbe_mode(VIDEO_MODE)) {
    return 1;
  }  

  uint8_t kb_bitno, timer_bitno, mouse_bitno, rtc_bitno, ser_bitno;
  int ipc_status = 0;
  message msg;
  uint8_t r;

  //serialport_start();

  if(subscribe_serialport(&ser_bitno)) {
    return 1;
  }
  
  if (subscribe_rtc(&rtc_bitno)) {
    return 1;
  }
  
  if (subscribe_kbc(&kb_bitno)) {
    return 1;
  }

  if (timer_subscribe_int(&timer_bitno)) {
    return 1;
  }

  if (subscribe_mouse(&mouse_bitno)) {
      return 1;
  }

  disable_mouse();

  if (mouse_cmd_write(ENABLE_DR)) {
    return 1;
  }
    
  enable_mouse();

  uint8_t kb_irq_set = BIT(kb_bitno);
  uint8_t timer_irq_set = BIT(timer_bitno);
  uint8_t mouse_irq_set = BIT(mouse_bitno);
  uint8_t rtc_irq_set = BIT(rtc_bitno);
  //uint8_t ser_irq_set = BIT(ser_bitno);


  load_all_images();

  
  // Create mouse pointer sprite
  mouse_pointer = createFruit(game_images[KATANA_UP_IMG], game_images[KATANA_DOWN_IMG], 0,0,0,0,false,false);
  
  
  main_screen = loadMainScreen();
  singleplayer_screen = loadSinglePlayerScreen();
  endsp_game_screen = loadSinglePlayerEndScreen();
  help_screen = loadHelpScreen();

  if (!spectator) {
    multiplayer_screen = loadMultiPlayerScreen();
  } else {
    multiplayer_screen = loadSpectatorScreen();
  }

  current_screen = main_screen;

  while(data[0] != ESC) { 
    r = driver_receive(ANY, &msg, &ipc_status);
    if ( r != 0 ) { 
        printf("Error driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kb_irq_set) {
            kbc_ih();
            current_screen->menu_handler(KEYBOARD);
          }

          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            refresh_buffer();
            current_screen->menu_handler(TIMER);
          }

          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            current_screen->menu_handler(MOUSE);
          }
          if (msg.m_notify.interrupts & rtc_irq_set) {
            rtc_generate_seed();
          }
          //if (msg.m_notify.interrupts & ser_irq_set) {
            ser_ih();
            if (ser_read) {
              //printf("recieved message %d", ser_in);
              current_screen->menu_handler(SERIAL);
              ser_read = false;
            }
          //}
        default:
          break;
      }
    }
  }

  destroyScreen(main_screen);
  destroyScreen(singleplayer_screen);
  destroyScreen(help_screen);
  destroyScreen(endsp_game_screen);
  destroyScreen(multiplayer_screen);

  free(mouse_pointer);

  
  if(unsubscribe_serialport()) {
    return 1;
  }

  //serialport_end();
  
  if (unsubscribe_rtc()) {
    return 1;
  }

  if (unsubscribe_kbc()) {
    printf("Error unsubscribing from KBC!");
    return 1;
  }

  if (timer_unsubscribe_int()) {
    return 1;
  }
  
  disable_mouse();

  if (mouse_cmd_write(DISABLE_DR)) {
    return 1;
  }
  
  enable_mouse();

  if (unsubscribe_mouse() != 0 ){
    return 1;
  }

  vg_exit();
  
  return 0;
}
