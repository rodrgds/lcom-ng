#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "constants.h"
#include "i8042.h"
#include "menus.h"
#include "mouse.h"
#include "pixmaps.h"
#include "sprite.h"
#include "video.h"

static Sprite *mouse;
uint8_t *main_menu_img;
uint8_t *instructions_img;

void create_mouse() {
  mouse = create_sprite(cursor, 576, 432, 0, 0);
}

void move_mouse() {
  if (mouse->x + mouse->xspeed >= 0 && mouse->x + mouse->xspeed < (int) get_hres()) {
    mouse->x += mouse->xspeed;
  }
  if (mouse->y - mouse->yspeed >= 0 && mouse->y - mouse->yspeed < (int) get_vres()) {
    mouse->y -= mouse->yspeed;
  }
}

void set_mouse_speed(int xspeed, int yspeed) {
  mouse->xspeed = xspeed;
  mouse->yspeed = yspeed;
}

void draw_mouse() {
  erase_pixmap(mouse->x, mouse->y, mouse->width, mouse->height);
  move_mouse();
  draw_pixmap(mouse->map, mouse->x, mouse->y, mouse->width, mouse->height);
  set_mouse_speed(0, 0);
}

void destroy_mouse() {
  destroy_sprite(mouse);
  mouse = NULL;
}

uint8_t *get_main_menu_pixmap() {
  return main_menu_img;
}

uint8_t *get_instructions_pixmap() {
  return instructions_img;
}

int draw_main_menu() {
  xpm_image_t img;
  main_menu_img = xpm_load(main_menu, XPM_8_8_8_8, &img);

  if (img.width != get_hres() || img.height != get_vres()) {
    printf("Main menu doesn't fit the screen\n");
    return 1;
  }

  draw_pixmap(main_menu_img, 0, 0, img.width, img.height);

  return 0;
}

int draw_instructions() {
  xpm_image_t img;
  instructions_img = xpm_load(instructions, XPM_8_8_8_8, &img);

  if (img.width != get_hres() || img.height != get_vres()) {
    printf("Instructions menu doesn't fit the screen\n");
    return 1;
  }

  draw_pixmap(instructions_img, 0, 0, img.width, img.height);

  return 0;
}

event_t menus(state_t st) {
  uint8_t r, irq_set_mouse = 0, irq_set_timer = 0;
  int ipc_status;
  message msg;
  extern uint8_t reader;
  struct packet pp;
  uint8_t cntByte = 1;
  uint8_t bytes[3];

  bool MOUSE_EVT;
  bool valid_click = false;
  event_t next_evt;

  mouse_data_report(SET_STREAM_MODE);

  mouse_data_report(ENABLE_DATA_REPORT);

  timer_subscribe_int(&irq_set_timer);

  mouse_subscribe_int(&irq_set_mouse);

  while (!valid_click) {
    MOUSE_EVT = false;
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

            draw_mouse();
          }
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { /* subscribed mouse interrupt */
            mouse_ih();

            if (cntByte == 1) {
              if ((reader & STAT_REG_A2) == 0)
                continue;
              else {
                bytes[0] = reader;
                cntByte++;
              }
            }
            else if (cntByte == 2) {
              bytes[1] = reader;
              cntByte++;
            }
            else {
              bytes[2] = reader;
              parsePacket(&pp, bytes); //Parse mouse packet
              cntByte = 1;             //Next byte will be the first
              MOUSE_EVT = true;
            }
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
    if (MOUSE_EVT) {
      set_mouse_speed(pp.delta_x, pp.delta_y);
      if (pp.lb == 0) {
        valid_click = false;
        continue;
      }
      if (st == MAIN_MENU) {
        if (mouse->x >= START_GAME_BUTTON_LEFT_LIMIT_PX && mouse->x <= START_GAME_BUTTON_RIGHT_LIMIT_PX && mouse->y >= START_GAME_BUTTON_TOP_LIMIT_PX && mouse->y <= START_GAME_BUTTON_BOTTOM_LIMIT_PX) {
          next_evt = START_GAME;
          valid_click = true;
        }
        else if (mouse->x >= INSTRUCTIONS_BUTTON_LEFT_LIMIT_PX && mouse->x <= INSTRUCTIONS_BUTTON_RIGHT_LIMIT_PX && mouse->y >= INSTRUCTIONS_BUTTON_TOP_LIMIT_PX && mouse->y <= INSTRUCTIONS_BUTTON_BOTTOM_LIMIT_PX) {
          next_evt = INSTRUCTIONS;
          valid_click = true;
        }
        else if (mouse->x >= EXIT_BUTTON_LEFT_LIMIT_PX && mouse->x <= EXIT_BUTTON_RIGHT_LIMIT_PX && mouse->y >= EXIT_BUTTON_TOP_LIMIT_PX && mouse->y <= EXIT_BUTTON_BOTTOM_LIMIT_PX) {
          next_evt = EXIT;
          valid_click = true;
        }
        else {
          valid_click = false;
        }
      }
      else if (st == INSTRUCTIONS_MENU) {
        if (mouse->x >= GO_BACK_BUTTON_LEFT_LIMIT_PX && mouse->x <= GO_BACK_BUTTON_RIGHT_LIMIT_PX && mouse->y >= GO_BACK_BUTTON_TOP_LIMIT_PX && mouse->y <= GO_BACK_BUTTON_BOTTOM_LIMIT_PX) {
          next_evt = GO_BACK_TO_MAIN_MENU;
          valid_click = true;
        }
        else {
          valid_click = false;
        }
      }
    }
  }

  mouse_unsubscribe_int();

  timer_unsubscribe_int();

  mouse_data_report(DISABLE_DATA_REPORT);

  return next_evt;
}
