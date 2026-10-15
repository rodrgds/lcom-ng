#include "instructions.h"
#include "../assets/font.h"
#include "../assets/game_pixmap.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/keyboard/keyboard.h"
#include "game.h"
#include "game_state.h"
#include "hangman.h"
#include "menu.h"
#include <lcom/lcf.h>
#include <stdio.h>


void draw_instructions_screen(void) {

  const char *instructions[] = {"Welcome to Hangman!\n\n",
                                "Rules:\n",
                                "- Guess the hidden word one letter at a time.\n",
                                "- Each incorrect guess draws a part of the hangman.\n",
                                "- You win if you guess the word before the hangman is \n",
                                "  complete.\n",
                                "- Use the arrow keys to navigate.\n",
                                "- Press ENTER to select.\n",
                                "- Press ESC to GO BACK. \n",
                                "Good luck!"};

  // 'Instructions' position
  int ix = (SCREEN_WIDTH - (strlen("Instructions") * 8 * 4)) / 2;
  int iy = 100;

  // button position
  int bx = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
  int by = SCREEN_HEIGHT - BUTTON_HEIGHT - 80;

  extern int mouse_x, mouse_y;
  uint32_t back_color = BUTTON_COLOR;
  if (mouse_x >= bx && mouse_x <= bx + BUTTON_WIDTH &&
      mouse_y >= by && mouse_y <= by + BUTTON_HEIGHT) {
    back_color = HIGHLIGHT_COLOR;
  }

  // Center the "Back" text on the button
  const char *back_label = "Back";
  int label_width = strlen(back_label) * 8 * 2;
  int label_height = 8 * 2;
  int label_x = bx + (BUTTON_WIDTH - label_width) / 2;
  int label_y = by + (BUTTON_HEIGHT - label_height) / 2;

  // instructions position
  int num_lines = sizeof(instructions) / sizeof(instructions[0]);
  int line_spacing = 20 * 2;
  int x = 100;
  int y = (SCREEN_HEIGHT - ((num_lines - 1) * line_spacing)) / 2;

  // Draw 'Instructions'
  draw_string("Instructions", ix, iy, TITLE_COLOR, 4);

  // Draw the instructions
  for (int i = 0; i < num_lines; i++) {
    draw_string(instructions[i], x, y + i * line_spacing, TEXT_COLOR, 2);
  }

  // Draw the button rectangle
  vg_draw_rectangle(bx, by, BUTTON_WIDTH, BUTTON_HEIGHT, back_color);

  // Draw the 'Back' button
  draw_string(back_label, label_x, label_y, TEXT_COLOR, 2);


  return;
}
