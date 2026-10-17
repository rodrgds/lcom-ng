#include "visual_menu.h"

sprite_t *menu_exit;
sprite_t *menu_title;
sprite_t *menu_background;
sprite_t *menu_levels;
sprite_t *menu_level_1;
sprite_t *menu_level_2;
sprite_t *menu_level_3;
sprite_t *menu_back;
sprite_t *level_1_background;
sprite_t *level_2_background;
sprite_t *level_3_background;
sprite_t *player;
sprite_t *victory;
sprite_t *coin_1;
sprite_t *coin_2;
sprite_t *coin_3;
extern int f;

uint32_t *frame_array;

extern ScoreEntry scoreboard[MAX_SCORES];



int(menu_draw_header)(uint32_t *background_color_array){

  if(game_draw_sprite(menu_background, background_color_array)) return 1;
  if(game_draw_sprite(menu_title, background_color_array)) return 1;
  
  return 0;
}

int(menu_draw_options)(int selected, uint32_t *game_color_array) {

  switch (selected) {
    case 0:
      menu_levels->i_frame = 1;
      menu_scoreboard->i_frame = 0;
      menu_exit->i_frame = 0;
      break;
    case 1:
      menu_levels->i_frame = 0;
      menu_scoreboard->i_frame = 1;
      menu_exit->i_frame = 0;
      break;
    case 2:
      menu_levels->i_frame = 0;
      menu_scoreboard->i_frame = 0;
      menu_exit->i_frame = 1;
      break;
    default:
      return 1;
  }

  if(game_draw_sprite(menu_levels, game_color_array)) return 1;
  if(game_draw_sprite(menu_scoreboard, game_color_array)) return 1;
  if(game_draw_sprite(menu_exit, game_color_array)) return 1;
  return 0;
}

int draw_pause_menu_options(int selected, uint32_t *game_color_array){
  switch (selected) {
    case 0:
      pause_resume->i_frame = 1;
      pause_main_menu->i_frame = 0;
      break;
    case 1:
      pause_resume->i_frame = 0;
      pause_main_menu->i_frame = 1;
      break;
    default:
      return 1;
  }
  game_draw_sprite(pause_resume, game_color_array);
  game_draw_sprite(pause_main_menu, game_color_array);
  return 0;
}

int draw_win_menu_options(int selected, uint32_t *game_color_array){
  game_draw_sprite(victory,game_color_array);
  game_draw_sprite(win_main_menu, game_color_array);
  return 0;
}

int (draw_dead_options)(int selected, uint32_t *game_color_array){
  switch (selected) {
    case 0:
      dead_restart->i_frame = 1;
      pause_main_menu->i_frame = 0;
      break;
    case 1:
      dead_restart->i_frame = 0;
      pause_main_menu->i_frame = 1;
      break;
    default:
      return 1;
  }
  game_draw_sprite(dead_restart, game_color_array);
  game_draw_sprite(pause_main_menu, game_color_array);
  return 0;
}

int (scoreboard_draw)(int selected, uint32_t *color_array){
  menu_back->i_frame = 1;
  game_draw_sprite(frame, color_array);
  game_draw_sprite(menu_back, color_array);
  game_draw_sprite(slide_container, color_array);
  game_draw_sprite(slide, color_array);
  return 0;
}

int(menu_undraw_header)(uint32_t *game_color_array){
  if(game_undraw_sprite(menu_background, game_color_array)) return 1;
  if(game_undraw_sprite(menu_title, game_color_array)) return 1;
  return 0;
}

int(undraw_scoreboard)(uint32_t *game_color_array){
  if(game_undraw_sprite(menu_back, game_color_array)) return 1;
  if(game_undraw_sprite(frame, game_color_array)) return 1;
  if(game_undraw_sprite(slide_container, game_color_array)) return 1;
  if(game_undraw_sprite(slide, game_color_array)) return 1;
  return 0;
}

int(menu_undraw_options)(uint32_t *game_color_array) {
  if(game_undraw_sprite(menu_scoreboard, game_color_array)) return 1;
  if(game_undraw_sprite(menu_levels, game_color_array)) return 1;
  if(game_undraw_sprite(menu_exit, game_color_array)) return 1;
  return 0;

}

int(undraw_dead_options)(uint32_t *game_color_array) {
  if(game_undraw_sprite(pause_main_menu, game_color_array)) return 1;
  if(game_undraw_sprite(dead_restart, game_color_array)) return 1;
  return 0;
}

int (pre_level_undraw_options)(uint32_t *color_array){
  game_undraw_sprite(menu_level_1, color_array);
  game_undraw_sprite(menu_level_2, color_array);
  game_undraw_sprite(menu_level_3, color_array);
  game_undraw_sprite(menu_back, color_array);

  return 0;
}

int (pause_menu_undraw_options)(uint32_t *color_array){
  game_undraw_sprite(pause_resume, color_array);
  game_undraw_sprite(pause_main_menu, color_array);
  return 0;
}



int (pre_level_draw)(uint8_t selected, uint32_t *game_color_array){

  switch (selected) {
    case 0:
      menu_level_1->i_frame = 1;
      menu_level_2->i_frame = 0;
      menu_level_3->i_frame = 0;
      menu_back->i_frame = 0;
      break;
    case 1:
      menu_level_1->i_frame = 0;
      menu_level_2->i_frame = 1;
      menu_level_3->i_frame = 0;
      menu_back->i_frame = 0;
      break;
    case 2:
      menu_level_1->i_frame = 0;
      menu_level_2->i_frame = 0;
      menu_level_3->i_frame = 1;
      menu_back->i_frame = 0;
      break;
    case 3:
      menu_level_1->i_frame = 0;
      menu_level_2->i_frame = 0;
      menu_level_3->i_frame = 0;
      menu_back->i_frame = 1;
      break;
    default:
      return 1;
  }

  if(game_draw_sprite(menu_level_1, game_color_array)) return 1;
  if(game_draw_sprite(menu_level_2, game_color_array)) return 1;
  if(game_draw_sprite(menu_level_3, game_color_array)) return 1;
  if(game_draw_sprite(menu_back, game_color_array)) return 1;
  return 0;
}

int(level_1_draw)(uint32_t *background_color_array, uint32_t *game_color_array){
      level_1_background->i_frame=0;
      f=0;
      coin_1->position=(position_t){69, 45};
      coin_2->position=(position_t){29, 50};
      coin_3->position=(position_t){20, 36};
   if(game_draw_sprite(level_1_background, background_color_array)) return 1;
   print_min_points(get_min_points(LEVEL_1), 
   background_color_array);
   print_points(0, 0, game_color_array);
   game_draw_sprite(pause_button, game_color_array);
   reset_player();
   return 0;
}

int(level_2_draw)(uint32_t *background_color_array, uint32_t *game_color_array){
  level_2_background->i_frame=0;
  f=0;
  coin_1->position=(position_t){64, 57};
  coin_2->position=(position_t){29, 46};
  coin_3->position=(position_t){12, 36};
  if(game_draw_sprite(level_2_background, background_color_array)) return 1;
   if(game_draw_sprite(coin_1,game_color_array)) return 1;
   if(game_draw_sprite(coin_2,game_color_array)) return 1;
   if(game_draw_sprite(coin_3,game_color_array)) return 1;
  print_min_points(get_min_points(LEVEL_2), background_color_array);
  print_points(0, 0, game_color_array);
  game_draw_sprite(pause_button, game_color_array);
  reset_player();
  return 0;
}

int(level_3_draw)(uint32_t *background_color_array, uint32_t *game_color_array){
  level_3_background->i_frame=0;
  f=0;
  coin_1->position=(position_t){66, 45};
  coin_2->position=(position_t){29, 50};
  coin_3->position=(position_t){45, 55};
  if(game_draw_sprite(level_3_background, background_color_array)) return 1;
  print_min_points(get_min_points(LEVEL_3), background_color_array);
  print_points(0, 0, game_color_array);
  game_draw_sprite(pause_button, game_color_array);
  reset_player();
  return 0;
}


void draw_scoreboard_scroller(uint32_t *game_color_array, int scroller) {
  xpm_map_t xpm_numbers[10] = {
    (xpm_map_t)xpm_num_zero, (xpm_map_t)xpm_num_one, (xpm_map_t)xpm_num_two, (xpm_map_t)xpm_num_three, 
    (xpm_map_t)xpm_num_four, (xpm_map_t)xpm_num_five, (xpm_map_t)xpm_num_six, (xpm_map_t)xpm_num_seven, 
    (xpm_map_t)xpm_num_eight, (xpm_map_t)xpm_num_nine
  };
  uint32_t *scroll_array = game_create_sized_color_array(56, 90);
  frame_array = game_create_sized_color_array(56, 26);
 
  for (int lvl = 1; lvl < 4; lvl++){
    load_xpm(xpm_numbers[lvl], (position_t){1, 30*(lvl-1) + 1}, scroll_array);
    for (int i = 0; i < MAX_SCORES; i++) {
      if(scoreboard[i].level == lvl){
        char score_str[10];
        snprintf(score_str, sizeof(score_str), "%d", scoreboard[i].score);
        int count_x = 0;
        for (int j = 0; score_str[j] != '\0'; j++) {
            int num = score_str[j] - '0';
            position_t pos = { 1 + count_x*5, (27*(lvl-1) + 1) + (i+1)*8};
            load_xpm(xpm_numbers[num], pos, scroll_array);
            count_x++;
        }
        char day_str[10];
        snprintf(day_str, sizeof(day_str), "%d", scoreboard[i].day);
        count_x = 0;
        for (int j = 0; day_str[j] != '\0'; j++) {
            int num = day_str[j] - '0';
            position_t pos = { 18 + count_x*5, (27*(lvl-1) + 1) + (i+1)*8};
            load_xpm(xpm_numbers[num], pos, scroll_array);
            count_x++;
        }
        char month_str[10];
        snprintf(month_str, sizeof(month_str), "%d", scoreboard[i].month);
        count_x = 0;
        for (int j = 0; month_str[j] != '\0'; j++) {
            int num = month_str[j] - '0';
            position_t pos = { 29 + count_x*5, (27*(lvl-1) + 1) + (i+1)*8};
            load_xpm(xpm_numbers[num], pos, scroll_array);
            count_x++;
        }

        char year_str[10];
        snprintf(year_str, sizeof(year_str), "%d", scoreboard[i].year);
        count_x = 0;
        for (int j = 0; year_str[j] != '\0'; j++) {
            int num = year_str[j] - '0';
            position_t pos = { 40 + count_x*5, (27*(lvl-1) + 1) + (i+1)*8};
            load_xpm(xpm_numbers[num], pos, scroll_array);
            count_x++;
        }

      }
    //scoreboard[i].level;
    }
  }
  for (int i = scroller; i < (scroller+26); i++){
    for (int j = 0; j < 56; j++){
      frame_array[(i-scroller)*56 + j] = scroll_array[i*56 + j]; 
    }   
  }
  game_draw_frame_array(frame_array, 12, 19, 56, 26);
}
