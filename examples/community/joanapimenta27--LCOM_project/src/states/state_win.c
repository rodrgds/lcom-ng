#include "state_win.h"

extern sys_state_e sys_state;
extern game_state_e game_state;
extern level_state_e level_state;
extern int selected;

extern int mouse_x;
extern int mouse_y;


extern sprite_t *player;

extern uint32_t *game_color_array;
extern uint32_t *background_color_array;

extern unsigned int countdown;
extern unsigned int timer_counter;


int (handle_keyboard_in_win)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;
    
      
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          game_state = MENU;
          selected=0;
          clear_color_array(game_color_array);
          menu_draw_header(background_color_array);
          menu_draw_options(selected, game_color_array);

            break;
          
          default:
            break;
          }

      break;
    default:
      break;
  }

  return 0;
}

int (handle_mouse_in_win)(){
  if(mouse_over_sprite(win_main_menu)){
      if(left_button_released()){
      game_state = MENU;
      selected=0;
      clear_color_array(game_color_array);
      pause_menu_undraw_options(game_color_array);
      menu_draw_header(background_color_array);
      menu_draw_options(selected, game_color_array);
    }
   
  }

  return 0;
}
