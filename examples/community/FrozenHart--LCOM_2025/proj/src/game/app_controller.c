#include "app_controller.h"

application_state_t app_state;
bool multi_player_mode = false;
bool first_init = true;

int app_controller_init() {
  app_state = MAIN_MENU; // Start in the main menu state
  keyboard_init_scancode_map();
  return 0;
}

int app_controller_handle_application_state() {
  // printf("APP Handler: ");
  // printf("app state: %d\n", app_state);
  switch (app_state) {
    case MAIN_MENU:
      // Handle main menu logic
      break;
    case SINGLE_PLAYER:
      // Handle single player logic
      if (first_init) {
        if (game_controller_init()) {
          printf("Failed to initialize single player game\n");
          return 1;
        }
        first_init = false;
      }
      if (game_controller_game_state_handler() != 0) {
        printf("Failed fame controller state handler \n");
        return 1;
      }
      break;
    case MULTI_PLAYER:
      // Handle multi-player logic
      if (first_init) {
        if (game_controller_init()) {
          printf("Failed to initialize single player game\n");
          return 1;
        }
        first_init = false;
      }
      if (game_controller_game_state_handler() != 0) {
        printf("Failed fame controller state handler \n");
        return 1;
      }
      break;
    case EXIT:
      // Handle exit logic
      game_controller_free();
      break;
    default:
      printf("Invalid application state\n");
      break;
  }
  return 0;
}
