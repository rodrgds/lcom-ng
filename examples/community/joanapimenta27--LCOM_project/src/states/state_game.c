#include "state_game.h"

extern sys_state_e sys_state;
extern game_state_e game_state;
extern int selected;

extern int mouse_x;
extern int mouse_y;

extern sprite_t *player;

extern uint32_t *game_color_array;
extern uint32_t *background_color_array;
extern uint32_t *game_color_array_backup;
extern uint32_t *background_color_array_backup;

int (handle_keyboard_in_level)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_P:
      game_state = PAUSED;
      memcpy(game_color_array_backup, game_color_array, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      memcpy(background_color_array_backup, background_color_array, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      clear_color_array(game_color_array);
      game_draw_sprite(pause_background, background_color_array);
      draw_pause_menu_options(selected, game_color_array);
      break;

    case KEYBOARD_W:
      move_player((position_t) {player->position.x,player->position.y-1},1);
      break;
    
    case KEYBOARD_S:
      move_player((position_t){player->position.x,player->position.y+1},0);
      break;

    case KEYBOARD_A:
      move_player((position_t){player->position.x-1,player->position.y},3);
      break;

    case KEYBOARD_D:
    move_player((position_t){player->position.x +1,player->position.y},2);
      break;

    default:
      break;
  }

  return 0;
}

int (handle_keyboard_in_paused)(uint8_t scancode){
  switch(scancode){
    case KEYBOARD_Q:
      sys_state = GAME_EXIT;
      break;
    
    case KEYBOARD_P:
      game_state = LEVEL;
      memcpy(game_color_array, game_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      memcpy(background_color_array, background_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      game_draw_color_array(background_color_array);
      game_draw_color_array(game_color_array);
      break;

    case KEYBOARD_ARROW_DOWN:
      selected ++;
      if (selected > NUMBER_OF_MENU_OPTIONS - 1){
        selected = 0;
      }
      draw_pause_menu_options(selected, game_color_array);
      break;

    case KEYBOARD_ARROW_UP:
      selected --;
      if (selected < 0){
        selected = NUMBER_OF_MENU_OPTIONS - 1;
      }
      draw_pause_menu_options(selected, game_color_array);
      break;
      
    case KEYBOARD_ENTER_BREAK:
      switch (selected){
        case 0:
          game_state = LEVEL;
          memcpy(game_color_array, game_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
          memcpy(background_color_array, background_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
          game_draw_color_array(background_color_array);
          game_draw_color_array(game_color_array);
          break;
        case 1:
          game_state = MENU;
          selected=0;
          reset_player();
          clear_color_array(game_color_array);
          pause_menu_undraw_options(game_color_array);
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

int(handle_mouse_in_level)(){
  handle_mouse_in_game();
  for(int i = 0; i < MAX_TARGETS; i++){
    target_t target = get_target(i);
    if(target.active){
      if(mouse_over_sprite(&target.sprite)) {
        if(left_button_released()) {
          game_undraw_sprite(&target.sprite,game_color_array);
          drain_target_time(&target);

          switch (target.sprite.identifier) {
            case TARGET_LOW:
              add_points(5);
              break;
            case TARGET_MID:
              add_points(10);
              break;
            case TARGET_HIGH:
              add_points(25);
              break;
            default:
              break;
          }
        }
      }
    }
  }
  return 0;
}

int (handle_mouse_in_paused)(){
  if(mouse_over_sprite(pause_resume)){
    selected=0;
    draw_pause_menu_options(selected, game_color_array);
    
    if(left_button_released()){
        game_state = LEVEL;
        memcpy(game_color_array, game_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
        memcpy(background_color_array, background_color_array_backup, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
        game_draw_color_array(background_color_array);
        game_draw_color_array(game_color_array);
    }
  }
  else if(mouse_over_sprite(pause_main_menu)){
    selected=1;
    draw_pause_menu_options(selected, game_color_array);

    if(left_button_released()){
      game_state = MENU;
      selected=0;
      reset_player();
      clear_color_array(game_color_array);
      pause_menu_undraw_options(game_color_array);
      menu_draw_header(background_color_array);
      menu_draw_options(selected, game_color_array);
    }
  }


  return 0;
}

int (handle_mouse_in_game)(){
  if(mouse_over_sprite(pause_button)){
    if(left_button_released()){
      game_state = PAUSED;
      memcpy(game_color_array_backup, game_color_array, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      memcpy(background_color_array_backup, background_color_array, H_PSEUDO_RES * V_PSEUDO_RES * sizeof(uint32_t));
      clear_color_array(game_color_array);
      game_draw_sprite(pause_background, background_color_array);
      draw_pause_menu_options(selected, game_color_array);
    }
  }

  return 0;
}
