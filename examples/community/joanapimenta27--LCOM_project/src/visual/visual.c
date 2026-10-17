#include "visual.h"

// A pseudo pixel, in this context, is a 10x10 block of pixels
// A pseudo-pixel positioned in coordinates (0,0) is going to actually fill all pixels from (0,0) to (9,9) in the actual screen. Therefore making 10x10 pixel blocks


not_pseudo_sprite *mouse_gun;

extern int mouse_x;
extern int mouse_y;
int last_pos_x; 
int last_pos_y;

extern sprite_t *menu_exit;
extern sprite_t *menu_title;
extern sprite_t *menu_background;
extern sprite_t *menu_levels;
extern sprite_t *menu_level_1;
extern sprite_t *menu_level_2;
extern sprite_t *menu_level_3;
extern sprite_t *menu_back;
extern sprite_t *player;
extern sprite_t *level_1_background;
extern sprite_t *level_2_background;
extern sprite_t *level_3_background;
extern sprite_t *victory;
extern sprite_t *coin_1;
extern sprite_t *coin_2;
extern sprite_t *coin_3;

extern sprite_t *target_low;
extern sprite_t *target_mid;
extern sprite_t *target_high;

extern uint32_t *background_color_array;

void get_xpm_dimensions(xpm_map_t xpm, int *width, int *height) {
    if (xpm == NULL || width == NULL || height == NULL) {
        return;
    }
    sscanf(xpm[0], "%d %d", width, height);
}

int(prepare_sprites)() {

  menu_background = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_menu_background}, (position_t) {0, 0}, 1,TEXT);
  if (menu_background == NULL) return 1;

  level_1_background = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_level_1_start,(xpm_map_t) xpm_level_1_midlle,(xpm_map_t) xpm_level_1_end}, (position_t) {0, 0}, 3,TEXT);
  if (level_1_background == NULL) return 1;

  level_2_background = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_level_2_start,(xpm_map_t) xpm_level_2_midlle,(xpm_map_t) xpm_level_2_end}, (position_t) {0, 0}, 3,TEXT);
  if (level_2_background == NULL) return 1;

  level_3_background = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_level_3_start,(xpm_map_t) xpm_level_3_midlle,(xpm_map_t) xpm_level_3_end}, (position_t) {0, 0}, 3,TEXT);
  if (level_3_background == NULL) return 1;

  menu_title = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_name_game}, (position_t) {9, 10}, 1,TEXT);
  if (menu_title == NULL) return 1;

  menu_levels = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_levels, (xpm_map_t)selected_xpm_levels}, (position_t) {28, 30}, 2, TEXT);
  if (menu_levels == NULL) return 1;

  menu_scoreboard = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_scoreboard, (xpm_map_t)selected_xpm_scoreboard}, (position_t) {16, 40}, 2, TEXT);

  frame = create_sprite((xpm_map_t[]) {(xpm_map_t) frame_xpm}, (position_t) {10, 17}, 1, TEXT);
  slide_container = create_sprite((xpm_map_t[]) {(xpm_map_t) slide_container_xpm}, (position_t) {70, 17}, 1, TEXT);
  slide = create_sprite((xpm_map_t[]) {(xpm_map_t) slide_xpm}, (position_t) {70, 17}, 1, TEXT);

  menu_exit = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_exit, (xpm_map_t)selected_xpm_exit}, (position_t) {32, 50}, 2, TEXT);
  if (menu_exit == NULL) return 1;

  menu_level_1 = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_level_1, (xpm_map_t)selected_xpm_level_1}, (position_t) {28, 27}, 2, TEXT);
  if (menu_level_1 == NULL) return 1;

  menu_level_2 = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_level_2, (xpm_map_t)selected_xpm_level_2}, (position_t) {28, 35}, 2, TEXT);
  if (menu_level_2 == NULL) return 1;

  menu_level_3 = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_level_3, (xpm_map_t)selected_xpm_level_3}, (position_t) {28, 43}, 2, TEXT);
  if (menu_level_3 == NULL) return 1;

  menu_back = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_back, (xpm_map_t)selected_xpm_back}, (position_t) {31, 51}, 2, TEXT);
  if (menu_back == NULL) return 1;

  player=create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_player_idle, (xpm_map_t)xpm_player_idle_back ,(xpm_map_t) xpm_player_walk_frame_one,(xpm_map_t)xpm_player_walk_frame_two,(xpm_map_t)xpm_player_die_frame_one,(xpm_map_t)xpm_player_die_frame_two}, (position_t) {2, 34}, 6, TEXT);
  if (player == NULL) return 1;

  pause_background = create_sprite((xpm_map_t[]) {(xpm_map_t) xpm_pause_background}, (position_t) {0, 0}, 1,TEXT);
  if (pause_background == NULL) return 1;

  pause_resume = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_resume, (xpm_map_t)selected_xpm_resume}, (position_t) {24, 20}, 2, TEXT);
  if (pause_resume == NULL) return 1;

  pause_main_menu = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_main_menu, (xpm_map_t)selected_xpm_main_menu}, (position_t) {17, 40}, 2, TEXT);
  if (pause_main_menu == NULL) return 1;

  win_main_menu = create_sprite((xpm_map_t[]) { (xpm_map_t)selected_xpm_main_menu}, (position_t) {17, 40}, 1, TEXT);

  victory = create_sprite((xpm_map_t[]) { (xpm_map_t)xpm_victory}, (position_t) {20, 20}, 1, TEXT);

  dead_restart = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_restart, (xpm_map_t)selected_xpm_restart}, (position_t) {22, 30}, 2, TEXT);
  if (dead_restart == NULL) return 1;

  coin_1= create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_coin}, (position_t) {69, 45}, 1, TEXT);
  coin_2= create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_coin}, (position_t) {29, 50}, 1, TEXT);
  coin_3= create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_coin}, (position_t) {20, 36}, 1, TEXT);


  pause_button = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_pause_button, (xpm_map_t)selected_xpm_restart}, (position_t) {69, 1}, 2, TEXT);

  mouse_gun = create_not_pseudo_sprite((xpm_map_t) xpm_mouse_gun);
  if (mouse_gun == NULL) return 1;

  target_low = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_target_low, (xpm_map_t)xpm_target_low_pop}, (position_t) {0, 0}, 2,TARGET_LOW);
  if (target_low == NULL) return 1;

  target_mid = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_target_mid, (xpm_map_t)xpm_target_mid_pop}, (position_t) {0, 0}, 2,TARGET_MID);
  if (target_mid == NULL) return 1;

  target_high = create_sprite((xpm_map_t[]) {(xpm_map_t)xpm_target_high, (xpm_map_t)xpm_target_high_pop}, (position_t) {0, 0}, 2,TARGET_HIGH);
  if (target_high == NULL) return 1;

  return 0;
}

void(delete_sprites)() {
  delete_sprite(menu_background);
  delete_sprite(menu_title);
  delete_sprite(menu_levels);
  delete_sprite(menu_exit);
  delete_sprite(menu_level_1);
  delete_sprite(menu_level_2);
  delete_sprite(menu_level_3);
  delete_sprite(menu_back);
  delete_sprite(level_1_background);
  delete_sprite(level_2_background);
  delete_sprite(level_3_background);
  delete_sprite(player);
  delete_not_pseudo_sprite(mouse_gun);
  delete_sprite(target_low);
  delete_sprite(target_mid);
  delete_sprite(target_high);
}

int(game_draw_pseudo_pixel)(position_t pos, uint32_t color) {
    // PSEUDO_PIXEL_SIZE = 10
    if(vg_draw_rectangle(PSEUDO_PIXEL_SIZE * pos.x, PSEUDO_PIXEL_SIZE * pos.y, PSEUDO_PIXEL_SIZE, PSEUDO_PIXEL_SIZE, color)!=0)return 1;
    return 0;
}

int (game_draw_xpm)(xpm_map_t xpm, position_t pos, uint32_t *color_array){
  xpm_image_t img;
  memset(&img,0,sizeof(img));
  uint8_t* xpm_colors = xpm_load(xpm, XPM_8_8_8, &img);
  if(xpm_colors==NULL)return 1;
  
  for(int i=0; i<img.height; i++){
    for(int j=0; j<img.width; j++){
      int index = i*img.width + j;
      uint8_t blue = xpm_colors[index * 3];
      uint8_t green = xpm_colors[index * 3 + 1];
      uint8_t red = xpm_colors[index * 3 + 2];
      uint32_t color = 0 | (red << 16) | (green << 8) | blue;

      if(color!=CHROMA_KEY_GREEN_888){
        for (int a = 0; a < 10; a++){
          for (int b = 0; b < 10; b++){
            if (vg_draw_pixel(pos.x*10 + b + j*10, pos.y*10 + a + i*10, color)) return 1;
          }
        }
        color_array[(pos.y + i)*H_PSEUDO_RES+(pos.x + j)] = color;
      }
    }
  }

  return 0;
}

int (load_xpm)(xpm_map_t xpm, position_t pos, uint32_t *color_array){
  xpm_image_t img;
  memset(&img,0,sizeof(img));
  uint8_t* xpm_colors = xpm_load(xpm, XPM_8_8_8, &img);
  if(xpm_colors==NULL)return 1;
  
  for(int i=0; i<img.height; i++){
    for(int j=0; j<img.width; j++){
      int index = i*img.width + j;
      uint8_t blue = xpm_colors[index * 3];
      uint8_t green = xpm_colors[index * 3 + 1];
      uint8_t red = xpm_colors[index * 3 + 2];
      uint32_t color = 0 | (red << 16) | (green << 8) | blue;

      if(color!=CHROMA_KEY_GREEN_888){
        color_array[(pos.y + i)*56+(pos.x + j)] = color;
      }
    }
  }

  return 0;
}

int (game_undraw_xpm)(xpm_map_t xpm, position_t pos, uint32_t *color_array) {
  int width, height;
  get_xpm_dimensions(xpm, &width, &height);
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      for (int a = 0; a < 10; a++){
        for (int b = 0; b < 10; b++){
          if (vg_draw_pixel(pos.x*10 + b + j*10, pos.y*10 + a + i*10, background_color_array[(pos.y + i)*H_PSEUDO_RES + pos.x + j])) return 1;
        }
      }
      color_array[(pos.y + i)*H_PSEUDO_RES + (pos.x + j)] = CHROMA_KEY_GREEN_888;
    }
  }
  
  return 0;
}

int (game_draw_text)(char* text, position_t pos, uint32_t *color_array){
  int offset_inc, offset = 0;
  while (*text != '\0'){
    xpm_map_t xpm_char = get_xpm_from_char(*text);
    if (xpm_char == NULL) return 1;

    if(game_draw_xpm(xpm_char, (position_t) {pos.x + offset, pos.y}, color_array))return 1;

    offset_inc = get_xpm_width_from_char(*text);
    if (offset_inc == -1) return 1;

    offset+=offset_inc + 1;
    text++;
  }
  return 0;
}

int (game_draw_sprite)(sprite_t *spr, uint32_t *color_array){
  for (int i = 0; i < spr->frames[spr->i_frame].height; i++){
    for (int j = 0; j < spr->frames[spr->i_frame].width; j++){
      uint32_t color = spr->frames[spr->i_frame].color_array[i * spr->frames[spr->i_frame].width + j];
      if (color != CHROMA_KEY_GREEN_888){
        if(!(spr->position.x+j>=80)){
          for (int a = 0; a < 10; a++){
            for (int b = 0; b < 10; b++){
              if (vg_draw_pixel(spr->position.x*10 + b + j*10, spr->position.y*10 + a + i*10, color)) return 1;
            }
          }
          color_array[(spr->position.y + i)*H_PSEUDO_RES+(spr->position.x + j)] = color;
        }
      }
    }
  }

  return 0;
}

int (game_draw_not_pseudo_sprite)(not_pseudo_sprite *sprite, int pos_x, int pos_y){
  uint16_t height = sprite->height, width = sprite->width;
  last_pos_x = pos_x;
  last_pos_y = pos_y;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (sprite->colors[w + h*width]!= CHROMA_KEY_GREEN_888 )
        if (vg_draw_pixel(pos_x + w, pos_y + h, sprite->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

int(game_undraw_sprite)(sprite_t *spr, uint32_t* color_array){
  for (int i = 0; i < spr->frames[spr->i_frame].height; i++){
    for (int j = 0; j < spr->frames[spr->i_frame].width; j++){
      if(!(spr->position.x+j>=80)){
        for (int a = 0; a < 10; a++){
          for (int b = 0; b < 10; b++){
            if (vg_draw_pixel(spr->position.x*10 + b + j*10, spr->position.y*10 + a + i*10, background_color_array[(spr->position.y + i)*H_PSEUDO_RES + spr->position.x + j])) return 1;
          }
        }
      }
      color_array[(spr->position.y + i)*H_PSEUDO_RES + (spr->position.x + j)] = CHROMA_KEY_GREEN_888;
    }
  }
  
  return 0;
}

int game_undraw_not_pseudo_sprite(not_pseudo_sprite *sprite, int last_pos_x, int last_pos_y, uint32_t* game_color_array) {
    uint16_t height = sprite->height, width = sprite->width;
    
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            int game_x = last_pos_x + w;
            int game_y = last_pos_y + h;
            int index = (game_y/10 * H_PSEUDO_RES + game_x/10);

            if(game_color_array[index]!=CHROMA_KEY_GREEN_888){
              if (vg_draw_pixel(game_x, game_y, game_color_array[index])) {
                  printf("Error drawing pixel at (%d, %d)\n", game_x, game_y);
                  return 1;
              }
            }
            else {
              if (vg_draw_pixel(game_x, game_y, background_color_array[index])) {
                  printf("Error drawing pixel at (%d, %d)\n", game_x, game_y);
                  return 1;
              }
            }
        }
    }
    return 0;
}

int(game_undraw_pseudo_pixel)(position_t pos, uint32_t* game_color_array){
  int position = pos.y * H_PSEUDO_RES + pos.x;
  if(game_draw_pseudo_pixel(pos, game_color_array[position])!=0)return 1;
  return 0;
}

uint32_t* (game_create_color_array)(){
  uint32_t* game_color_array = (uint32_t*) malloc(H_PSEUDO_RES*V_PSEUDO_RES*sizeof(uint32_t));
  if(game_color_array==NULL)return NULL;
  
  // Initialize the array with transparent color
  for(int i=0; i<H_PSEUDO_RES*V_PSEUDO_RES; i++){
    game_color_array[i] = 0x00B140;
  }
  
  return game_color_array;
}

uint32_t* game_create_sized_color_array(int x, int y){
  uint32_t* color_array = (uint32_t*) malloc(x*y*sizeof(uint32_t));
  if(color_array==NULL)return NULL;
  
  // Initialize the array with transparent color
  for(int i=0; i<x*y; i++){
    color_array[i] = 0x534F89;
  }
  
  return color_array;
}

int(game_draw_color_array)(uint32_t* game_color_array){
  // H_PSEUDO_RES = 80 and V_PSEUDO_RES = 60
  for(int i=0; i<V_PSEUDO_RES; i++){
    for(int j=0; j<H_PSEUDO_RES; j++){
      if(game_color_array[i*H_PSEUDO_RES + j]!=CHROMA_KEY_GREEN_888){
        for (int a = 0; a < 10; a++){
          for (int b = 0; b < 10; b++){
            if (vg_draw_pixel(b + j*10, a + i*10, game_color_array[i*H_PSEUDO_RES + j])) return 1;
          }
        }
      }
    }
  }
  return 0;
}

int(game_draw_frame_array)(uint32_t* color_array, int x, int y, int size_x, int size_y){
  for(int i=0; i<size_y; i++){
    for(int j=0; j<size_x; j++){
      for (int a = 0; a < 10; a++){
        for (int b = 0; b < 10; b++){
          if (vg_draw_pixel(x*10 + j*10 + b, y*10 + i*10 + a, color_array[i*56 + j])) return 1;
        }
      }
    }
  }
  return 0;
}

void (game_delete_color_array)(uint32_t* game_color_array){
  free(game_color_array);
  game_color_array = NULL;
  return;
}


void (game_clear_color_array)(uint32_t* game_color_array){
  for(int y=0; y<V_PSEUDO_RES; y++){
    for(int x=0; x<H_PSEUDO_RES; x++){
      game_color_array[y*H_PSEUDO_RES + x]=0;
    }
  }
  return;
}

int(game_add_xpm)(xpm_map_t xpm, position_t pos, uint32_t* game_color_array){
    xpm_image_t img;
    memset(&img,0,sizeof(img));
    uint8_t* xpm_colors = xpm_load(xpm, XPM_8_8_8, &img);
    if(xpm_colors==NULL)return 1;
    
    for(int i=0; i<img.height; i++){
        for(int j=0; j<img.width; j++){
            int index = i*img.width + j;
            uint8_t blue = xpm_colors[index * 3];
            uint8_t green = xpm_colors[index * 3 + 1];
            uint8_t red = xpm_colors[index * 3 + 2];
            uint32_t color = 0 | (red << 16) | (green << 8) | blue;

            if(color!=CHROMA_KEY_GREEN_888){
                for (int a = 0; a < 10; a++){
                  for (int b = 0; b < 10; b++){
                    if (vg_draw_pixel(pos.x*10 + b + j*10, pos.y*10 + a + i*10, color)) return 1;
                  }
                }
                game_color_array[(pos.y+i)*H_PSEUDO_RES + pos.x+j] = color;
            }
        }
    }

    return 0;
}

int(game_add_text)(char* text, position_t pos, uint32_t* game_color_array){
    int offset_inc, offset = 0;
    while (*text != '\0'){
        xpm_map_t xpm_char = get_xpm_from_char(*text);
        if (xpm_char == NULL) return 1;

        if(game_add_xpm(xpm_char, (position_t) {pos.x + offset, pos.y}, game_color_array))return 1;

        offset_inc = get_xpm_width_from_char(*text);
        if (offset_inc == -1) return 1;

        offset+=offset_inc + 1;
        text++;
    }
    return 0;
}

int(game_add_sprite)(sprite_t *spr, uint32_t* game_color_array){
  position_t pos = spr->position;
  uint8_t i = spr->i_frame;
  pixmap_t frame = spr->frames[i];
  uint16_t width = frame.width;
  uint16_t height = frame.height;

  for(int x=0; x < width; x++){
    for(int y=0; y< height; y++){
      // CHROMA_KEY_GREEN_888 is what XPM_8_8_8 uses to represent the transparent color, the hex color of the value is 0x00B140
      if(frame.color_array[y*width + x]!=CHROMA_KEY_GREEN_888){
        game_color_array[(pos.y+y)*H_PSEUDO_RES + pos.x+x] = frame.color_array[y*width + x];
        //printf("Drawing pixel at (%d, %d) with color 0x%06X\n", pos.x+x, pos.y+y, frame.color_array[y*width + x]);
      }
    }
  }
  
  return 0;
}

void (print_mouse)(uint32_t* game_color_array) {
  game_undraw_not_pseudo_sprite(mouse_gun, last_pos_x, last_pos_y, game_color_array);
  game_draw_not_pseudo_sprite(mouse_gun, mouse_x, mouse_y);
}

void (clear_color_array)(uint32_t *color_array){
  for(int i=0; i<H_PSEUDO_RES*V_PSEUDO_RES; i++){
    color_array[i] = 0x00B140;
  }
}

void print_countdown(unsigned int countdown, uint32_t *color_array){
  position_t pos;

  xpm_map_t xpm_numbers[10] = {
        (xpm_map_t)xpm_num_zero, (xpm_map_t)xpm_num_one, (xpm_map_t)xpm_num_two, (xpm_map_t)xpm_num_three, 
        (xpm_map_t)xpm_num_four, (xpm_map_t)xpm_num_five, (xpm_map_t)xpm_num_six, (xpm_map_t)xpm_num_seven, 
        (xpm_map_t)xpm_num_eight, (xpm_map_t)xpm_num_nine
  };

  unsigned int previous_countdown = (countdown == 0) ? 0 : countdown + 1;

  unsigned int prev_minutes = previous_countdown / 60;
  unsigned int prev_seconds = previous_countdown % 60;

  unsigned int minutes = countdown / 60;
  unsigned int seconds = countdown % 60;

  char time_str[10];
  char prev_time_str[10];
  snprintf(prev_time_str, sizeof(prev_time_str), "%u:%02u", prev_minutes, prev_seconds);
  snprintf(time_str, sizeof(time_str), "%u:%02u", minutes, seconds);
  int length = strlen(time_str);
  int prev_length = strlen(prev_time_str);

  int x_start = 44;
  int y_start = 1;

  bool colon_printed = false;

  for (int i = 0; i < prev_length; i++) {
    if (prev_time_str[i] != time_str[i]) {
        if (prev_time_str[i] == ':') {
          colon_printed = true;
        } 
        else {
          int digit = prev_time_str[i] - '0';
          xpm_map_t xpm_digit = xpm_numbers[digit];
          if (colon_printed){
            pos = (position_t) {x_start + i*5 - 2, y_start};
          }
          else {
            pos = (position_t) {x_start + i*4, y_start};
          }
          if (game_undraw_xpm(xpm_digit, pos, color_array) != 0) {
              printf("Error undrawing digit %d\n", digit);
          }
        }
      }
    }

  colon_printed = false;

  for (int i = 0; i < length; i++) {
    char ch = time_str[i];
    if (ch == ':') {
      if (game_draw_xpm((xpm_map_t) xpm_colon, (position_t) {x_start + i * 4 + 1, y_start}, color_array) != 0) {
        printf("Error drawing colon\n");
      }
      else {
        colon_printed = true;
      }
    }
    else {
      int digit = ch - '0';
      xpm_map_t xpm_digit = xpm_numbers[digit];
      if (colon_printed){
        pos = (position_t) {x_start + i*5 - 2, y_start};
      }
      else {
        pos = (position_t) {x_start + i*4, y_start};
      }
      if (game_undraw_xpm(xpm_digit, pos, color_array) != 0) {
          printf("Error undrawing digit %d\n", digit);
      }
      if (game_draw_xpm(xpm_digit, pos, color_array) != 0) {
          printf("Error drawing digit %d\n", digit);
      }
    }
  }
}

void print_min_points(unsigned int min_points, uint32_t *color_array){
  position_t pos;

  xpm_map_t xpm_numbers[10] = {
        (xpm_map_t)xpm_num_zero, (xpm_map_t)xpm_num_one, (xpm_map_t)xpm_num_two, (xpm_map_t)xpm_num_three, 
        (xpm_map_t)xpm_num_four, (xpm_map_t)xpm_num_five, (xpm_map_t)xpm_num_six, (xpm_map_t)xpm_num_seven, 
        (xpm_map_t)xpm_num_eight, (xpm_map_t)xpm_num_nine
  };

  char min_points_str[4];
  snprintf(min_points_str, sizeof(min_points_str), "%03u", min_points);

  int min_points_length = strlen(min_points_str);

  if(game_draw_xpm((xpm_map_t) xpm_slash, (position_t) {20, 1}, color_array) != 0){
    printf("Error drawing slash\n");
  }

  for (int i = 0; i < min_points_length; i++) {
    int digit = min_points_str[i] - '0';
    pos = (position_t) {24 + i*5, 1};
    if (game_draw_xpm(xpm_numbers[digit], pos, color_array) != 0) {
      printf("Error drawing digit %d\n", digit);
    }
  }
}

void print_points(unsigned int points, unsigned int prev_points, uint32_t *color_array){
  position_t pos;

  xpm_map_t xpm_numbers[10] = {
        (xpm_map_t)xpm_num_zero, (xpm_map_t)xpm_num_one, (xpm_map_t)xpm_num_two, (xpm_map_t)xpm_num_three, 
        (xpm_map_t)xpm_num_four, (xpm_map_t)xpm_num_five, (xpm_map_t)xpm_num_six, (xpm_map_t)xpm_num_seven, 
        (xpm_map_t)xpm_num_eight, (xpm_map_t)xpm_num_nine
  };

  char prev_points_str[4];
  char points_str[4];

  snprintf(prev_points_str, sizeof(prev_points_str), "%03u", prev_points);
  snprintf(points_str, sizeof(points_str), "%03u", points);

  int points_length = strlen(points_str);

  int digit = 0;
  
  for (int i = 0; i < points_length; i++) {
    if (prev_points_str[i] != points_str[i]) {
      digit = prev_points_str[i] - '0';
      pos = (position_t) {5 + i*5, 1};
      if (game_undraw_xpm(xpm_numbers[digit], pos, color_array) != 0) {
          printf("Error undrawing digit %d\n", digit);
      }
    }
    digit = points_str[i] - '0';
    pos = (position_t) {5 + i*5, 1};
    if (game_draw_xpm(xpm_numbers[digit], pos, color_array) != 0) {
      printf("Error drawing digit %d\n", digit);
    }
  }
}

void increase_redness(uint32_t *color_array, uint8_t increment){
  for (int i = 0; i < H_PSEUDO_RES*V_PSEUDO_RES; i++) {
    uint32_t color = color_array[i];

    if (color != CHROMA_KEY_GREEN_888) {
      uint8_t red = (color >> 16) & 0xFF;
      uint8_t green = (color >> 8) & 0xFF;
      uint8_t blue = color & 0xFF;

      red = (red + increment > 255) ? 255 : red + increment;

      color_array[i] = (red << 16) | (green << 8) | blue;
    }
  }
}

void draw_dead_text(const char *death_str, uint32_t *color_array){
  int string_width = strlen(death_str);

  int x_position = 40 - (string_width / 2)*4;
  printf("%d", x_position);
  position_t pos = {x_position, 10};

  if (game_add_text((char *)death_str, pos, color_array) != 0) {
    printf("Error drawing death text\n");
  }
}
