#include "state_dead.h"

extern sys_state_e sys_state;
extern game_state_e game_state;
extern level_state_e level_state;
extern int selected;

extern int mouse_x;
extern int mouse_y;


extern sprite_t *player;

extern uint32_t *game_color_array;
extern uint32_t *background_color_array;

unsigned int countdown;
unsigned int timer_counter;
unsigned int target_lifetime;
unsigned int target_spawn_chance;

int (handle_keyboard_in_dead)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;
    
    case KEYBOARD_ARROW_DOWN:
      selected ++;
      if (selected > NUMBER_OF_MENU_OPTIONS - 1){
        selected = 0;
      }
      draw_dead_options(selected, game_color_array);
      break;

    case KEYBOARD_ARROW_UP:
      selected --;
      if (selected < 0){
        selected = NUMBER_OF_MENU_OPTIONS - 1;
      }
      draw_dead_options(selected, game_color_array);
      break;
      
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          game_state = LEVEL;
          undraw_dead_options(game_color_array);
          switch (level_state){
          case LEVEL_1:
            clear_color_array(game_color_array);
            level_1_draw(background_color_array, game_color_array);

            prepare_level_values(level_state);
            clear_targets_array();

            print_countdown(countdown, game_color_array);
            break;

          case LEVEL_2:
            clear_color_array(game_color_array);
            level_2_draw(background_color_array, game_color_array);

            prepare_level_values(level_state);
            clear_targets_array();

            print_countdown(countdown, game_color_array);
            break;

          case LEVEL_3:
            clear_color_array(game_color_array);
            level_3_draw(background_color_array, game_color_array);

            prepare_level_values(level_state);
            clear_targets_array();

            print_countdown(countdown, game_color_array);
            break;
          
          default:
            break;
          }

          break;
        case 1:
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

int (handle_mouse_in_dead)(){
  if(mouse_over_sprite(dead_restart)){
    selected=0;
    draw_dead_options(selected, game_color_array);
    
    if(left_button_released()){
      game_state = LEVEL;
      undraw_dead_options(game_color_array);
      switch (level_state){
      case LEVEL_1:
        clear_color_array(game_color_array);
        level_1_draw(background_color_array, game_color_array);
        
        prepare_level_values(level_state);
        clear_targets_array();

        print_countdown(countdown, game_color_array);
        break;

      case LEVEL_2:
        clear_color_array(game_color_array);
        level_2_draw(background_color_array, game_color_array);

        prepare_level_values(level_state);
        clear_targets_array();

        print_countdown(countdown, game_color_array);
        break;

      case LEVEL_3:
        clear_color_array(game_color_array);
        level_3_draw(background_color_array, game_color_array);

        prepare_level_values(level_state);
        clear_targets_array();
        
        print_countdown(countdown, game_color_array);
        break;
      
      default:
        break;
      }
    }
  }
  else if(mouse_over_sprite(pause_main_menu)){
    selected=1;
    draw_dead_options(selected, game_color_array);

    if(left_button_released()){
      game_state = MENU;
      selected=0;
      clear_color_array(game_color_array);
      menu_draw_header(background_color_array);
      menu_draw_options(selected, game_color_array);
    }
  }

  return 0;
}
