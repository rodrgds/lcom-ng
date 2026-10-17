#include "gameOver.h"

extern uint16_t scancode;
extern uint16_t mode;
extern int arena_x;
extern int arena_y;
extern struct Snake snake;
extern bool menu;

int (gameOver_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer, uint16_t vg_mode) {
  mode = vg_mode;
  scancode = 0x0000;
  start_gameOver();
  swapBuffer();

//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;

  //Main loop variable
  //bool running = true;
  while (scancode != Q_KEY) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_keyboard) { // subscribed keyboard interrupt
            kbc_ih();

            if(scancode != Q_KEY) scancode = 0x0000;
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed mouse interrupt
            mouse_ih();
          }

        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  menu = true;
  scancode = 0x0000;
  return 0;
}

int (start_gameOver)(){

  uint32_t background_col=0X0;
    if (mode == 0x115 || mode == 0x14C){
      background_col = MENU_BACKGORUND;
  }
  else if (mode == 0x110){
      background_col = MENU_BACKGORUND_110;
  }
  else if (mode == 0x105){   
      background_col = MENU_BACKGORUND_105; 
    }
  else if (mode == 0x11A){
      background_col = MENU_BACKGORUND_11A;
  }

  vg_draw_rectangle(0, 0, h_res, v_res, background_col);
  //DRAW TITLE
  vg_ultimate_pixmap_handler(h_res / 2 - (GAMEOVER_WIDTH / 2), v_res / 8,mode, GAMEOVER);
  vg_ultimate_pixmap_handler(h_res / 2 - (TITLE_WIDTH / 2) - 20, 2 * v_res / 9,mode, TITLE);
  //DRAW GAMEPLAY STATUS
  unsigned long *day = (unsigned long*) malloc(sizeof(unsigned long));
  unsigned long *month = (unsigned long*) malloc(sizeof(unsigned long));
  unsigned long *year = (unsigned long*) malloc(sizeof(unsigned long));

  unsigned long *hours = (unsigned long*) malloc(sizeof(unsigned long));
  unsigned long *minutes = (unsigned long*) malloc(sizeof(unsigned long));
  unsigned long *seconds = (unsigned long*) malloc(sizeof(unsigned long));

  int i = 0;
  while (i < 5) {
    if (isRTCUpdating()) {
      printf("RTC UPDATING\n");
    } else {
      getDate(day, month, year);
      getHour(hours, minutes, seconds);
      printf("Hour of death: %lu:%lu\n", *hours, *minutes);
    }
    i++;
  }

  int x_init = h_res / 2 - (11 * NUMBER_WIDTH) / 2 - 20;
  drawNumbers(*day,x_init, 3 * v_res / 9, 2);
  x_init += NUMBER_WIDTH * 2;
  vg_ultimate_pixmap_handler(x_init,  3 * v_res / 9,mode,SLASH);
  x_init += NUMBER_WIDTH * 3;
  drawNumbers(*month,x_init, 3 * v_res / 9, 2);
  x_init += NUMBER_WIDTH * 2;
  vg_ultimate_pixmap_handler(x_init,  3 * v_res / 9,mode,SLASH);
  x_init += NUMBER_WIDTH * 5;
  drawNumbers(*year,x_init , 3 * v_res / 9, 4);

  int time_x = h_res / 2 - (5 * NUMBER_WIDTH) / 2;
  drawNumbers(*hours, time_x, 4 * v_res / 9,2);
  time_x += 2 * NUMBER_WIDTH;
  vg_ultimate_pixmap_handler(time_x, 4 * v_res / 9,mode,TWO_POINTS);
  time_x += 3 * NUMBER_WIDTH;
  drawNumbers(*minutes,time_x,4* v_res / 9,2);

  int x_pos = h_res / 2 - ( (SCORE_WIDTH + 4 * NUMBER_WIDTH + 100)/ 2);
  vg_ultimate_pixmap_handler(x_pos, 6 * v_res / 9,mode, SCORE);
  drawNumbers(snake.score,x_pos + SCORE_WIDTH + 4 * NUMBER_WIDTH + 50,6 * v_res / 9,4);

  //DRAW GO BACK INFO
  vg_ultimate_pixmap_handler(h_res / 2 - (GAME_O_BACK_1_WIDTH / 2), 7 * v_res / 9, mode, GO_BACK_1);
  vg_ultimate_pixmap_handler(h_res / 2 - (GAME_O_BACK_2_WIDTH / 2), 8 * v_res / 9, mode, GO_BACK_2);

  return 0;

}

