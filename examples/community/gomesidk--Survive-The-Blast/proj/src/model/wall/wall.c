#include "wall.h"

struct wall_imp {
  uint8_t x, y; // position of the wall in the grid
  Sprite *img; // pointer to the sprite
  bool is_destroyable; // flag to indicate if the wall is destroyable
  bool active; // flag to indicate if the wall is active, because all walls are created but set to inactive
};

uint8_t get_wall_Xpos(Wall *w) {
  if (w == NULL) {
    return 0;
  }
  return w->x;
}

uint8_t get_wall_Ypos(Wall *w) {
  if (w == NULL) {
    return 0;
  }
  return w->y;
}

bool is_wall_destroyable(Wall *w) {
  if (w == NULL) {
    return false;
  }
  return w->is_destroyable;
}

bool is_wall_active(Wall *w) {
  if (w == NULL) {
    return false;
  }
  return w->active;
}

void set_wall_active(Wall *w, bool active) {
  if (w == NULL) {
    return;
  }
  w->active = active;
}

Sprite* get_wall_sprite(Wall *w) {
  if (w == NULL) {
    return NULL;
  }
  return w->img;
}

Wall *create_wall(uint8_t x, uint8_t y, Sprite *img, bool is_destroyable, bool active) {
  // allocate space for the "object"
  Wall *wall = (Wall *) malloc(sizeof(Wall));
  if (wall == NULL) {
    return NULL;
  }

  wall->x = x;
  wall->y = y;
  wall->img = img;
  wall->is_destroyable = is_destroyable;

  if (!is_destroyable) {
    wall->active = true; // set to active if not destroyable
  } else {
    wall->active = active; 
  }
  return wall;
}

void destroy_wall(Wall *w) {
  if (w == NULL) {
    return;
  }
  
  free(w);
  w = NULL;
}

void draw_wall(Wall *w, uint16_t x_initial_grid, uint16_t y_initial_grid, uint16_t grid_square_width) {
  if (w == NULL || w->active==0) { // Check if the wall is NULL or inactive
    return;
  }
  
  uint16_t x_pos = (w->x*grid_square_width + x_initial_grid);
  uint16_t y_pos = (w->y*grid_square_width + y_initial_grid); 

  draw_sprite(w->img, x_pos, y_pos);
}



