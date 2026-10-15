#include "menu.h"
#include "../assets/font.h"
#include "../assets/game_pixmap.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/keyboard/keyboard.h"
#include "../peripherals/mouse/mouse.h"
#include "game.h"
#include "game_state.h"
#include "hangman.h"
#include "instructions.h"
#include "words.h"
#include <lcom/lcf.h>
#include <stdio.h>

extern StateOptions state;

void draw_options(int selected_option) {

  draw_string("LCOM Hangman", (SCREEN_WIDTH - BUTTON_WIDTH) / 2 , 100, 63, 3);

  // Define the labels for the buttons
  const char *labels[] = {"Start", "Instructions", "Exit"};
  const int num_labels = 3;

  // Calculate the total height of all buttons and spacings
  int total_height = (num_labels * BUTTON_HEIGHT) + ((num_labels - 1) * BUTTON_SPACING);

  // Calculate the vertical center of the screen
  int y = (SCREEN_HEIGHT - total_height) / 2;

  // Loop through the labels and draw each button
  for (int i = 0; i < num_labels; i++) {
    
    int x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;

    
    uint32_t color = (i == selected_option) ? HIGHLIGHT_COLOR : BUTTON_COLOR;

    // Draw the button rectangle
    if (vg_draw_rectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, color) != 0) {
      printf("Failed to draw %s button\n", labels[i]);
    }

    // Calculate label width and height based on text size (3x scale)
    int label_width = strlen(labels[i]) * 8 * 3; // Each character is 8 pixels wide, scaled by 3
    int label_height = 8 * 3;                    // Each character is 8 pixels tall, scaled by 3

    
    int label_x = x + (BUTTON_WIDTH - label_width) / 2;
    int label_y = y + (BUTTON_HEIGHT - label_height) / 2;

    // Draw the button label
    if (draw_string(labels[i], label_x, label_y, TEXT_COLOR, 3) != 0) {
      printf("Failed to draw %s text\n", labels[i]);
    }

    
    y += BUTTON_HEIGHT + BUTTON_SPACING;
  }
}

void handle_menu_input(uint8_t scancode, int *selected_option) {

  switch (scancode) {
    case ESC_BREAKCODE:
      gameRunning = false; 
      break;

    case KEY_UP_ARROW:
      // Move up in the menu
      if (*selected_option > 0) {
        (*selected_option)--;
      }
      break;

    case KEY_DOWN_ARROW:
      // Move down in the menu
      if (*selected_option < 2) { // Assuming 3 options
        (*selected_option)++;
      }
      break;

    case KEY_ENTER:
      // Handle selection of the current option
      switch (*selected_option) {
        case 0:
          printf("Start game selected\n");
          state = CATEGORIES;
          break;
        case 1:
          printf("Instructions selected\n");
          state = INSTRUCTIONS;
          draw_instructions_screen();
          break;
        case 2:
          gameRunning = false; // Exit the game
          break;

      }
  }
}

void handle_menu_click(int x, int y, int *selected_option) {
  if (state == MENU) {
    // Calculate the starting position of the first button
    int button_y = (SCREEN_HEIGHT - ((3 * BUTTON_HEIGHT) + (2 * BUTTON_SPACING))) / 2; // Center vertically

    for (int i = 0; i < 3; i++) {
      int button_x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;

      // Check if the click is within the bounds of the button
      if (x >= button_x && x <= button_x + BUTTON_WIDTH &&
          y >= button_y && y <= button_y + BUTTON_HEIGHT) {
        *selected_option = i;                          // Set the selected option based on the clicked button
        handle_menu_input(KEY_ENTER, selected_option); // Simulate Enter key press
        return;
      }

      // Move to the next button position
      button_y += BUTTON_HEIGHT + BUTTON_SPACING;
    }
  }

  if (state == INSTRUCTIONS) {
    // Calculate the position of the "Back" button
    int back_button_x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;  // Center horizontally
    int back_button_y = SCREEN_HEIGHT - BUTTON_HEIGHT - 80; // Position near the bottom

    // Handle clicks on the "Back" button
    if (x >= back_button_x && x <= back_button_x + BUTTON_WIDTH &&
        y >= back_button_y && y <= back_button_y + BUTTON_HEIGHT) {
      state = MENU; // Return to menu on click
      redraw_needed = true;

    }
  }
}

void update_selected_option(int mouse_x, int mouse_y, int *selected_option) {
  if (state != MENU)
    return;

  int button_y = (SCREEN_HEIGHT - ((3 * BUTTON_HEIGHT) + (2 * BUTTON_SPACING))) / 2;
  for (int i = 0; i < 3; i++) {
    int button_x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;
    if (mouse_x >= button_x && mouse_x <= button_x + BUTTON_WIDTH &&
        mouse_y >= button_y && mouse_y <= button_y + BUTTON_HEIGHT) {
          if (*selected_option != i){
            *selected_option = i;
            redraw_needed = true;
          }
      return;
    }
    button_y += BUTTON_HEIGHT + BUTTON_SPACING;
  }
}
