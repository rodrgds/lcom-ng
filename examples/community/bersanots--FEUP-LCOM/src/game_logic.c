#include <lcom/lcf.h>

#include "elements.h"
#include "game_logic.h"
#include "i8042.h"
#include "keyboard.h"
#include "sprite.h"

winner win_player = DRAW;
int red_player_goals = 0, blue_player_goals = 0;
bool update_scoreboard = false;

void play() {
  uint8_t r, irq_set_kbd = 0, irq_set_timer = 0;
  int ipc_status;
  message msg;
  extern uint8_t code;
  bool two_byte = false;
  extern int cnt;

  bool KBD_EVT;

  kbd_subscribe_int(&irq_set_kbd);

  timer_subscribe_int(&irq_set_timer);

  cnt = 0;

  while (code != ESC_BREAKCODE || win_player == DRAW) {
    KBD_EVT = false;
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                        /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(irq_set_timer)) { /* subscribed timer interrupt */
            timer_int_handler();

            if (cnt % 60 == 0 && win_player == DRAW) //update every second
              update_time((cnt / 60) / 60, ((cnt / 60) % 60) / 10, ((cnt / 60) % 60) % 10);

            if ((cnt / 60) / 60 >= 3) {
              if (red_player_goals > blue_player_goals) {
                win_player = RED;
                draw_winner_message(win_player);
              }
              else if (blue_player_goals > red_player_goals) {
                win_player = BLUE;
                draw_winner_message(win_player);
              }
            }

            if (update_scoreboard) {
              update_scores(red_player_goals, blue_player_goals);
              update_scoreboard = false;
              if (red_player_goals == 3) {
                win_player = RED;
                draw_winner_message(win_player);
              }
              else if (blue_player_goals == 3) {
                win_player = BLUE;
                draw_winner_message(win_player);
              }
              else {
                for (int i = 0; i < NUM_ELEMENTS; i++) {
                  erase_sprite(get_elements()[i]);
                }
                reset_positions();
                for (int i = 0; i < NUM_ELEMENTS; i++) {
                  draw_sprite(get_elements()[i]);
                }
              }
            }
            else {
              if (win_player == DRAW)
                animate_sprites(get_elements());
            }
          }
          if (msg.m_notify.interrupts & BIT(irq_set_kbd)) { /* subscribed keyboard interrupt */
            kbc_ih();

            if (code == TWO_BYTE_SCANCODE) {
              two_byte = true;
              continue;
            }

            if (two_byte) {
              two_byte = false;
            }

            if (win_player == DRAW)
              KBD_EVT = true;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
    /* Now, do application dependent event handling */
    if (KBD_EVT) {
      handle_kbd(code);
      if (goal_scored())
        update_scoreboard = true;
    }
  }

  timer_unsubscribe_int();

  kbd_unsubscribe_int();
}

void handle_kbd(uint8_t code) {
  switch (code) {
    /* --- RED PLAYER --- */
    //move
    case A_MAKECODE:
      set_direction(get_elements()[RED_PLAYER_IDX], LEFT);
      break;
    case D_MAKECODE:
      set_direction(get_elements()[RED_PLAYER_IDX], RIGHT);
      break;
    case W_MAKECODE:
      set_direction(get_elements()[RED_PLAYER_IDX], UP);
      break;
    case S_MAKECODE:
      set_direction(get_elements()[RED_PLAYER_IDX], DOWN);
      break;

    //stop moving
    case A_BREAKCODE:
      clear_direction(get_elements()[RED_PLAYER_IDX], LEFT);
      break;
    case D_BREAKCODE:
      clear_direction(get_elements()[RED_PLAYER_IDX], RIGHT);
      break;
    case W_BREAKCODE:
      clear_direction(get_elements()[RED_PLAYER_IDX], UP);
      break;
    case S_BREAKCODE:
      clear_direction(get_elements()[RED_PLAYER_IDX], DOWN);
      break;

    /* --- BLUE PLAYER --- */
    //move
    case ARROW_LEFT_MAKECODE:
      set_direction(get_elements()[BLUE_PLAYER_IDX], LEFT);
      break;
    case ARROW_RIGHT_MAKECODE:
      set_direction(get_elements()[BLUE_PLAYER_IDX], RIGHT);
      break;
    case ARROW_UP_MAKECODE:
      set_direction(get_elements()[BLUE_PLAYER_IDX], UP);
      break;
    case ARROW_DOWN_MAKECODE:
      set_direction(get_elements()[BLUE_PLAYER_IDX], DOWN);
      break;

    //stop moving
    case ARROW_LEFT_BREAKCODE:
      clear_direction(get_elements()[BLUE_PLAYER_IDX], LEFT);
      break;
    case ARROW_RIGHT_BREAKCODE:
      clear_direction(get_elements()[BLUE_PLAYER_IDX], RIGHT);
      break;
    case ARROW_UP_BREAKCODE:
      clear_direction(get_elements()[BLUE_PLAYER_IDX], UP);
      break;
    case ARROW_DOWN_BREAKCODE:
      clear_direction(get_elements()[BLUE_PLAYER_IDX], DOWN);
      break;

    default:
      break;
  }
}

winner get_winner() {
  return win_player;
}

bool goal_scored() {
  if (get_elements()[BALL_IDX]->x + get_elements()[BALL_IDX]->width <= FIELD_LEFT_LIMIT_PX + 1) {
    blue_player_goals++;
    return true;
  }
  if (get_elements()[BALL_IDX]->x >= FIELD_RIGHT_LIMIT_PX - 1) {
    red_player_goals++;
    return true;
  }
  return false;
}
