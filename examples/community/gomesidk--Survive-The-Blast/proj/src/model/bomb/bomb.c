#include "bomb.h"

struct bomb_imp {
  uint8_t x, y; // position in the game grid
  AnimSprite* img; // pointer to the animated sprite
  uint8_t current_frame; // current frame of the animation
  uint8_t current_image; // current image of the animation
  bool active;
  bool exploded; // Indicates if the bomb has exploded
  BombType type; // Type of bomb
};

uint8_t get_bomb_Xpos(Bomb *b) { 
  if (b == NULL) {
    return 0;
  }
  return b->x;
}

uint8_t get_bomb_Ypos(Bomb *b) {
  if (b == NULL) {
    return 0;
  }
  return b->y;
}

bool is_bomb_active(Bomb *b) {
  if (b == NULL) {
    return false;
  }
  return b->active;
}

void set_bomb_active(Bomb *b, bool active, BombType type) {
  if (b == NULL) {
    return;
  }
  b->active = active;
  b->type = type;
}

bool is_bomb_exploded(Bomb *b) {
  if (b == NULL || b->active) {
    return false;
  }
  if (b->exploded) {
    b->exploded = false; // Reset exploded status for the next use
    return true;
  }else{
    return false;
  }
}

BombType get_bomb_type(Bomb *b) {
  if (b == NULL) {
    return NORMAL; // Default type
  }
  return b->type;
}

Bomb *create_bomb(uint8_t x, uint8_t y, AnimSprite* img, bool active) {
  // allocate space for the "object"
  Bomb *bomb = (Bomb *) malloc(sizeof(Bomb));
  if (bomb == NULL) {
    return NULL;
  }

  bomb->x = x;
  bomb->y = y;
  bomb->img = img;
  bomb->current_frame = 0;
  bomb->current_image = 0;
  bomb->active = active;
  bomb->exploded = false; // Initialize exploded status to false
  bomb->type = NORMAL; // Default bomb type

  return bomb;
}

void destroy_bomb(Bomb *b) {
  if (b == NULL) {
    return;
  }

  free(b);
  b = NULL;
}

static void update_bomb_frame(Bomb *b) {
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
      b->active = false; // Reset the bomb to inactive after the animation
      b->exploded = true;
    }
  }
}

void draw_bomb(Bomb *b, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width) {
  if (b == NULL || (b->active == false)) {
    return;
  }
  // Calculate the pixel position based on the grid position
  uint16_t x_pos = b->x*grid_square_width + x_initial_grid;
  uint16_t y_pos = b->y*grid_square_width + y_initial_grid + 10;

  draw_anim_sprite(b->img, x_pos, y_pos, b->current_image);
  update_bomb_frame(b);
}



