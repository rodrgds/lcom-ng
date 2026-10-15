#ifndef DIGITS_H
#define DIGITS_H

#include <stdint.h>

void draw_digit(int x, int y, int digit, uint32_t color);

void draw_letter(int x, int y, char letter, uint32_t color);

void draw_text(int x, int y, const char* text, uint32_t color);

void draw_number(int x, int y, int number, uint32_t color);

#endif
