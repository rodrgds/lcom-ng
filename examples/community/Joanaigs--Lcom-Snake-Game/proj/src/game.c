#include "cursor.h"
#include "graphics.h"
#include "header.h"
#include "i8042.h"
#include "images/mouse_cursor.xpm"
#include "images/game_background.xpm"
#include "keyboard.h"
#include "mouse.h"
#include "objects.h"
#include "apple.h"
#include "rtc.h"
#include "snake.h"
#include "timer.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

uint8_t fr_rate = 5;
int16_t speed = 40;

void (init_game)() {
    
    drawBackground();
   
    initAppleXPM();
    initHeaderElements();
    lives = 5;
    numOfApplesEaten = 0;
    gameTime = 0;
    numApples=0;
    Apple initialApple;
    initApple(&initialApple, 280, 200, red);
    drawApple(initialApple);
    init_snake();
    drawHeader();
    drawSnakeBody();
}

void (keyboard_interrupt)() {
    kbc_ih();
    if (scanCode[0] == D_MAKE_CODE || scanCode[1] == RIGHT_MAKE_CODE ||
        scanCode[0] == W_MAKE_CODE || scanCode[1] == UP_MAKE_CODE || scanCode[0] == S_MAKE_CODE ||
        scanCode[1] == DOWN_MAKE_CODE)
        start = true;
    if (strcmp(snakeBody[0].direction, "UP") == 0 || strcmp(snakeBody[0].direction, "DOWN") == 0) {
        if (scanCode[0] == D_MAKE_CODE || scanCode[1] == RIGHT_MAKE_CODE) {
            start = true;
            snakeBody[0].direction = "RIGHT";
            snakeBody[0].img = snakeBody[0].imgRight;
            snakeBody[0].map = snakeBody[0].mapRight;
        }
        if (scanCode[0] == A_MAKE_CODE || scanCode[1] == LEFT_MAKE_CODE) {
            start = true;
            snakeBody[0].direction = "LEFT";
            snakeBody[0].img = snakeBody[0].imgLeft;
            snakeBody[0].map = snakeBody[0].mapLeft;
        }
    } else if (strcmp(snakeBody[0].direction, "LEFT") == 0 ||
               strcmp(snakeBody[0].direction, "RIGHT") == 0) {
        if (scanCode[0] == W_MAKE_CODE || scanCode[1] == UP_MAKE_CODE) {
            start = true;
            snakeBody[0].direction = "UP";
            snakeBody[0].img = snakeBody[0].imgUp;
            snakeBody[0].map = snakeBody[0].mapUp;
        }
        if (scanCode[0] == S_MAKE_CODE || scanCode[1] == DOWN_MAKE_CODE) {
            start = true;
            snakeBody[0].direction = "DOWN";
            snakeBody[0].img = snakeBody[0].imgDown;
            snakeBody[0].map = snakeBody[0].mapDown;
        }
    }
}

int (singlePlayerMode)() {
    init_game();
    uint16_t frames = sys_hz() / fr_rate;
    int ipc_status, r;
    int n_interrupts = 0;
    message msg;
    uint8_t irq_keyboard = 0, irq_timer = 0, irq_rtc = 0;
    if (timer_subscribe(&irq_timer, 0))
        return 1;
    if (keyboard_subscribe(&irq_keyboard, 1))
        return 1;
    if (rtc_subscribe_int(&irq_rtc))
        return 1;
    set_periodic();
    set_update_int(true);
    start = false;
    while (scanCode[0] != ESC_BREAK_CODE) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_timer) {
                        n_interrupts++;
                        printf("%d", start);
                        if ((n_interrupts % frames == 0) && start) {
                            if (movement(speed)) {
                                if (rtc_unsubscribe_int())
                                    return 1;
                                if (set_update_int(false))
                                    return 1;
                                if (keyboard_unsubscribe())
                                    return 1;
                                if (timer_unsubscribe_int())
                                    return 1;
                                return 0;
                            }
                        }
                        if ((n_interrupts % sys_hz() == 0) && start) {
                            eraseTime();
                            gameTime++;
                            drawHeader();
                        }
                        copy_buffer_to_mem();
                    }

                    if (msg.m_notify.interrupts & irq_keyboard) {
                        keyboard_interrupt();
                    }
                    if (msg.m_notify.interrupts & irq_rtc) {
                        rtc_ih();
                        if (dealWithInterrupt) {
                            periodicApples();
                        }
                        dealWithInterrupt = false;
                    }

                    break;
                default:
                    break;
            }
        } else {
        }
    }
  scanCode[0]=0;
  if (rtc_unsubscribe_int())
    return 1;
  if (set_update_int(false))
    return 1;
  if (keyboard_unsubscribe())
    return 1;
  if (timer_unsubscribe_int())
    return 1;
  return 0;
}

int (multiPlayerMode)() {

    init_game();

    copy_buffer_to_mem();
    start = false;
    if (mouse_enable_data_report(true))
    return 1;
    uint16_t frames = sys_hz() / fr_rate;
    int ipc_status, r;
    int n_interrupts = 0;
    message msg;
    uint8_t irq_keyboard = 0, irq_timer = 0, mouse_set = 0;
    cursor c;
    c.x = 200;
    c.y = 200;
    xpm_load((xpm_map_t) mouse_cursor, XPM_8_8_8, &(c.img));

    if (timer_subscribe(&irq_timer, 2))
        return 1;
    if (keyboard_subscribe(&irq_keyboard, 3))
        return 1;
    if (mouse_subscribe(&mouse_set, 4))
        return 1;

  while (scanCode[0] != ESC_BREAK_CODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            n_interrupts++;
                        // movement draws the snake and moves her
            if (n_interrupts % frames == 0 && start) {
              if (movement(speed)) {
                if (mouse_enable_data_report(false))
                  return 1;
                if (timer_unsubscribe_int())
                  return 1;
                if (keyboard_unsubscribe())
                  return 1;
                if (mouse_unsubscribe())
                  return 1;
                return 0;
              }
            }
            if ((n_interrupts % sys_hz() == 0) && start) {
              eraseTime();
              gameTime++;
              drawHeader();
            }
            if (start) {
              copy_buffer_to_mem();
              draw_xpm_video_mem(c.img, c.img.bytes, c.x, c.y);
            }
          }

                    if (msg.m_notify.interrupts & irq_keyboard) {
                        keyboard_interrupt();
                    }
                    if (msg.m_notify.interrupts & mouse_set) {
                        mouse_ih();
                        if (mouse_number_bytes >= 3) {
                            struct packet p = parse_packet();
                            c.x += p.delta_x;
                            c.y -= p.delta_y;

                            // EVENTO DO CLIQUE
                            struct mouse_ev event = mouse_get_event(&p);
                            if (event.type == LB_RELEASED) {
                                placeApple(c.x, c.y, brown);
                            }
                            if (event.type == RB_RELEASED) {
                                placeApple(c.x, c.y, black);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
  scanCode[0]=0;
   if (mouse_enable_data_report(false))
    return 1;
  if (timer_unsubscribe_int())
    return 1;
  if (keyboard_unsubscribe())
    return 1;
  if (mouse_unsubscribe())
    return 1;
  return 0;
}
