#ifndef FONT_H
#define FONT_H

#include "../model/sprite.h"

int (load_font)(void);
void (destroy_font)(void);

void (draw_text)(const char *text, int x, int y);
int (text_width)(const char *text);
void (draw_text_centered)(const char *text, int center_x, int y);

#endif
