#include <lcom/lcf.h>

// Same includes as in proj.c as well as mouse.h
#include "vbe.h"
#include "screen.h"
#include "mouse.h"
#include "timer.h"
#include "kbd.h"

// All the following header files are xpm used in the game
#include "screens/Game_Over.h"
#include "screens/Pos1.h"
#include "screens/Pos2.h"
#include "screens/Pos3.h"
#include "screens/Pos4.h"
#include "screens/Pos5.h"
#include "screens/PosF.h"
#include "screens/PosI.h"
#include "screens/PosM.h"
#include "screens/level_1.h"
#include "screens/level_2.h"
#include "screens/level_3.h"
#include "screens/level_4.h"
#include "screens/level_5.h"
#include "screens/score_000.h"
#include "screens/score_050.h"
#include "screens/score_100.h"
#include "screens/score_150.h"
#include "screens/score_200.h"
#include "screens/score_250.h"
#include "screens/screen_gameScreen.h"
#include "screens/screen_gameScreen2.h"
#include "screens/screen_mainMenu.h"
#include "screens/screen_instructions.h"
#include "screens/time_0.h"
#include "screens/time_1.h"
#include "screens/time_10.h"
#include "screens/time_2.h"
#include "screens/time_3.h"
#include "screens/time_4.h"
#include "screens/time_5.h"
#include "screens/time_6.h"
#include "screens/time_7.h"
#include "screens/time_8.h"
#include "screens/time_9.h"
#include "screens/Target.h"
#include "screens/mouse_pointer.h"
#include "screens/Paused.h"

// Global Variables for VBE
char *video_mem;
uint8_t *v_ptr;
uint16_t hres;
uint16_t vres;
uint16_t bits_per_pixel;

// Global Variables for Keyboard
uint8_t scancode;
extern int irq_set_kbd;

// Global Variable for Timer
extern int irq_set_timer;

// Global Variables for Mouse
extern bool ih_return;
extern uint32_t p_byte;
extern int counter_timer;
extern uint32_t cnt2;
extern uint8_t lb;
extern uint16_t x_mouse, y_mouse;
extern int hook_id_mouse;

// Global Variables for Main Menu Screen
uint8_t * sprite_mainMenu;
uint16_t width_mainMenu;
uint16_t height_mainMenu;

// Global Variables for Game Screen
uint8_t * sprite_gameScreen;
uint16_t width_gameScreen;
uint16_t height_gameScreen;

// Global Variables for level, score, cannon and time, respetively
int l;
int s;
int c;
int t;

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y) {
  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img;
  
  if (xpm == screen_mainMenu) { // If the XPM is the Main Menu, 
                                //save it's details in the global variables
    sprite_mainMenu = xpm_load(xpm, type, &img); 
    width_mainMenu = img.width;
    height_mainMenu = img.height;
  }
  
  if (xpm == screen_gameScreen) { // If the XPM is the Game Screen, 
                                  //save it's details in the global variables
    sprite_gameScreen = xpm_load(screen_gameScreen2, type, &img); 
    width_gameScreen = img.width;
    height_gameScreen = img.height;
  }
  
  uint8_t *sprite = xpm_load(xpm, type, &img);
  uint16_t width = img.width, height = img.height;

  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      uint32_t *color;
      color = (uint32_t *) (sprite + ((i * width + j) * ((bits_per_pixel + 7) / 8)));
      vg_draw_pixel(x + j, y + i, color);
    }
  }

  return 0;
}


int draw_xpm_transparent(xpm_map_t xpm, uint16_t x, uint16_t y) {
  enum xpm_image_type type = XPM_8_8_8;
  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, type, &img);
  uint16_t width = img.width, height = img.height;

  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      uint32_t *color;
      color = (uint32_t *) (sprite + ((i * width + j) * ((bits_per_pixel + 7) / 8)));
      vg_draw_pixel_transparent(x + j, y + i, color); // In this case, it will draw only the 
                                                      //non-white pixels
      }
    }

  return 0;
}

int draw_xpm_partial(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if (xpm == screen_mainMenu) { // Draws the XPM of Main Menu without loading it
    for (uint16_t i = 0; i < height_mainMenu; i++) {
      for (uint16_t j = 0; j < width_mainMenu; j++) {
        uint32_t *color;
        color = (uint32_t *) (sprite_mainMenu + ((i * width_mainMenu + j) * ((bits_per_pixel + 7) / 8)));
        vg_draw_pixel(x + j, y + i, color);
      }
    }
  }
  else { // Draws the XPM of Game Screen without loading it
    for (uint16_t i = 0; i < height_gameScreen; i++) {
      for (uint16_t j = 0; j < width_gameScreen; j++) {
        uint32_t *color;
        color = (uint32_t *) (sprite_gameScreen + ((i * width_gameScreen + j) * ((bits_per_pixel + 7) / 8)));
        vg_draw_pixel_transparent(x + j, y + i, color);
      }
    }
  }

  return 0;
}

void draw_target(int tg) {
  switch (tg) {
    case 1:
      draw_xpm_transparent(Target, 601, 549);
      break;
    case 2:
      draw_xpm_transparent(Target, 641, 549);
      break;
    case 3:
      draw_xpm_transparent(Target, 661, 549);
      break;
    case 4:
      draw_xpm_transparent(Target, 681, 549);
      break;
    case 5:
      draw_xpm_transparent(Target, 750, 549);
      break;
    default:
      break;
  }
}

void draw_man(int c) {
  switch (c) {
    case 1:
      draw_man_1();
      break;
    case 2:
      draw_man_2();
      break;
    case 3:
      draw_man_3();
      break;
    case 4:
      draw_man_4();
      break;
    case 5:
      draw_man_5();
      break;
    default:
      break;
  }
}

// Each of the following functions, except draw_man_5, have 3 for cycles
// corresponding to the 3 Homem Bala (Bullet Man) positions

void draw_man_1() {
  for (int i = 50; i <= 270; i++) {
    change_cannon(1);
    draw_xpm_transparent(PosI, i, 570-i);
  }
  for (int k = 271; k <= 330; k++) {
    change_cannon(1);
    draw_xpm_transparent(PosM, k, 300);
  }
  for (int j = 0; j <= 270; j++) {
    change_cannon(1);
    draw_xpm_transparent(PosF, j+331, 300+j);
  }
}

void draw_man_2() {
  for (int i = 50; i <= 290; i++) {
    change_cannon(2);
    draw_xpm_transparent(PosI, i, 570-i);
  }
  for (int k = 291; k <= 350; k++) {
    change_cannon(2);
    draw_xpm_transparent(PosM, k, 280);
  }
  for (int j = 0; j <= 290; j++) {
    change_cannon(2);
    draw_xpm_transparent(PosF, j+351, 280+j);
  }
}

void draw_man_3() {
  for (int i = 50; i <= 310; i++) {
    change_cannon(3);
    draw_xpm_transparent(PosI, i, 570-i);
  }
  for (int k = 301; k <= 350; k++) {
    change_cannon(3);
    draw_xpm_transparent(PosM, k, 260);
  }
  for (int j = 0; j <= 310; j++) {
    change_cannon(3);
    draw_xpm_transparent(PosF, j+351, 260+j);
  }
}

void draw_man_4() {
  for (int i = 50; i <= 330; i++) {
    change_cannon(4);
    draw_xpm_transparent(PosI, i, 570-i);
  }
  for (int k = 301; k <= 350; k++) {
    change_cannon(4);
    draw_xpm_transparent(PosM, k, 240);
  }
  for (int j = 0; j <= 330; j++) {
    change_cannon(4);
    draw_xpm_transparent(PosF, j+351, 240+j);
  }
}

void draw_man_5() {
  for (int i = 50; i <= 750; i++) {
    change_cannon(5);
    draw_xpm_transparent(PosM, i, 570);
  }
}

void draw_level(int l) {
  switch (l) {
    case 1:
      draw_xpm(level_1, 97, 39);
      break;
    case 2:
      draw_xpm(level_2, 97, 39);
      break;
    case 3:
      draw_xpm(level_3, 97, 39);
      break;
    case 4:
      draw_xpm(level_4, 97, 39);
      break;
    case 5:
      draw_xpm(level_5, 97, 39);
      break;
    default:
      break;
  }
}

void draw_score(int s) {
  switch (s) {
    case 0:
      draw_xpm(score_000, 660, 39);
      break;
    case 1:
      draw_xpm(score_050, 660, 39);
      break;
    case 2:
      draw_xpm(score_100, 660, 39);
      break;
    case 3:
      draw_xpm(score_150, 660, 39);
      break;
    case 4:
      draw_xpm(score_200, 660, 39);
      break;
    case 5:
      draw_xpm(score_250, 660, 39);
      break;
    default:
      break;
  }
}

void draw_time(int t) {
  switch (t) {
    case 0:
      draw_xpm(time_0, 350, 39);
      break;
    case 1:
      draw_xpm(time_1, 350, 39);
      break;
    case 2:
      draw_xpm(time_2, 350, 39);
      break;
    case 3:
      draw_xpm(time_3, 350, 39);
      break;
    case 4:
      draw_xpm(time_4, 350, 39);
      break;
    case 5:
      draw_xpm(time_5, 350, 39);
      break;
    case 6:
      draw_xpm(time_6, 350, 39);
      break;
    case 7:
      draw_xpm(time_7, 350, 39);
      break;
    case 8:
      draw_xpm(time_8, 350, 39);
      break;
    case 9:
      draw_xpm(time_9, 350, 39);
      break;
    case 10:
      draw_xpm(time_10, 350, 39);
      break;
    default:
    break;
  }
}

// The following functions draws the gameScreen2 overall, then the target and the cannot
// It's used in Homem Bala (Bullet Man) movement, to "clean" it's track

void change_cannon(int c) {
  switch (c) {
    case 1:
      draw_xpm_partial(screen_gameScreen2, 0, 0);
      draw_target(l);
      draw_xpm_transparent(Pos1, 0, 559);
      break;
    case 2:
      draw_xpm_partial(screen_gameScreen2, 0, 0);
      draw_target(l);
      draw_xpm_transparent(Pos2, 0, 559);
      break;
    case 3:
      draw_xpm_partial(screen_gameScreen2, 0, 0);
      draw_target(l);
      draw_xpm_transparent(Pos3, 0, 559);
      break;
    case 4:
      draw_xpm_partial(screen_gameScreen2, 0, 0);
      draw_target(l);
      draw_xpm_transparent(Pos4, 0, 559);
      break;
    case 5:
      draw_xpm_partial(screen_gameScreen2, 0, 0);
      draw_target(l);
      draw_xpm_transparent(Pos5, 0, 559);
      break;
    default:
      break;
  }
}

int home_screen() {

  int ipc_status;
  int r;
  message msg;

  int irq_set_mouse = BIT(hook_id_mouse);
  
  // Mouse Interruptions Subscribe
  mouse_subscribe();
  disable_mouse_ih();
  mouse_command_stream(KBC_EN_DATA);
  enable_mouse_ih();

  uint8_t packets[100], counter = 0;

  // Initialization of left click as 0 and coordinates x and y of the mouse
  lb = 0;
  x_mouse = 400;
  y_mouse = 300;

  // Variable to be set with the option selected in Main Menu
  int pir = 0;

  // Draws home screen as well as the mouse in initial position
  draw_xpm(screen_mainMenu, 0, 0);
  draw_xpm_transparent(mouse_pointer, x_mouse, y_mouse);

  while (scancode != 0x12 && scancode != 0x19 && scancode != 0x17 && pir == 0) {

    // The while cycle only ends when the user presses some key or mouse click over
    // some Main Menu option

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 
          if (msg.m_notify.interrupts & irq_set_kbd) { // Subscribed interrupt fo keyboard
            kbc_ih();
          }

          if (msg.m_notify.interrupts & irq_set_mouse) { // Subscribed interrupt fo mouse

            mouse_ih();

            switch (counter) { // Switch to read the mouse packages
              case 0:
                if (p_byte & BIT(3)) {
                  packets[0] = p_byte;
                  counter++;
                }
                break;

              case 1:
                packets[1] = p_byte;
                counter++;
                break;

              case 2:
                packets[2] = p_byte;
                counter++;
                break;
            }

            if (counter == 3) { // Draws the mainMenu and the mouse in new position
              counter = 0;
              parse(packets, &lb, &x_mouse, &y_mouse);
              draw_xpm_partial(screen_mainMenu, 0, 0);
              draw_xpm_transparent(mouse_pointer, x_mouse, y_mouse);
              if (lb == 1) { // If it's left click
                if (x_mouse >= 80 && x_mouse <= 189 && y_mouse >= 464 && y_mouse <= 513) {
                  // And if it was in Play option
                  pir = 1;
                } else if (x_mouse >= 266 && x_mouse <= 585 && y_mouse >= 464 && y_mouse <= 513) {
                  // And if it was in Instructions option
                  pir = 2;
                } else if (x_mouse >= 648 && x_mouse <= 752 && y_mouse >= 464 && y_mouse <= 513) {
                  // And if it was in Exit option
                  pir = 3;
                }
              }
            }
          }
          break;
        default: break;
      }
    }
    else { 
    }
  }

  // Mouse Interruptions Unsubscribe
  disable_mouse_ih();
  mouse_command_stream(KBC_DIS_DATA);
  enable_mouse_ih();
  mouse_unsubscribe();

  if (scancode == 0x19 || pir == 1) { // scancode key 'P' or left click in Play
    scancode = 0x00;
    game_screen();
  }

  if (scancode == 0x12 || pir == 3) { // scancode key 'E' or left click in Exit
    scancode = 0x00;
    return 1;
  }

  if (scancode == 0x17 || pir == 2) { // scancode key 'I' or left click in Instructions
    scancode = 0x00;
    instructions_menu();
  }

  return 0;
}

void gameover_screen() {
  draw_xpm(Game_Over, 100, 260);  // Draws the XPM of Game Over
  tickdelay(200);                 // during 2 seconds
  home_screen();                  // Then, goes back to Main Menu
}

void paused() {
  draw_xpm(Paused, 100, 260); // Draws the XPM of Paused
  
  int ipc_status;
  int r;
  message msg;

  while (scancode != 0x19) {

    // Only if the P key is pressed, the while loop ends

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 
          if (msg.m_notify.interrupts & irq_set_kbd) { // Subscribed interrupt for Keyboard
            kbc_ih();
          }

          break;
        default: break;
      }
    }
    else {
    }
  }

  scancode = 0x00;
  
  // Get back into the game, setting the time, cannon, level, score and target
  // where they were right before the Pause
  draw_xpm(screen_gameScreen, 0, 0);
  draw_time(t);
  change_cannon(c);
  draw_level(l);
  draw_score(s);
  draw_target(l);
}

void instructions_menu() {
  draw_xpm(screen_instructions, 0, 0); // Draws the XPM of Instructions Menu
  
  int ipc_status;
  int r;
  message msg;

  while (scancode != 0x12) {

    // Only if the E key key is pressed, the while loop ends

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 
          if (msg.m_notify.interrupts & irq_set_kbd) { // subscribed interrupt for Keyboard
            kbc_ih();
          }

          break;
        default: break;
      }
    }
    else {
    }
  }

  scancode = 0x00;
  home_screen();
}

void game_screen() {

  int ipc_status;
  int r;
  message msg;
  extern int counter;

  // Initial values for time, cannon, level and score, respectively
  t = 10;
  c = 1;
  l = 1;
  s = 0;

  // Draws game screen with time 00:10, level 1, score 000 
  // and cannon and target at default positions
  draw_xpm(screen_gameScreen, 0, 0);
  draw_time(t);
  change_cannon(c);
  draw_level(l);
  draw_score(s);
  draw_target(l);

  // Booleans to check if the game ended by Game Over of
  // for being completed
  bool gameOver = false;
  bool endOfGame = false;

  while (scancode != 0x12 && !gameOver && !endOfGame && t != -1) {

    // If the E key is pressed OR if it's gameOver OR 
    // if it's the endOfGame, the while loop ends

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 
          if (msg.m_notify.interrupts & irq_set_kbd) { // Subscribed interrupt for Keyboard
            kbc_ih();
            if (scancode == 0x1f && c != 5) { // If the key 'W' is pressed and cannon isn't already
              c++;                            // in the highest position
            }
            else if (scancode == 0x11 && c != 1) { // If the key 'S' is pressed and cannon isn't 
              c--;                                 // already in the lowest position
            }
            else if (scancode == 0x26) { // if the key 'L' (Launch) is pressed 
              draw_man(c);
              if (c != l) { // This is the secret to pass the game! (Level == Cannon Position)
                gameOver = true;
                break;
              }
              else if (l < 5) { // If the Game didn't reached it's end, passes to next level
                l++;
                s++;
                t = 10;
                c = 1;
                draw_xpm(screen_gameScreen, 0, 0);
                draw_time(t);
                change_cannon(c);
                draw_level(l);
                draw_score(s);
                draw_target(l);
              }
              else { // End of Game - Congratulations!
                endOfGame = true;
                break;
              }
            } else if (scancode == 0x19) { // If the key 'P' (Paused) was pressed
              scancode = 0x00;
              paused();
            }
            change_cannon(c);
          }
          if (msg.m_notify.interrupts & irq_set_timer) { // Subscribed interrupt for Timer
            timer_int_handler();
            if ((counter % 60) == 0) {
              draw_time(t);
              t--;
            }
          }
          break;
        default: break;
      }
    }
    else {
    }
  }

  scancode = 0x00;
  if (endOfGame) { // If the Game Ended
    home_screen();
  } else { // GameOver due to time or due to initial position error
    gameover_screen();
  }

}
