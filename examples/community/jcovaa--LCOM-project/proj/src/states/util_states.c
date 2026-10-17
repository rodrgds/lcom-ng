#include "util_states.h"
#include "../drivers/video/graphics.h"
#include "../ui_assets.h"
#include <string.h>

extern uint16_t x_resolution;

uint16_t calc_paragraph_width(const char** paragraph, size_t size, uint16_t word_spacing) {
  uint16_t width = 0;
  for (size_t i = 0; i < size; i++) {
    width += strlen(paragraph[i]) * (16 + 5);
  }
  width += word_spacing * (size - 1);
  return width;
}

void draw_centered_paragraph(const char** paragraph, size_t size, uint16_t y, uint16_t word_spacing, uint8_t text_scale) {
  uint16_t para_width = calc_paragraph_width(paragraph, size, word_spacing);
  uint16_t current_x = x_resolution / 2 - para_width / 2;
  
  for (size_t i = 0; i < size; i++) {
    draw_text(paragraph[i], current_x, y, text_scale);
    current_x += strlen(paragraph[i]) * (16 + 5) + word_spacing;
  }
}
