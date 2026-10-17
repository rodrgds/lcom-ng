#include "GameController.h"
#include "src/GUI/gui.h"

/**
 * @file GameController.c
 * @brief Implements core game loop logic, input handling, and state transitions.
 *
 * Manages game context, handles keyboard and mouse inputs,
 * updates timer, and coordinates updates to game systems like map and player.
 */

GameState gamestate = MENU_STATE;

void gamecontroller_init() {

  Position cur;
  cur.x = mode_info.XResolution / 2;
  cur.y = mode_info.YResolution / 2;
  ctx.cursor = new_cursor(cur, 1, 1);
}

void gamecontroller_exit() {

  delete_Cursor(ctx.cursor);
  delete_MapController();
}

Cursor *get_cursor() {
  return ctx.cursor;
}

void update_Keyboard() {
  kbc_ih();
  if (scan_code[0] == 0x81) {
    if (gamestate == GAME_STATE) {
      gamestate = PAUSE_STATE;
    }
    else if (gamestate == PAUSE_STATE) {
      gamestate = MENU_STATE;
    }
    else if (gamestate == MENU_STATE) {
      gamestate = EXIT_STATE;
    }
    else if (gamestate == OPTIONS_STATE) {
      gamestate = MENU_STATE;
    }
  }

  if (gamestate == GAME_STATE) {
    handle_player_input(scan_code);
    backpack_select_item();
  }
}
void update_Mouse() {
  mouse_ih();
}

void update_Timer() {


  if (!waiting_mouse) {
    waiting_mouse = true;
    Update_Mouse(get_cursor(), gamestate);
  }
  if (gamestate == GAME_STATE){
    timer_int_handler();
    if(counter_TIMER % 60 == 0) {
        update_map();
        update_Trees(get_MapController());
    }
    update_game();
  }
  draw();
}

void update_game() {
  update_player();
}

void change_video_mode(uint16_t mode) {

  if (vg_exit() != 0) {
    printf("Error exiting current video mode\n");
    return;
  }

  if(vg_map(mode) != 0) {
    printf("Error mapping video memory for mode %x\n", mode);
    return;
  }

  if (VG_init(mode) != 0) {
    printf("Error initializing video mode %x\n", mode);
    return;
  }

  setup_sprites(mode);
}
