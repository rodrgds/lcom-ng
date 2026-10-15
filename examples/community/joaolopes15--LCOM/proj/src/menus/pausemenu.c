#include "pausemenu.h"
#include "../assets/menus/continueS_xpm.h"
#include "../assets/menus/continue_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/pausedBIGGER_xpm.h"
#include "../assets/menus/paused_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/retry_xpm.h"

pause_menu_t *pause_menu_init() {
  pause_menu_t *pause_menu = (pause_menu_t *) malloc(sizeof(pause_menu_t));

  if (pause_menu == NULL) {
    printf("Error alocating memory for pause menu structure\n");
    return NULL;
  }

  pause_menu->paused_sprite = NULL;
  pause_menu->continueS_sprite = NULL;
  pause_menu->continue_sprite = NULL;
  pause_menu->retryS_sprite = NULL;
  pause_menu->retry_sprite = NULL;
  pause_menu->exittomenuS_sprite = NULL;
  pause_menu->exittomenu_sprite = NULL;

  pause_menu->paused_sprite = create_sprite((xpm_map_t) pausedBIGGER_xpm);
  if (pause_menu->paused_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->continueS_sprite = create_sprite((xpm_map_t) continueS_xpm);
  if (pause_menu->continueS_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->continue_sprite = create_sprite((xpm_map_t) continue_xpm);
  if (pause_menu->continue_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->retryS_sprite = create_sprite((xpm_map_t) retryS_xpm);
  if (pause_menu->retryS_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->retry_sprite = create_sprite((xpm_map_t) retry_xpm);
  if (pause_menu->retry_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->exittomenuS_sprite = create_sprite((xpm_map_t) exittomenuS_xpm);
  if (pause_menu->exittomenuS_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->exittomenu_sprite = create_sprite((xpm_map_t) exittomenu_xpm);
  if (pause_menu->exittomenu_sprite == NULL) {
    destroy_pause_menu(pause_menu);
    return NULL;
  }

  pause_menu->selected_option = 0;

  return pause_menu;
}

menu_action_t pausemenu_process_input(pause_menu_t *pause_menu, uint8_t scancode) {
  bool is_release = (scancode & 0x80) != 0;
  uint8_t key_code = scancode & 0x7F;

  if (!is_release) {
    if (scancode == 0x1C) { // enter key
      if (pause_menu->selected_option == 0) {
        return MENU_ACTION_START_GAME;
      }
      else if (pause_menu->selected_option == 1) {
        return MENU_ACTION_RETRY;
      }
      else if (pause_menu->selected_option == 2) {
        return MENU_ACTION_MAIN_MENU;
      }
    }
    else if (key_code == 0x48) { // up arrow
      if (pause_menu->selected_option > 0)
        pause_menu->selected_option--;
    }
    else if (key_code == 0x50) { // down arrow
      if (pause_menu->selected_option < 2)
        pause_menu->selected_option++;
    }
  }
  return MENU_ACTION_NONE;
}

menu_action_t pausemenu_process_mouse_click(pause_menu_t *pause_menu, int mouse_x, int mouse_y) {
  if (pause_menu == NULL) {
    return MENU_ACTION_NONE;
  }
  
  // Check if click is on "Continue" button (at position 250, 200)
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 200 && mouse_y <= 280) {
    pause_menu->selected_option = 0;
    return MENU_ACTION_START_GAME;
  }
  
  // Check if click is on "Retry" button (at position 250, 300)
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
    pause_menu->selected_option = 1;
    return MENU_ACTION_RETRY;
  }
  
  // Check if click is on "Exit to Menu" button (at position 250, 400)
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
    pause_menu->selected_option = 2;
    return MENU_ACTION_MAIN_MENU;
  }
  
  return MENU_ACTION_NONE;
}

void pausemenu_update_hover(pause_menu_t *pause_menu, int mouse_x, int mouse_y) {
  if (pause_menu == NULL) {
    return;
  }
  
  // Check if mouse is over "Continue" button (at position 250, 200)
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 200 && mouse_y <= 280) {
    pause_menu->selected_option = 0;
  }
  // Check if mouse is over "Retry" button (at position 250, 300)
  else if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
    pause_menu->selected_option = 1;
  }
  // Check if mouse is over "Exit to Menu" button (at position 250, 400)
  else if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
    pause_menu->selected_option = 2;
  }
}

void draw_pause_menu(pause_menu_t *pause_menu) {
  if (pause_menu == NULL) {
    return;
  }

  if (pause_menu->paused_sprite != NULL) {
    draw_sprite(pause_menu->paused_sprite, 200, 30);
  }

  if (pause_menu->selected_option == 0) {
    if (pause_menu->continueS_sprite != NULL) {
      draw_sprite(pause_menu->continueS_sprite, 250, 200);
    }
  }
  else {
    if (pause_menu->continue_sprite != NULL) {
      draw_sprite(pause_menu->continue_sprite, 250, 200);
    }
  }

  if (pause_menu->selected_option == 1) {
    if (pause_menu->retryS_sprite != NULL) {
      draw_sprite(pause_menu->retryS_sprite, 250, 300);
    }
  }
  else {
    if (pause_menu->retry_sprite != NULL) {
      draw_sprite(pause_menu->retry_sprite, 250, 300);
    }
  }

  if (pause_menu->selected_option == 2) {
    if (pause_menu->exittomenuS_sprite != NULL) {
      draw_sprite(pause_menu->exittomenuS_sprite, 250, 400);
    }
  }
  else {
    if (pause_menu->exittomenu_sprite != NULL) {
      draw_sprite(pause_menu->exittomenu_sprite, 250, 400);
    }
  }
}

void destroy_pause_menu(pause_menu_t *pause_menu) {
  if (pause_menu == NULL) {
    return;
  }

  if (pause_menu->paused_sprite != NULL) {
    destroy_sprite(pause_menu->paused_sprite);
    pause_menu->paused_sprite = NULL;
  }

  if (pause_menu->continueS_sprite != NULL) {
    destroy_sprite(pause_menu->continueS_sprite);
    pause_menu->continueS_sprite = NULL;
  }

  if (pause_menu->continue_sprite != NULL) {
    destroy_sprite(pause_menu->continue_sprite);
    pause_menu->continue_sprite = NULL;
  }

  if (pause_menu->retryS_sprite != NULL) {
    destroy_sprite(pause_menu->retryS_sprite);
    pause_menu->retryS_sprite = NULL;
  }

  if (pause_menu->retry_sprite != NULL) {
    destroy_sprite(pause_menu->retry_sprite);
    pause_menu->retry_sprite = NULL;
  }

  if (pause_menu->exittomenuS_sprite != NULL) {
    destroy_sprite(pause_menu->exittomenuS_sprite);
    pause_menu->exittomenuS_sprite = NULL;
  }

  if (pause_menu->exittomenu_sprite != NULL) {
    destroy_sprite(pause_menu->exittomenu_sprite);
    pause_menu->exittomenu_sprite = NULL;
  }

  free(pause_menu);
}
