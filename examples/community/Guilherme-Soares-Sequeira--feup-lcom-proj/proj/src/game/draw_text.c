#include <lcom/lcf.h>

#include "../devices/video/graphics.h"
#include "charset.h"
#include "draw_text.h"
#include "text.h"

int text_size = 0;                    /**< @brief size of the piece of text */
char text[INPUT_CHAR_SIZE + 1] = {0}; /**< @brief the text currently being edited */

void(add_char_to_current_text)(char c) {

  if (text_size >= INPUT_CHAR_SIZE - 1)
    return;

  if (c == NO_ADD_CHAR)
    return;

  text[text_size++] = c;
}

void(remove_char_from_current_text)() {

  if (text_size <= 0)
    return;

  text[text_size] = 0; // so we do not leave trailing characters
  text[--text_size] = 0;
}

void(clear_current_text)() {
  while (text_size > 0)
    remove_char_from_current_text();
}

char *(get_current_text) () {
  return text;
}
