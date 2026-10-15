#include "sprite_view.h"


int(draw_sprite)(Sprite *sprite, uint16_t x, uint16_t y, uint8_t *buffer) {
  
  int height = sprite->height;
  int width = sprite->width;
  uint32_t color;
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      color = sprite->map[i * width + j];
      if (color == xpm_transparency_color(XPM_8_8_8_8)) continue; // skiping transparent pixels
      if (draw_pixel(x + j, y + i, color, buffer) != 0) return 1; 
    }
  }
  return 0;
}


int(draw_sprite_highlighted)(Sprite *sprite, uint16_t x, uint16_t y, uint8_t *buffer) {
  int height = sprite->height;
  int width = sprite->width;
  uint32_t color;
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      color = sprite->map[i * width + j];
      
      // Skip transparent pixels
      if (color == xpm_transparency_color(XPM_8_8_8_8)) continue;
      
      // Make the color brighter (highlight effect)
      uint8_t r = ((color >> 16) & 0xFF);
      uint8_t g = ((color >> 8) & 0xFF);
      uint8_t b = (color & 0xFF);
      
      // Brighten the color (clamped to 255)
      r = (r + 50 > 255) ? 255 : r + 50;
      g = (g + 50 > 255) ? 255 : g + 50;
      b = (b + 50 > 255) ? 255 : b + 50;
      
      uint32_t highlighted_color = (r << 16) | (g << 8) | b;
      
      if (draw_pixel(x + j, y + i, highlighted_color, buffer) != 0) return 1;
    }
  }
  return 0;
}



int draw_sprite_recolor(Sprite *sprite, uint16_t x, uint16_t y, uint32_t recolor, uint8_t *buffer) {
  
  int height = sprite->height;
  int width = sprite->width;
  uint32_t color;
  
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      color = sprite->map[i * width + j];
      if (color == xpm_transparency_color(XPM_8_8_8_8)) continue; 
      if (draw_pixel(x + j, y + i, recolor, buffer) != 0) return 1; 
    }
  }
  return 0;
}


