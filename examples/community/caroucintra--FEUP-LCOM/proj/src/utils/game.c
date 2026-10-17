#include "game.h"
#include "helpers.h"
#include "keyboard.h"
#include "lcom/timer.h"
#include "mouse.h"
#include "video.h"
#include "xpms.h"
#include <lcom/lcf.h>

// State
// static States game_state;
// static MenuStates menu_state;
// static onButton on_button;
// static bool action_button;

Violetchi *violetchi;
Game game;
bool inGame = true;

// Device data
extern uint8_t scancode[2];
extern uint8_t bitno_timer, bitno_keyboard, bitno_mouse;
extern int global_counter;
extern int hunger_counter;
extern int to_sleep_counter;
extern int hygiene_counter;
extern int love_counter;
extern int play_counter;
extern int minute_counter;
extern int petting_counter;

void(start_game)() {
  game.state = MENU;
  game.button = PLAY_GAME;
  game.action = false;

  violetchi = create_violetchi(violetchi4);
  initialize_scene();
  game.violetchi = *violetchi;
  game.mouse.x = 99;
  game.mouse.y = 100;
  game.mouse.in_action = false;
  game.mouse.action_completed = false;
  game.mouse.packet = &packet;
}

int(play_game)(void) {

  uint8_t r;

  uint8_t irq_set_kbd = BIT(bitno_keyboard);
  uint8_t irq_set_timer = BIT(bitno_timer);
  uint8_t irq_set_mouse = BIT(bitno_mouse);

  int ipc_status;
  message msg;
  if (start_scene(game))
    return 1;
  change_buffer();

  while (inGame) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */
            kbc_ih();
            uint8_t size = 1;
            if (secondbyte)
              size = 2;
            get_key(scancode, size);
            update_state();
            update_arrow();
            start_scene(game);
            change_buffer();
          }
          if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
            mouse_ih();
            if (pack_done && game.state == GAME) {
              mouse_handle_pos();
              update_mouse();
              check_action();
              start_scene(game);
              change_buffer();
              pack_done = false;
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
            if (game.state != GAME)
              continue;

            timer_int_handler();

            if (game.action && game.button == LOVE) {
              if(check_end_love()){
                start_scene(game);
                change_buffer();
              }
              update_love_state();
            }

            if (game.action && game.button != LOVE) {
              if (global_counter == 60) {
                change_animation();
                start_scene(game);
                change_buffer();
              }
              
              if (global_counter == 120) {
                reset_violetchi();
                start_scene(game);
                change_buffer();
              }
            }

            if (verify_timer_events()) {
              update_state();
              start_scene(game);
              change_buffer();
            }
          }
          break;

        default:
          break;
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  return 0;
}

int update_state() {
  if ((scancode[0] == ESC || scancode[1] == ESC) && (game.state == GAME || game.state == END)) {
    game.state = MENU;
    game.button = PLAY_GAME;
    game.action = false;
    return 0;
  }

  if (game.violetchi.mood == DEAD) {
    game.violetchi.mood = HAPPY;
    game.state = END;
  }

  if (game.action) {
    if (game.state == MENU) {
      if (game.button == PLAY_GAME) {
        game.state = GAME;
        game.button = FOOD;
        reset_timer_counters();
        game.action = false;
      }
      else if (game.button == EXIT)
        inGame = false;
    }
    else if (game.state == GAME) {
      // update_violetchi_need(violetchi, game.button);
      update_buttons(game.violetchi.needs);
    }
  }
  return 0;
}

int update_love_state() {
  // if (game.mouse.in_action) love_animation();
  if (game.mouse.action_completed) {
    // update_violetchi_need(violetchi, game.button);
    game.action = false;
    game.mouse.action_completed = false;
  }
  return 0;
}
