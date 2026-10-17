#include <lcom/lcf.h>
#include "libs/video.h"
#include "libs/sprites.h"
#include "enum.h"
#include "text.h"

int text_width, text_height;
char* text_sprite;
int char_width = 12;
int char_height = 24;

void init_text(){
  text_sprite = read_xpm(alphabet_xpm, &text_width, &text_height);
}

void destroy_text(){
  free(text_sprite);
}

void print_char(char c, uint32_t y, uint32_t x, uint32_t color, int size_text){
  if (c < 32 || c > 126)
    return;
  uint16_t begin_x = ((int)(c-32)) * char_width;
  vg_draw_text_sprite( text_sprite, text_height, text_width,
    char_height, char_width, 0, begin_x, y, x, color, size_text);
}

void print_string(char* string, uint32_t size, uint32_t y, uint32_t x, uint32_t color, int size_text, int align){
  if (size_text <= 0) return;
  uint32_t begin;
  if (align == ALIGN_LEFT)
    begin = x;
  else if (align == ALIGN_CENTER){
    begin = x - size_text * size * char_width / 2;
    y = y - char_height*size_text/2;
  } else
    begin = x - size_text * size * char_width;
  for (uint32_t i = 0; i < size; i++){
    print_char(string[i], y, begin + i * size_text * char_width, color, size_text);
  }
}
