#include "drawScreens.h"
#include <lcom/lcf.h>
#include <string.h>

typedef struct {
  uint8_t *xpm_map;
  xpm_image_t img;
} characters_xpm;

characters_xpm xpm_array_digits[11];
characters_xpm xpm_array_characters[26];

int (load_char_xpms)() {
  xpm_map_t xpm_array[] = {zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm, five_xpm, six_xpm, seven_xpm, eight_xpm, nine_xpm, dot_xpm};

  for (int i = 0; i < 11; i++) {
    xpm_array_digits[i].xpm_map = xpm_load(xpm_array[i], XPM_8_8_8_8, &xpm_array_digits[i].img);
    if(xpm_array_digits[i].img.type == INVALID_XPM) return 1;
  }

  xpm_map_t xpm_array_char[] = {A_xpm, B_xpm, C_xpm, D_xpm, E_xpm, F_xpm, G_xpm, H_xpm, I_xpm, J_xpm, K_xpm, L_xpm, M_xpm, N_xpm, O_xpm, P_xpm, Q_xpm, R_xpm, S_xpm, T_xpm, U_xpm, V_xpm, W_xpm, X_xpm, Y_xpm, Z_xpm};

  for (int i = 0; i < 26; i++) {
    xpm_array_characters[i].xpm_map = xpm_load(xpm_array_char[i], XPM_8_8_8_8, &xpm_array_characters[i].img);
    if(xpm_array_characters[i].img.type == INVALID_XPM) return 1;
  }

  return 0;
}

int (write_on_center)(int size_str, int* x_middle) {
  int str_length = size_str*25 + (size_str-1)*5;
  *x_middle = (1152 - str_length)/2;

  return 0;
}


int (draw_timer_display)(int elapsed_time) {
  int seconds_remaining = TIMER_INITIAL - elapsed_time;

  if(seconds_remaining < 0) return 1;

  if(vg_draw_rectangle(680, 50, 150, 70, ACCENT_COLOR)) return 1;

  int minutes = seconds_remaining / 60;
  int seconds = seconds_remaining % 60;

  int xpm_index[4] = {minutes, 10, seconds/10, seconds%10};

  for(int i = 0; i<4; i++){
    int index = xpm_index[i];

    if(vg_draw_xpm(xpm_array_digits[index].img, xpm_array_digits[index].xpm_map, 700 + i * (xpm_array_digits[index].img.width + 5), 65)) return 1;
  }

  return 0;
}

int (draw_cursor)(int x, int y, uint8_t* mouse_map, xpm_image_t img){
  if(vg_draw_xpm(img, mouse_map, x, y)) return 1;
  return 0;
}

int (draw_string)(char string[], int size, int x, int y){
  for(int i = 0; i < size; i++){
    if(string[i] == ' ') continue;

    int char_ascii = (int) string[i];

    if(vg_draw_xpm(xpm_array_characters[char_ascii-65].img, xpm_array_characters[char_ascii-65].xpm_map, x + i * (xpm_array_characters[char_ascii-65].img.width + 5), y)) return 1;
  }
  return 0;
}

int (draw_main_menu)(uint8_t* mouse_map, xpm_image_t img, int mouse_x, int mouse_y, int* c_mouse, int* new_screen){
  int x;

  if(*c_mouse > 0 || *new_screen > 0){
    clear_buffer();
    write_on_center(5, &x);
    if(draw_string("SKESS", 5, x, 300)) return 1;
    if(vg_draw_rectangle(420, 500, 300, 100, ACCENT_COLOR)) return 1;
    if(draw_string("START", 5, 504, 525)) return 1;
    if(draw_cursor(mouse_x, mouse_y, mouse_map, img)) return 1;

    (*new_screen)--;
    (*c_mouse)--;
  }
  return 0;
}

int (draw_lose_menu)(int* new_screen){
  int x;

  if(*new_screen > 0){
    clear_buffer();
    write_on_center(8, &x);
    if(draw_string("YOU LOST", 8, x, 410)) return 1;
    (*new_screen)--;
  }
  return 0;
}

int (draw_win_menu)(int* new_screen){
  int x;

  if(*new_screen > 0){
    clear_buffer();
    write_on_center(7, &x);
    if(draw_string("YOU WON", 7, x, 410)) return 1;
    (*new_screen)--;
  }
  return 0;
}

int (draw_guessing_screen)(uint32_t drawingScreen[], uint8_t* mouse_map, xpm_image_t img, int x, int y, int elapsed_time, char string[], int size_str, int* c_mouse, int* c_text, int* c_timer, int* new_submit, int* new_screen, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int history_count){
  if(*new_screen > 0){
    clear_buffer();
  }

  if(*c_mouse > 0 || *new_screen > 0){
    if(vg_draw_buffer(50, 50, MAX_X_GUESSING, MAX_Y_GUESSING + 84, &drawingScreen[0])) return 1;
    if(draw_clear_button(drawingScreen)) return 1;
    if(draw_cursor(x, y, mouse_map, img)) return 1;
    (*c_mouse) --;
  }

  if(*c_timer > 0 || *new_screen > 0){
    if(draw_timer_display(elapsed_time / 60)) return 1;
    (*c_timer) --;
  }

  if(*new_submit > 0 || *new_screen > 0){
    if(draw_history(710, 110, history, history_sizes, history_count)) return 1;
    (*new_submit)--;
  }

  if(*c_text > 0 || *new_screen > 0){  
    if(vg_draw_rectangle(705, 740, 390, 80, TEXT_SPACE)) return 1;
    char temp[MAX_STR_LEN];
    if(size_str > MAX_STR_LEN){
      strncpy(temp, string + size_str - MAX_STR_LEN, MAX_STR_LEN);
    } else {
      strncpy(temp, string, MAX_STR_LEN);
    }

    if(draw_string(temp, size_str > MAX_STR_LEN ? MAX_STR_LEN : size_str, 715, 760)) return 1;

    (*c_text) --;
  }
  
  (*new_screen)--;

  return 0;
}

int (draw_countdown_screen)(int elapsed_time, char* subject, int* new_screen) {
  int x;

  if(*new_screen > 0){
    if (60 > elapsed_time && elapsed_time >= 0){
      clear_buffer();
      write_on_center(5, &x);
      if(draw_string("THREE", 5, x, 410)) return 1;
      (*new_screen)--;
    } else if (120 > elapsed_time && elapsed_time >= 60){
      clear_buffer();
      write_on_center(3, &x);
      if(draw_string("TWO", 3, x, 410)) return 1;
      (*new_screen)--;
    } else if (180 > elapsed_time && elapsed_time >= 120){
      clear_buffer();
      write_on_center(3, &x);
      if(draw_string("ONE", 3, x, 410)) return 1;
      (*new_screen)--;
    } else {
      clear_buffer();
      write_on_center(9, &x);
      if(draw_string("DRAW THIS", 9, x, 300)) return 1;
      write_on_center(strlen(subject), &x);
      if(draw_string(subject, strlen(subject), x, 400)) return 1;
      (*new_screen)--;
    }
  }
  
  return 0;
}

int (draw_clear_button)(){
  if(vg_draw_rectangle(495, 795, 155, 55, ACCENT_COLOR)) return 1;
  if(draw_string("CLEAR", 5, 505, 800)) return 1;

  return 0;
  
}

int (draw_history)(int x, int y, char history[MAX_HISTORY_SIZE][3 * MAX_STR_LEN], int history_sizes[MAX_HISTORY_SIZE], int history_count) {
  int areaWidth = 440;
  int areaHeight = 720;  

  if(vg_draw_rectangle(680, 130, areaWidth, areaHeight, ACCENT_COLOR)) return 1;
  
  if(draw_string("TRIES", 5, x + 115, 160)) return 1;
  
  int lineSpacing = 60; 
  y = 660;
  
  int maxLines = MAX_HISTORY_SIZE;
  
  for (int i = history_count - 1; i >= 0; i--) {
    int len = history_sizes[i];

    int nLines = len / MAX_STR_LEN + (len % MAX_STR_LEN == 0 ? 0 : 1);

    for (int j = nLines - 1; j >= 0 && maxLines > 0; j--){
      int lineLen = ((j + 1) * MAX_STR_LEN <= len) ? MAX_STR_LEN : len - j * MAX_STR_LEN;

      char temp[MAX_STR_LEN] = "\0";
      strncpy(temp, history[i] + j * MAX_STR_LEN, lineLen);

      if(draw_string(temp, lineLen, x, y)) return 1;

      y -= lineSpacing;
      maxLines --;
    }

    y -= 20;

    if(maxLines == 0) break;
  }
  
  return 0;
}

