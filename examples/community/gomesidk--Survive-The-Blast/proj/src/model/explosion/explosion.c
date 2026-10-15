#include "explosion.h"

struct explosion_imp {
  uint8_t x, y; // position in the game grid
  AnimSprite* img; // pointer to the animated sprite
  uint8_t current_frame; // current frame of the animation
  uint8_t current_image; // current image of the animation
  bool active;
};

uint8_t get_explosion_Xpos(Explosion *b) { 
  if (b == NULL) {
    return 0;
  }
  return b->x;
}

uint8_t get_explosion_Ypos(Explosion *b) {
  if (b == NULL) {
    return 0;
  }
  return b->y;
}

bool is_explosion_active(Explosion *b) {
  if (b == NULL) {
    return false;
  }
  return b->active;
}

void set_explosion_active(Explosion *b, bool active) {
  if (b == NULL) {
    return;
  }
  b->active = active;
}

Explosion *create_explosion(uint8_t x, uint8_t y, AnimSprite* img) {
  // allocate space for the "object"
  Explosion *explosion = (Explosion *) malloc(sizeof(Explosion));
  if (explosion == NULL) {
    return NULL;
  }

  explosion->x = x;
  explosion->y = y;
  explosion->img = img;
  explosion->current_frame = 0;
  explosion->current_image = 0;
  explosion->active = false;

  return explosion;
}

void destroy_explosion(Explosion *b) {
  if (b == NULL) {
    return;
  }
  
  free(b);
  b = NULL;
}

static void update_explosion_frame(Explosion *b) {
  if (b == NULL) {
    return;
  }

  b->current_frame++;
  if (b->current_frame == get_anim_sprite_aspeed(b->img)) {
    b->current_frame = 0;
    b->current_image++;
    if (b->current_image >= get_anim_sprite_num_images(b->img)) {
      b->current_image = 0;
      b->current_frame = 0;
      b->active = false; // Reset the explosion to inactive after the animation
    }
  }
}

void draw_explosion(Explosion *b, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width) {
  if (b == NULL || (b->active == false)) {
    return;
  }
  // Calculate the pixel position based on the grid position
  uint16_t x_pos = b->x*grid_square_width + x_initial_grid - 10;
  uint16_t y_pos = b->y*grid_square_width + y_initial_grid + 10;

  draw_anim_sprite(b->img, x_pos, y_pos, b->current_image);
  update_explosion_frame(b);
}



