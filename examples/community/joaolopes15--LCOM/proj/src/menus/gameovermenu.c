#include "gameovermenu.h"
#include "../assets/menus/exitS_xpm.h"
#include "../assets/menus/exit_xpm.h"
#include "../assets/menus/exittomenuS_xpm.h"
#include "../assets/menus/exittomenu_xpm.h"
#include "../assets/menus/gameoverBIGGER_xpm.h"
#include "../assets/menus/gameover_xpm.h"
#include "../assets/menus/highscore_xpm.h"
#include "../assets/menus/retryS_xpm.h"
#include "../assets/menus/retry_xpm.h"

game_over_menu_t *game_over_menu_init() {
  game_over_menu_t *game_over_menu = (game_over_menu_t *) malloc(sizeof(game_over_menu_t));

  if (game_over_menu == NULL) {
    printf("Error alocating memory for game over structure\n");
    return NULL;
  }

  game_over_menu->selected_option = 0;

  game_over_menu->gameover_sprite = NULL;
  // game_over_menu->highscore_sprite = NULL;
  game_over_menu->retry_sprite = NULL;
  game_over_menu->retryS_sprite = NULL;
  game_over_menu->exittomenu_sprite = NULL;
  game_over_menu->exittomenuS_sprite = NULL;
  game_over_menu->exit_sprite = NULL;
  game_over_menu->exitS_sprite = NULL;

  game_over_menu->gameover_sprite = create_sprite((xpm_map_t) gameoverBIGGER_xpm);
  if (game_over_menu->gameover_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  /* game_over_menu->highscore_sprite = create_sprite((xpm_map_t) highscore_xpm);
  if (game_over_menu->highscore_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  } */

  game_over_menu->retry_sprite = create_sprite((xpm_map_t) retry_xpm);
  if (game_over_menu->retry_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  game_over_menu->retryS_sprite = create_sprite((xpm_map_t) retryS_xpm);
  if (game_over_menu->retryS_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  game_over_menu->exittomenu_sprite = create_sprite((xpm_map_t) exittomenu_xpm);
  if (game_over_menu->exittomenu_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  game_over_menu->exittomenuS_sprite = create_sprite((xpm_map_t) exittomenuS_xpm);
  if (game_over_menu->exittomenuS_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  game_over_menu->exit_sprite = create_sprite((xpm_map_t) exit_xpm);
  if (game_over_menu->exit_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  game_over_menu->exitS_sprite = create_sprite((xpm_map_t) exitS_xpm);
  if (game_over_menu->exitS_sprite == NULL) {
    destroy_game_over(game_over_menu);
    return NULL;
  }

  return game_over_menu;
}

menu_action_t gameovermenu_process_input(game_over_menu_t *game_over_menu, uint8_t scancode) {
  bool is_release = (scancode & 0x80) != 0;
  uint8_t key_code = scancode & 0x7F;

  if (!is_release) {
    if (scancode == 0x1C) { // enter key
      if (game_over_menu->selected_option == 0) {
        return MENU_ACTION_RETRY;
      }
      else if (game_over_menu->selected_option == 1) {
        return MENU_ACTION_MAIN_MENU;
      }
      else if (game_over_menu->selected_option == 2) {
        return MENU_ACTION_EXIT;
      }
    }
    else if (key_code == 0x48) { // up arrow
      if (game_over_menu->selected_option > 0)
        game_over_menu->selected_option--;
    }
    else if (key_code == 0x50) { // down arrow
      if (game_over_menu->selected_option < 2)
        game_over_menu->selected_option++;
    }
  }
  return MENU_ACTION_NONE;
}

menu_action_t gameovermenu_process_mouse_click(game_over_menu_t *game_over_menu, int mouse_x, int mouse_y) {
  if (game_over_menu == NULL) {
    return MENU_ACTION_NONE;
  }
  
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
    game_over_menu->selected_option = 0;
    return MENU_ACTION_RETRY;
  }
  
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
    game_over_menu->selected_option = 1;
    return MENU_ACTION_MAIN_MENU;
  }
  
  if (mouse_x >= 220 && mouse_x <= 450 && mouse_y >= 500 && mouse_y <= 580) {
    game_over_menu->selected_option = 2;
    return MENU_ACTION_EXIT;
  }
  
  return MENU_ACTION_NONE;
}

void gameovermenu_update_hover(game_over_menu_t *game_over_menu, int mouse_x, int mouse_y) {
  if (game_over_menu == NULL) {
    return;
  }
  
  if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 300 && mouse_y <= 380) {
    game_over_menu->selected_option = 0;
  } else if (mouse_x >= 250 && mouse_x <= 450 && mouse_y >= 400 && mouse_y <= 480) {
    game_over_menu->selected_option = 1;
  } else if (mouse_x >= 220 && mouse_x <= 450 && mouse_y >= 500 && mouse_y <= 580) {
    game_over_menu->selected_option = 2;
  }
}

void draw_game_over(game_over_menu_t *game_over_menu) {
    if (game_over_menu == NULL) {
        return;
    }

    if (game_over_menu->gameover_sprite != NULL) {
        draw_sprite(game_over_menu->gameover_sprite, 250, 50);
    }

    /* if (game_over_menu->highscore_sprite != NULL) {
        draw_sprite(game_over_menu->highscore_sprite, 250, 200);
    } */

  if (game_over_menu->selected_option == 0) {
    if (game_over_menu->retryS_sprite) {
        draw_sprite(game_over_menu->retryS_sprite, 250, 300);
    }
  } else {
    if (game_over_menu->retry_sprite != NULL) {
        draw_sprite(game_over_menu->retry_sprite, 250, 300);
    }
  }

  if (game_over_menu->selected_option == 1) {
    if (game_over_menu->exittomenuS_sprite != NULL) {
        draw_sprite(game_over_menu->exittomenuS_sprite, 250, 400);
    }
  } else {
    if (game_over_menu->exittomenu_sprite != NULL) {
        draw_sprite(game_over_menu->exittomenu_sprite, 250, 400);
    }
  }

  if (game_over_menu->selected_option == 2) {
    if (game_over_menu->exitS_sprite != NULL) {
        draw_sprite(game_over_menu->exitS_sprite, 220, 500);
    }
  } else {
    if (game_over_menu->exit_sprite != NULL) {
        draw_sprite(game_over_menu->exit_sprite, 250, 500);
    }
  }
}

void destroy_game_over(game_over_menu_t *game_over_menu) {
    if (game_over_menu == NULL) {
        return;
    }

    /* if (game_over_menu->highscore_sprite != NULL) {
        destroy_sprite(game_over_menu->highscore_sprite);
        game_over_menu->highscore_sprite = NULL;
    } */

    if (game_over_menu->retry_sprite != NULL) {
        destroy_sprite(game_over_menu->retry_sprite);
        game_over_menu->retry_sprite = NULL;
    }

    if (game_over_menu->retryS_sprite != NULL) {
        destroy_sprite(game_over_menu->retryS_sprite);
        game_over_menu->retryS_sprite = NULL;
    }

    if (game_over_menu->exittomenu_sprite != NULL) {
        destroy_sprite(game_over_menu->exittomenu_sprite);
        game_over_menu->exittomenu_sprite = NULL;
    }

    if (game_over_menu->exittomenuS_sprite != NULL) {
        destroy_sprite(game_over_menu->exittomenuS_sprite);
        game_over_menu->exittomenuS_sprite = NULL;
    }

    if (game_over_menu->exit_sprite != NULL) {
        destroy_sprite(game_over_menu->exit_sprite);
        game_over_menu->exit_sprite = NULL;
    }

    if (game_over_menu->exitS_sprite != NULL) {
        destroy_sprite(game_over_menu->exitS_sprite);
        game_over_menu->exitS_sprite = NULL;
    }

    free(game_over_menu);
}
