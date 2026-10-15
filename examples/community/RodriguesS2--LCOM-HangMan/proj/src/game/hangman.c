#include "hangman.h"
#include "../assets/game_pixmap.h"
#include "../peripherals/graphics/graphics.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
extern uint16_t h_res;
extern uint16_t v_res;

int draw_hangman(uint16_t x, uint16_t y, int stage) {
  xpm_image_t img;
  uint8_t *pixmap;

  // Select the appropriate pixmap based on the stage
  xpm_row_t const *selected_pixmap;
  switch (stage) {
    case 0: selected_pixmap = man_0; break;
    case 1: selected_pixmap = man_1; break;
    case 2: selected_pixmap = man_2; break;
    case 3: selected_pixmap = man_3; break;
    case 4: selected_pixmap = man_4; break;
    case 5: selected_pixmap = man_5; break;
    case 6: selected_pixmap = final_man; break;
    default:
      printf("Invalid hangman stage: %d\n", stage);
      return 1;
  }

  // Load the selected pixmap
  pixmap = xpm_load(selected_pixmap, XPM_INDEXED, &img);
  if (pixmap == NULL) {
    printf("Failed to load hangman pixmap for stage %d.\n", stage);
    return 1;
  }

  // Check if the pixmap exceeds screen bounds
  if (x + img.width > h_res || y + img.height > v_res) {
    printf("Pixmap exceeds screen bounds.\n");
    return 1;
  }

  // Draw the pixmap at the specified coordinates, scaled by a factor of 4
  for (int row = 0; row < img.height; row++) {
    for (int col = 0; col < img.width; col++) {
      uint32_t color = pixmap[row * img.width + col];
      for (int dy = 0; dy < 1; dy++) {
        for (int dx = 0; dx < 1; dx++) {
          if (vg_draw_pixel(x + col * 1 + dx, y + row * 1 + dy, color) != 0) {
            printf("Failed to draw scaled pixel at (%d, %d).\n", x + col * 1 + dx, y + row * 1 + dy);
            return 1;
          }
        }
      }
    }
  }

  return 0;
}
