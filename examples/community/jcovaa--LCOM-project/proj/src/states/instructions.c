#include "instructions.h"
#include "../drivers/video/graphics.h"
#include "../drivers/kbc/keyboard.h"
#include "../ui_assets.h"
#include "util_states.h" 

#include <string.h>

extern uint8_t scancode;
extern uint16_t x_resolution;
extern uint16_t y_resolution;

void draw_instructions() {
  set_white_background();

  draw_text("INSTRUCTIONS", x_resolution / 2 - 120, 80, 5);

  uint16_t text_y = 140;
  uint16_t line_spacing = 30;
  uint16_t word_spacing = 10;

  const char* paragraph1[] = {"YOU", "WILL", "SEE", "A", "PAITING", "IN", "A"};
  const char* paragraph2[] = {"GRID", "FORMAT", "THAT", "YOU", "HAVE"};
  const char* paragraph3[] = {"TO", "REMEMBER"};

  const char* paragraph4[] = {"YOU", "HAVE", "15", "SECONDS", "TO", "SEE"};
  const char* paragraph5[] = {"THE", "ORIGINAL", "PAINTING"};

  const char* paragraph6[] = {"AFTER", "THAT", "YOU", "WILL", "HAVE"};
  const char* paragraph7[] = {"TO", "PAINT", "IT"};

  draw_centered_paragraph(paragraph1, sizeof(paragraph1) / sizeof(paragraph1[0]), text_y, word_spacing, 3);
  
  draw_centered_paragraph(paragraph2, sizeof(paragraph2) / sizeof(paragraph2[0]), text_y + line_spacing, word_spacing, 3);
  
  draw_centered_paragraph(paragraph3, sizeof(paragraph3) / sizeof(paragraph3[0]), text_y + 2 * line_spacing, word_spacing, 3);
  
  draw_centered_paragraph(paragraph4, sizeof(paragraph4) / sizeof(paragraph4[0]), text_y + 4 * line_spacing, word_spacing, 3);
  
  draw_centered_paragraph(paragraph5, sizeof(paragraph5) / sizeof(paragraph5[0]), text_y + 5 * line_spacing, word_spacing, 3);
  
  draw_centered_paragraph(paragraph6, sizeof(paragraph6) / sizeof(paragraph6[0]), text_y + 7 * line_spacing, word_spacing, 3);
  
  draw_centered_paragraph(paragraph7, sizeof(paragraph7) / sizeof(paragraph7[0]), text_y + 8 * line_spacing, word_spacing, 3);

  static int flash_conter = 0;
  flash_conter = (flash_conter + 1) % 30;

  if (flash_conter < 15) {
    const char* continue_text[] = {"PRESS", "ENTER", "TO", "CONTINUE"};
    draw_centered_paragraph(continue_text, sizeof(continue_text) / sizeof(continue_text[0]), y_resolution - 100, word_spacing, 3);
  }
}

void handle_instructions_input(GameState *state) {
  if (scancode != 0) {
    switch (scancode) {
      case ENTER_KEY:
        *state = SHOW_ORIGINAL;
        break;
      case ESC_KEY:
        *state = START_MENU;
        break;
    }
    scancode = 0;
  }
}
