/**
 * @file app_state.h
 * @brief Application state machine -- screen tags, gameplay state, and screen union.
 * @ingroup group_app
 *
 * The app tag determines which screen owns the current frame. The active
 * screen is stored in a tagged union: either a menu (main / instructions /
 * credits) or a gameplay context (scene + pause + result overlay).
 */

#ifndef PROJ_APP_STATE_H
#define PROJ_APP_STATE_H

#include "menu.h"
#include "overlay_menu.h"
#include "pause_menu.h"
#include "scene.h"

typedef enum {
  APP_SCREEN_MENU,
  APP_SCREEN_INSTRUCTIONS,
  APP_SCREEN_CREDITS,
  APP_SCREEN_MULTIPLAYER_WAIT,
  APP_SCREEN_MULTIPLAYER_ROLE,
  APP_SCREEN_PLAYING,
  APP_SCREEN_PAUSED,
  APP_SCREEN_GAME_OVER,
  APP_SCREEN_VICTORY,
  APP_SCREEN_EXIT,
} app_screen_tag_t;

typedef struct {
  scene_t scene;
  pause_menu_t pause;
  overlay_menu_t result;
} app_gameplay_state_t;

typedef struct {
  menu_t menu;
  overlay_menu_t role_menu;
} app_multiplayer_screen_state_t;

typedef union {
  menu_t menu;
  app_multiplayer_screen_state_t multiplayer;
  app_gameplay_state_t gameplay;
} app_screen_state_t;

#endif
