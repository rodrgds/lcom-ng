#include "controller.h"
#include "view/view.h"

#include "vector2.h"

#include "io/rtc/rtc.h"

#include <lcom/lcf.h>

// whether left button is down
static bool mouse_leftdown = 0;
extern uint16_t refresh_time;

static int timer_hook;
static int mouse_hook;
static int kbd_hook;

vec_2d mouse_pos = (vec_2d){150, 300};
int current_menu_item = 0;

int subscribe_all_int() {

  uint8_t timer_bitno;
  uint8_t kbd_bitno;
  uint8_t mouse_bitno;

  if (timer_subscribe_int(&timer_bitno) != 0) {
    return 1;
  }

  if (timer_set_frequency(0, GAME_FREQ) != 0) {
    return 1;
  }

  // disable mouse data reporting by default ?

  if (mouse_subscribe_int(&mouse_bitno) != 0) {
    return 1;
  }

  if (kbd_subscribe_int(&kbd_bitno) != 0) {
    return 1;
  }

  timer_hook = BIT(timer_bitno);
  mouse_hook = BIT(mouse_bitno);
  kbd_hook = BIT(kbd_bitno);

  return 0;
}

int unsubscribe_all_int() {

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

void init_controller() {

  scancode sc = {0};
  struct packet pkt;
  
  int ipc_status;
  message msg;

  while (sc.buf[0] != ESC_BREAK) { 

    int r = driver_receive(ANY, &msg, &ipc_status);
    if (r != OK) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

        // mouse interrupts
          if (msg.m_notify.interrupts & mouse_hook) {
            mouse_ih();
            if (mouse_parse_packet(&pkt) == 0) {
              on_mouse_event(&pkt);
            }
          }

        // kbd interrupts
          if (msg.m_notify.interrupts & kbd_hook) {
            kbc_ih();
            if (kbd_parse_scancode(&sc) == 0 && sc.make) {
              on_kbd_event(&sc);
            }
          }

        // timer interrupts
          if (msg.m_notify.interrupts & timer_hook) {
            timer_int_handler();
            on_timer_tick();
          }
        default:
          break;
      }
    }
  }
}

int on_timer_tick() {
  refresh_time++;
  if (game.state == MAIN_MENU) {
    render_menu_view(current_menu_item);
  }
  else if (game.state == ABOUT_MENU) {
    render_about_view();
  }
  else {
    render_game_view(&game, mouse_pos);
    if (game.state == GAME_OVER) {
      game.gameover_delay++;
      if (game.gameover_delay >= 300) {
        game.state = MAIN_MENU;
      }
    }
    if (game.state == AWAITING_PHYSICS) {
      update_physics(&mouse_pos);
    }
    if (game.state == WAITING_FOR_HIT || game.state == PREPARING_HIT) {
      game.current_turn_time++;
        if (game.current_turn_time / 60 >= 20) {
          handle_event(TOO_LONG_TO_HIT);
        }
    }
  }
  return 0;
}

int on_kbd_event(scancode *sc) {
  if (sc == NULL || !sc->make) {
    return 1;
  }
  
  if (sc->buf[0] == ENTER_MAKE) {
    // enter pressed
    if (game.state == MAIN_MENU) {
      if (current_menu_item == 0){
        game.state = AWAITING_PHYSICS;
        render_game_view(&game, mouse_pos);
      }
      if (current_menu_item == 1){
        game.state = ABOUT_MENU;
        render_about_view();
      }
    }
  }
  else if (sc->size && sc->buf[1] == ARROW_UP) {
    // arrow up key pressed
    current_menu_item = (current_menu_item - 1) % 2;
      if (current_menu_item < 0) {
        current_menu_item = 1;
      }
      render_menu_view(current_menu_item);
  }
  else if (sc->size && sc->buf[1] == ARROW_DOWN) {
    // arrow down key pressed
    current_menu_item = (current_menu_item + 1) % 2; 
    render_menu_view(current_menu_item);
  }
  else if(sc->buf[0] == B_LETTER) {
    if(game.state == ABOUT_MENU) {
      game.state = MAIN_MENU;
      render_menu_view(current_menu_item);
    }
  }
  return 0;
}

int on_mouse_event(struct packet *pkt) {
  if (pkt == NULL) {
    return 1;
  }
  if (pkt->lb && !mouse_leftdown) {
    mouse_leftdown = true;
    // mouse down
    if (game.state == WAITING_FOR_HIT && mouse_pos.x >= game.balls[0].pos.x - game.balls[0].radius && mouse_pos.x <= game.balls[0].pos.x + game.balls[0].radius && mouse_pos.y >= game.balls[0].pos.y - game.balls[0].radius && mouse_pos.y <= game.balls[0].pos.y + game.balls[0].radius) {
        game.state = PREPARING_HIT;
    }
  }
  else if (!pkt->lb && mouse_leftdown) {
    mouse_leftdown = false;
    // mouse up
    if (game.state == PREPARING_HIT) {
      vec_2d force = get_shot_force_dir(&game.balls[0], mouse_pos);
      game.balls[0].force = force;
      game.state = AWAITING_PHYSICS;
      game.current_turn_time = 0;
    }
  }
  if (game.state == WAITING_FOR_HIT || game.state == PREPARING_HIT) {
    if (!pkt->x_ov) mouse_pos.x += pkt->delta_x;
    if(!pkt->y_ov) mouse_pos.y -= pkt->delta_y;
    mouse_pos.x = MAX(mouse_pos.x, 0);
    mouse_pos.x = MIN(mouse_pos.x, 800);
    mouse_pos.y = MAX(mouse_pos.y, 0);
    mouse_pos.y = MIN(mouse_pos.y, 600);  
  }
  return 0;
}
