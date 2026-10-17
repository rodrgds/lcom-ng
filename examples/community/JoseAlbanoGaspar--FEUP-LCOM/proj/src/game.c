#include "game.h"

extern int count;
extern int snakeCount;
extern uint16_t scancode;
extern uint16_t mode;
extern struct packet mouse_packet; // data packet of 3 bytes
extern int mouseCount; //used to say in which byte of the mouse package we are in
extern struct Snake snake;
extern struct Enemy enemy;
extern int arena_x;
extern int arena_y;
bool menu = false;

int (game_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer, uint16_t vg_mode) {
  mode = vg_mode;
  int enemyCount = 0;
  /* Wait for ESC key */
  //Here we select the bit in the hook_id needed to check if we got the right interruption
  
//--------------------------------
  int ipc_status;
  message msg;
  //1 is true
  int r;
  bool ready_to_update = false;
  bool first_mouse = true;
  scancode = 0x0000;
  start_game();
  swapBuffer();
  //Main loop variable
  //bool running = true;
  while (scancode != ESC_KEY && snake.alive) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_timer) { // subscribed timer interrupt
            timer_int_handler();
            enemyCount++;
            if (enemyCount == (60*5)){
              enemyCount = 0;
              if (!enemy.active) spawnEnemy();
            }
            if (count == 5) {
              count = 0;
              drawGame();
              swapBuffer();
              if (ready_to_update){  
                if (checkClickEnemy(enemy.x, enemy.y)) killEnemy();
              }
            }
            if (snakeCount == 20){
              snakeCount = 0;
              if (canMove(snake.direction)) moveSnake(snake.direction);
              if (enemy.active) moveEnemy();
            }
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_keyboard) { // subscribed keyboard interrupt
            kbc_ih();
            if(scancode == P_KEY) pause_loop(irq_set_keyboard,irq_set_mouse);
            else changeDirection(scancode);
            kbc_reset_scancode();
          }

          // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed mouse interrupt
            mouse_ih();
            ready_to_update = false;
            mouseCount++; //received another packet
            if (mouseCount == 3){ //upon receiving the 3rd byte of a mouse packet, the program should parse it and print it on the console
                mouseCount = 0;
                ready_to_update = true;
                updateGameMouse();
                bool transition = false; //variable used to ensure the drawMouse function isn't called twice
                if (first_mouse){
                  drawMouse();
                  first_mouse = false;
                  transition = true;
                }
                if (checkMouseEnemy(enemy.x, enemy.y)) eraseMouse(true);   
                else {
                  eraseMouse(false);
                }
                if (!transition){
                  drawMouse();
                  transition = false;
                } 
                swapBuffer();
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  if (scancode == ESC_KEY) menu = true;
  scancode = 0x0000;
  return 0;
}


int (start_game)(){
  startPosition(mode);
  uint32_t arena_bg = 0x0, trim = 0x0, score = 0x0;
  if (mode == 0x115 || mode == 0x14C){
    arena_bg = ARENA_BACKGROUND_COLOR;
    trim = SCORE_BAR_TRIM_COLOR;
    score = SCORE_BAR_COLOR;
  }
  else if (mode == 0x110){
    arena_bg = ARENA_BACKGROUND_COLOR_110;
    trim = SCORE_BAR_TRIM_COLOR_110;
    score = SCORE_BAR_COLOR_110;
  }
  else if (mode == 0x105){
    arena_bg = ARENA_BACKGROUND_COLOR_105;
    trim = SCORE_BAR_TRIM_COLOR_105;
    score = SCORE_BAR_COLOR_105;
  }
  else if (mode == 0x11A){
    arena_bg = ARENA_BACKGROUND_COLOR_11A;
    trim = SCORE_BAR_TRIM_COLOR_11A;
    score = SCORE_BAR_COLOR_11A ;
  }
  vg_draw_rectangle(0, 0, arena_x, arena_y, arena_bg);
  vg_draw_rectangle(0, arena_y, arena_x, 80, trim);
  vg_draw_rectangle(5, arena_y+5, arena_x-10, 70, score);
  vg_ultimate_pixmap_handler(20,arena_y + 30,mode, SCORE);
  return 0;
}

void (drawGame)(){
  drawScore();
  drawSnake();
  drawApple();
  if (enemy.active) drawEnemy();
}


