#include "game.h"

extern uint32_t COUNTER; /* timer */
extern uint8_t scancode; /* mouse */
extern uint8_t rtc_time[3];
extern uint8_t last;

struct packet pack;
struct mouse_ev mouse_event;

GameState gameState = MAINMENU;

extern Cursor *cursor;
extern snakepart *snake;
extern apple *apple1;
extern int size; /* snake size */
extern int score; /* score of the player */

int(game_main_loop)() {

  int ipc_status, r;
  message msg;

  uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no,rtc_bit_no;

  if (kbd_subscribe_int(&kbd_bit_no) != 0) {
    return 1;
  }
  if (timer_subscribe_int(&timer_bit_no) != 0) {
    return 1;
  }
  if (mouse_enable_data_report() != 0) {
    return 1;
  }
  if (mouse_subscribe_int(&mouse_bit_no) != 0) {
    return 1;
  }

  if (rtc_subscribe_int(&rtc_bit_no) != 0){
    return 1;
  }

  int irq_kbd = BIT(kbd_bit_no);
  int irq_tmr = BIT(timer_bit_no);
  int irq_mouse = BIT(mouse_bit_no);
  int irq_rtc = BIT(rtc_bit_no);

  bool MouseReadSecond = false, MouseReadThird = false;
  uint8_t ms_bytes[3]; /* to store mouse bytes */

  loadMainMenu();

  while (gameState != EXIT) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_kbd) {
            kbc_ih();
            GeneralInterrupt(KEYBOARD);
          }
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            if (!MouseReadSecond && !MouseReadThird && (scancode & BIT(3))) {
              ms_bytes[0] = scancode;
              MouseReadSecond = true;
            }
            else if (MouseReadSecond) {
              ms_bytes[1] = scancode;
              MouseReadThird = true;
              MouseReadSecond = false;
            }
            else if (MouseReadThird) {
              ms_bytes[2] = scancode;
              MouseReadThird = false;
              for (unsigned int i = 0; i < 3; i++) {
                pack.bytes[i] = ms_bytes[i];
              }
              buildPacket(&pack);
              GeneralInterrupt(MOUSE);
            }
          }

          if (msg.m_notify.interrupts & irq_rtc){
            rtc_ih();
            GeneralInterrupt(RTC);
          }
          if (msg.m_notify.interrupts & irq_tmr) {
            timer_int_handler();
            GeneralInterrupt(TIMER);
            double_buffer();
          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }
  if (kbd_unsubscribe_int() != 0) {
    return 1;
  }
  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }
  if (mouse_disable_data_report() != 0) {
    return 1;
  }
  if (rtc_unsubscribe_int() != 0) {
    return 1;
  }

  if (vg_exit()) {
    return 1;
  }

  return 1;
}

void GeneralInterrupt(Device device) {

  switch (gameState) {
    case MAINMENU:
      MainMenuInterruptHandler(device);
      break;
     case SAVE:
      SaveInterruptHandler(device);
      break;
    case PLAY:
      PlayInterruptHandler(device);
      break;
    case SCOREBOARD:
      ScoreBoardInterruptHandler(device);
      break;
    case LOST:
      GameLostInterruptHandler(device);
      break;
    case EXIT:
      break;
  }
}

struct mouse_ev *mouse_events(struct packet *pack) {
  static bool lb_pressed = false, rb_pressed = false, mb_pressed = false;

  mouse_event.delta_x = pack->delta_x;
  mouse_event.delta_y = pack->delta_y;

  if (!lb_pressed && pack->lb && !rb_pressed && !mb_pressed && !pack->mb && !pack->rb) {
    lb_pressed = true;
    mouse_event.type = LB_PRESSED;
  }
  else if (!rb_pressed && pack->rb && !lb_pressed && !mb_pressed && !pack->mb && !pack->lb) {
    rb_pressed = true;
    mouse_event.type = RB_PRESSED;
  }
  else if (lb_pressed && !pack->lb && !rb_pressed && !mb_pressed && !pack->mb && !pack->rb) {
    lb_pressed = false;
    mouse_event.type = LB_RELEASED;
  }
  else if (rb_pressed && !pack->rb && !lb_pressed && !mb_pressed && !pack->mb && !pack->lb) {
    rb_pressed = false;
    mouse_event.type = RB_RELEASED;
  }
  else if (!mb_pressed && pack->mb) {
    mb_pressed = true;
    mouse_event.type = BUTTON_EV;
  }
  else if (mb_pressed && !pack->mb) {
    mb_pressed = false;
    mouse_event.type = BUTTON_EV;
  }
  else
    mouse_event.type = MOUSE_MOV;

  return &mouse_event;
}

void PlayInterruptHandler(Device device) {

  switch (device) {
    case TIMER:
     if(COUNTER % 4 == 0){
      check_snake_apple_collision(apple1);
      draw_sky();
      draw_score(750,90);
      draw_grass();
      draw_apple();
      draw_snake();
      define_snake_tail_sprite();
      snake_update_movement();
      define_snake_body_sprite();
      
      if (snake[size - 1].x >= 800 || snake[size - 1].x < 0 || snake[size - 1].y < 150 || snake[size - 1].y >= 600) {
        loadLostMenu();
        free(snake);
        free(apple1);
        gameState = LOST;
        break;
      }
      for (int i = 0; i < size - 1; i++) {
        if (snake[i].x == snake[size - 1].x && snake[i].y == snake[size - 1].y) {
          loadLostMenu();
          free(snake);
          free(apple1);
          gameState = LOST;
          break;
        }
      }
     }
      break;
    case KEYBOARD:
      snake_set_direction(last);
      break;
    case MOUSE: // NAO FAZER NADA
      break;
    case RTC:
      break;
  }
}

void(loadGame)() {
  xpm_load(day_xpm,XPM_8_8_8,&day_img);
  xpm_load(afternoon_xpm,XPM_8_8_8,&afternoon_img);
  xpm_load(night_xpm,XPM_8_8_8,&night_img);
  xpm_load(zero_xpm,XPM_8_8_8,&numbers_img[0]);
  xpm_load(one_xpm,XPM_8_8_8,&numbers_img[1]);
  xpm_load(two_xpm,XPM_8_8_8,&numbers_img[2]);
  xpm_load(three_xpm,XPM_8_8_8,&numbers_img[3]);
  xpm_load(four_xpm,XPM_8_8_8,&numbers_img[4]);
  xpm_load(five_xpm,XPM_8_8_8,&numbers_img[5]);
  xpm_load(six_xpm,XPM_8_8_8,&numbers_img[6]);
  xpm_load(seven_xpm,XPM_8_8_8,&numbers_img[7]);
  xpm_load(eight_xpm,XPM_8_8_8,&numbers_img[8]);
  xpm_load(nine_xpm,XPM_8_8_8,&numbers_img[9]);
  delete_xpm(menu_background_img,0,0);
  delete_xpm(logotip_img,250,50);

  create_grass();
  create_apple();
  create_snake();
}

void (draw_score)(int x, int y){
  int xx= 0;
  int aux = score;
  xx= 0;

  while (aux >= 0){ /* draw new score */
    int i = aux % 10;
    draw_xpm(numbers_img[i].bytes,&numbers_img[i],x-xx,y);
    xx += 20;
    aux = aux / 10;
    if (aux == 0) break;
  }
}

void draw_sky(){
  if(rtc_time[2]<15){
    draw_xpm(day_img.bytes,&day_img,0,0);
  }
  else if(rtc_time[2]<20){
    draw_xpm(afternoon_img.bytes,&afternoon_img,0,0);
  }
  else{
    draw_xpm(night_img.bytes,&night_img,0,0);
  }
}
