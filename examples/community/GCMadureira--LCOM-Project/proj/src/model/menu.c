/** 
 * @file menu.c
 * @brief Source code file for all the logic related to the menu struct
 * 
 * This file implements all the functions for the menu struct.
 */


#include "menu.h"


void (menu_select_option_up)(menu* menu){
  menu->menu_status = (menu->menu_status - 1) >= 0 ? menu->menu_status - 1 : menu->num_options - 1;
}

void (menu_select_option_down)(menu* menu){
  menu->menu_status = (menu->menu_status + 1)%menu->num_options;
}

menu* (menu_create_main)() {
  menu* main_menu = (menu*)malloc(sizeof(menu));

  main_menu->menu_status = 0;
  main_menu->num_options = 2;
  main_menu->sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 2);
  main_menu->sprites[0] = &start_selected_img;
  main_menu->sprites[1] = &quit_selected_img;
  main_menu->background_image = &start_background_img;

  return main_menu;
}

menu* (menu_create_game_over)() {
  menu* game_over_menu = (menu*)malloc(sizeof(menu));

  game_over_menu->menu_status = 0;
  game_over_menu->num_options = 2;
  game_over_menu->sprites = (xpm_image_t**)malloc(sizeof(xpm_image_t*) * 2);
  game_over_menu->sprites[0] = &play_again_selected_img;
  game_over_menu->sprites[1] = &back_to_menu_selected_img;
  game_over_menu->background_image = NULL;

  return game_over_menu;
}

int (menu_destroy)(menu* menu){
  free(menu->sprites);
  free(menu);
  return 0;
}
