#include "ui_assets.h"
#include "sprite.h"

char my_tolower(char c) {
  if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
  return c;
}

Sprite* get_char_sprite(char c) {
  if (c >= 'a' && c <= 'z')
    return letter_sprites[c - 'a'];
  if (c >= 'A' && c <= 'Z')
    return letter_sprites[c - 'A'];
  if (c >= '0' && c <= '9')
    return number_sprites[c - '0'];
  return NULL;
}

void draw_text(const char* word, uint16_t x, uint16_t y, uint16_t spacing) {
  for (size_t i = 0; word[i] != '\0'; i++) {
    char letter = my_tolower(word[i]);
    Sprite* sprite = get_char_sprite(letter);
    if (sprite != NULL) {
      draw_sprite(sprite, x, y);
      x += sprite->width + spacing;
    }
  }
}

void draw_mouse_cursor(uint16_t x, uint16_t y) {
  draw_sprite(mouseCursor, x, y);
}
