#ifndef DRAW_FUNCTIONS_H_
#define DRAW_FUNCTIONS_H_

#define MAX_WORDS 100

#include <stdbool.h>
#include "../Modules/vg.h"
#include "../Modules/sprites.h"




int draw_sentence(char* sentence, int x, int y, char* color);
int get_xpm_height(xpm_row_t const xpm[]);
int get_xpm_width(xpm_row_t const xpm[]);
char** split_phrase(const char* phrase);
void cache_line_heights(const char* word, int* height_cache);
void refresh_frame_buffer();
char breakcode_to_ascii(uint8_t breakcode);
const xpm_row_t* get_letter_sprite_green(char letter);
const xpm_row_t* get_letter_sprite(char letter);
const xpm_row_t* get_letter_sprite_black(char letter);
const xpm_row_t* get_letter_sprite_red(char letter);
void update_frame(int x1 , int x2 , int y1 , int y2, char* element , int newX , int newY);


void draw_start_game();


void refresh_frame_buffer();


#endif

