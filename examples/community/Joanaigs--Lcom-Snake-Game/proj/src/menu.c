#include "menu.h"
#include "graphics.h"
#include "images/menu_background.xpm"
#include "images/menu_exit.xpm"
#include "images/menu_instructions.xpm"
#include "images/menu_multiPlayer.xpm"
#include "images/menu_singlePlayer.xpm"
#include "images/mouse_cursor.xpm"
#include "macros.h"
#include "mouse.h"
#include "proj.h"
#include "objects.h"
#include "timer.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

static bool choose_instructions = false, choose_singlePlayer = false, choose_multiPlayer = false, choose_exit = false;
bool on_instructions = false, on_singlePlayer = false, on_multiPlayer = false, on_exit = false;


void init_menu() {
    main_menu.map = xpm_load((xpm_map_t) menu_xpm, XPM_8_8_8, &(main_menu.img));
    sp_menu.map = xpm_load((xpm_map_t) menu_singlePlayer_xpm, XPM_8_8_8, &(sp_menu.img));
    mp_menu.map = xpm_load((xpm_map_t) menu_multiPlayer_xpm, XPM_8_8_8, &(mp_menu.img));
    inst_menu.map = xpm_load((xpm_map_t) menu_instructions_xpm, XPM_8_8_8, &(inst_menu.img));
    exit_menu.map = xpm_load((xpm_map_t) menu_exit_xpm, XPM_8_8_8, &(exit_menu.img));
}

void drawMenu() {
  if (on_singlePlayer)
    draw_xpm(sp_menu.img, sp_menu.map, 0, 0);

  else if (on_multiPlayer)
    draw_xpm(mp_menu.img, mp_menu.map, 0, 0);

  else if (on_instructions)
    draw_xpm(inst_menu.img, inst_menu.map, 0, 0);

  else if (on_exit)
    draw_xpm(exit_menu.img, exit_menu.map, 0, 0);

  else
    draw_xpm(main_menu.img, main_menu.map, 0, 0);
  copy_buffer_to_mem();
}

int menuOptionCollisions(cursor *mouse_c) {
  if (mouse_c->y >= 240 && mouse_c->y <= 297 && mouse_c->x >= 194 && mouse_c->x <= 371)
    return 1;
  else if (mouse_c->y >= 240 && mouse_c->y <= 297 && mouse_c->x >= 398 && mouse_c->x <= 575)
    return 2;
  else if (mouse_c->y >= 325 && mouse_c->y <= 382 && mouse_c->x >= 185 && mouse_c->x <= 566)
    return 3;
  else if (mouse_c->y >= 415 && mouse_c->y <= 472 && mouse_c->x >= 185 && mouse_c->x <= 566)
    return 4;
  return 0;
}

int menu(cursor *c, struct packet *p) {
  struct mouse_ev event = mouse_get_event(p);

  switch (menuOptionCollisions(c)) {
    case 1:
      if (event.type == LB_RELEASED) {
        choose_singlePlayer = true;
        break;
      }
      if (!on_singlePlayer) {
        on_singlePlayer = true;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      break;
    case 2:
      if (event.type == LB_RELEASED) {
        choose_multiPlayer = true;
        break;
      }
      if (!on_multiPlayer) {
        on_multiPlayer = true;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      break;
    case 3:
      if (event.type == LB_RELEASED) {
        choose_instructions = true;
        break;
      }
      if (!on_instructions) {
        on_instructions = true;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      break;
    case 4:
      if (event.type == LB_RELEASED) {
        choose_exit = true;
        break;
      }
      if (!on_exit) {
        on_exit = true;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      break;

    case 0:
      if (on_singlePlayer) {
        on_singlePlayer = false;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      else if (on_multiPlayer) {
        on_multiPlayer = false;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      else if (on_instructions) {
        on_instructions = false;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      else if (on_exit) {
        on_exit = false;
        drawMenu();
        copy_buffer_to_mem();
        draw_xpm_video_mem(c->img, c->img.bytes, c->x, c->y);
      }
      break;
  }

  if (choose_singlePlayer) {
    baseState = singlePlayer;
    set_false_buttons();
    return 1;
  }
  else if (choose_multiPlayer) {
    baseState = multiPlayer;
    set_false_buttons();
    return 1;
  }
  else if (choose_instructions) {
    baseState = instructions;
    set_false_buttons();
    return 1;
  }
  else if (choose_exit) {
    baseState = leave;
    set_false_buttons();
    return 1;
  }
  return 0;
}

int mainMenuLoop() {
  cursor c;
  c.x = 200;
  c.y = 200;
  xpm_load((xpm_map_t) mouse_cursor, XPM_8_8_8, &(c.img));
  copy_buffer_to_mem();
  init_menu();
  drawMenu();
  if (mouse_enable_data_report(true))
        return 1;
  message msg;
  int ipc_status, r;
  uint8_t irq_timer = 0, irq_mouse = 2;

  if (timer_subscribe(&irq_timer, 0))
    return 1;
  if (mouse_subscribe(&irq_mouse, 1))
    return 1;

  int good = 1;
  while (good) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) {
            copy_buffer_to_mem();
            draw_xpm_video_mem(c.img, c.img.bytes, c.x, c.y);
          }

          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            if (mouse_number_bytes >= 3) {
              struct packet p = parse_packet();
              c.x += p.delta_x;
              c.y -= p.delta_y;
              if (menu(&c, &p))
                good = 0;
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }
  if (mouse_enable_data_report(false))
        return 1;
  if (timer_unsubscribe_int())
    return 1;
  if (mouse_unsubscribe())
    return 1;
  return 0;
}

void set_false_buttons(){
    choose_instructions = false;
    choose_singlePlayer = false;
    choose_multiPlayer = false;
    choose_exit = false;
    on_instructions = false;
    on_singlePlayer = false;
    on_multiPlayer = false;
    on_exit = false;
}
