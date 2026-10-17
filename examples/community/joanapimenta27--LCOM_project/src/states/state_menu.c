#include "state_menu.h"
#include "../controller/rtc/rtc.h"

extern sys_state_e sys_state;
extern game_state_e game_state;
extern level_state_e level_state;

extern int selected;
extern uint32_t *game_color_array;
extern int scroller;

extern int mouse_x;
extern int mouse_y;
int first_mouse_y;
int bck_pos_y;

extern uint32_t *frame_array;
extern uint32_t *background_color_array;

extern click_info info_mouse;

rtc_info time_info;
unsigned int countdown;
unsigned int timer_counter;
unsigned int target_lifetime;
unsigned int target_spawn_chance;

bool first = true;

int (handle_keyboard_in_menu)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;

    case KEYBOARD_ARROW_DOWN:
      selected ++;
      if (selected > 2){
        selected = 0;
      }
      menu_draw_options(selected, game_color_array);
      break;

    case KEYBOARD_ARROW_UP:
      selected --;
      if (selected < 0){
        selected = 2;
      }
      menu_draw_options(selected, game_color_array);
      break;
      
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          game_state = PRE_LEVEL;
          menu_undraw_options(game_color_array);
          pre_level_draw(selected, game_color_array);
          break;
        case 1:
          selected = 0;
          game_state = SCOREBOARD;
          menu_undraw_options(game_color_array);
          scoreboard_draw(selected, game_color_array);
          break;
        case 2:
          sys_state = GAME_EXIT;
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

int (handle_keyboard_in_pre_level)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;
    
    case KEYBOARD_ARROW_DOWN:
      selected ++;
      if(selected>3){
        selected=0;
      }

      pre_level_draw(selected, game_color_array);

      break;
    case KEYBOARD_ARROW_UP:
      selected--;
      if(selected<0){
        selected=3;
      }

      pre_level_draw(selected, game_color_array);

      break;
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          game_state = LEVEL;
          level_state = LEVEL_1;
          pre_level_undraw_options(game_color_array);
          level_1_draw(background_color_array, game_color_array);
          
          prepare_level_values(level_state);
          clear_targets_array();

          print_countdown(countdown, game_color_array);
        
          break;
        case 1:
          selected=0;
          game_state = LEVEL;
          level_state = LEVEL_2;
          pre_level_undraw_options(game_color_array);
          level_2_draw(background_color_array, game_color_array);

          prepare_level_values(level_state);
          clear_targets_array();

          print_countdown(countdown, game_color_array);
          break;
        case 2:
          selected=0;
          game_state = LEVEL;
          level_state = LEVEL_3;
          pre_level_undraw_options(game_color_array);
          level_3_draw(background_color_array, game_color_array);

          prepare_level_values(level_state);
          clear_targets_array();

          print_countdown(countdown, game_color_array);
          break;
        case 3:
          selected=0;
          pre_level_undraw_options(game_color_array);
          menu_draw_options(selected, game_color_array);
          game_state=MENU;
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

int (handle_keyboard_in_scoreboard)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;

    case KEYBOARD_ARROW_DOWN:
      scroller ++;
      break;

    case KEYBOARD_ARROW_UP:
      scroller --;
      if (scroller < 0){
        scroller = 0;
      }
      break;
      
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          selected=0;
          undraw_scoreboard(game_color_array);
          menu_draw_header(background_color_array);
          menu_draw_options(selected, game_color_array);
          game_state=MENU;
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

bool (mouse_over_sprite)(sprite_t *s){
  int x=(s->position.x)*10;
  int y=(s->position.y)*10;
  int width=(s->frames[s->i_frame].width)*10;
  int height=(s->frames[s->i_frame].height)*10;

  if((mouse_x + 25 >= x) && (mouse_y + 15 >= y) && (mouse_x <= x+ width) && (mouse_y + 10 <= y + height)){
    return true;
  }

  return false;
 
}

int (handle_mouse_in_menu)(){
  if(mouse_over_sprite(menu_levels)){
    selected=0;
    menu_draw_options(selected, game_color_array);
    
    if(left_button_released()){
        game_state = PRE_LEVEL;
        menu_undraw_options(game_color_array);
        pre_level_draw(selected, game_color_array);
    }
  }
  else if(mouse_over_sprite(menu_scoreboard)){
    selected=1;
    menu_draw_options(selected, game_color_array);

    if(left_button_released()){
      selected = 0;
      game_state = SCOREBOARD;
      menu_undraw_options(game_color_array);
      scoreboard_draw(selected, game_color_array);
    }
  }
  else if(mouse_over_sprite(menu_exit)){
    selected=2;
    menu_draw_options(selected, game_color_array);

    if(left_button_released()){
     sys_state = GAME_EXIT;
    }
  }


  return 0;
}

int (handle_mouse_in_pre_level)(){
  if(mouse_over_sprite(menu_level_1)){
    selected=0;
    pre_level_draw(selected, game_color_array);

    if(left_button_released()){
      game_state = LEVEL;
      level_state = LEVEL_1;
      pre_level_undraw_options(game_color_array);
      level_1_draw(background_color_array, game_color_array);

      prepare_level_values(level_state);
      clear_targets_array();

      print_countdown(countdown, game_color_array);
    }
  }
  else if(mouse_over_sprite(menu_level_2)){
    selected=1;
    pre_level_draw(selected, game_color_array);

    if(left_button_released()){
      selected = 0;
      game_state = LEVEL;
      level_state = LEVEL_2;
      pre_level_undraw_options(game_color_array);
      level_2_draw(background_color_array, game_color_array);

      prepare_level_values(level_state);
      clear_targets_array();

      print_countdown(countdown, game_color_array);
    }
  }
  else if(mouse_over_sprite(menu_level_3)){
    selected=2;
    pre_level_draw(selected, game_color_array);

    if(left_button_released()){
      selected = 0;
      game_state = LEVEL;
      level_state = LEVEL_3;
      pre_level_undraw_options(game_color_array);
      level_3_draw(background_color_array, game_color_array);
      
      prepare_level_values(level_state);
      clear_targets_array();
      
      print_countdown(countdown, game_color_array);
    }
  }
  else if(mouse_over_sprite(menu_back)){
    selected=3;
    pre_level_draw(selected, game_color_array);

    if(left_button_released()){
      selected = 0;
      pre_level_undraw_options(game_color_array);
      menu_draw_options(selected, game_color_array);
      game_state=MENU;
    }
  }

  return 0;
}

int (handle_mouse_in_scoreboard)(){
  if(mouse_over_sprite(menu_back)){    
    if(left_button_released()){
          selected=0;
          undraw_scoreboard(game_color_array);
          menu_draw_header(background_color_array);
          menu_draw_options(selected, game_color_array);
          game_state=MENU;
    }
  }
  else if(left_button_released()){
    first = true;
  }
  else if (mouse_over_sprite(slide)){
    if(info_mouse.left){
      if (first) {
        bck_pos_y = slide->position.y;
        first_mouse_y = mouse_y;
        first = false;
      }
      game_undraw_sprite(slide, game_color_array);
      game_draw_sprite(slide_container, game_color_array);
      slide->position.y += (mouse_y - first_mouse_y)/10;
      scroller += (mouse_y - first_mouse_y)/6;
      game_draw_sprite(slide, game_color_array);
      if (bck_pos_y != slide->position.y || (mouse_y - first_mouse_y)/10>1){
        bck_pos_y = slide->position.y;
        first_mouse_y = mouse_y;
      }
    }
  }
 
  return 0;
}
