#include "player.h"

struct player_imp {
  uint16_t x, y; // position in the grid
  AnimSprite *left_movement; // pointer to the animated sprite for left movement
  AnimSprite *right_movement; // pointer to the animated sprite for right movement
  AnimSprite *up_movement; // pointer to the animated sprite for up movement
  AnimSprite *down_movement; // pointer to the animated sprite for down movement
  uint8_t current_frame; // current frame of the animation
  uint8_t current_image; // current image of the animation
  Sprite *standing; // pointer to the sprite for standing
  Direction current_d; // current direction of the player
  Direction last_d; // last direction of the player
  uint8_t speed; // number of pixels to move per frame
};

uint16_t get_player_Xpos(Player *p) {
  if (p == NULL) {
    return 0;
  }
  return p->x;
}

uint16_t get_player_Ypos(Player *p) {
  if (p == NULL) {
    return 0;
  }
  return p->y;
}

uint8_t get_player_speed(Player *p) {
  if (p == NULL) {
    return 0;
  }
  return p->speed;
}

Sprite* get_player_standing(Player *p) {
  if (p == NULL) {
    return NULL;
  }
  return p->standing;
}

Player *create_player(uint16_t x, uint16_t y, AnimSprite *left_movement, AnimSprite *right_movement, AnimSprite *up_movement, AnimSprite *down_movement, Sprite *standing) {
  // allocate space for the "object"
  Player *player = (Player *) malloc(sizeof(Player));
  if (player == NULL) {
    return NULL;
  }

  player->x = x;
  player->y = y;
  player->left_movement = left_movement;
  player->right_movement = right_movement;
  player->up_movement = up_movement;
  player->down_movement = down_movement;
  player->standing = standing;
  player->current_d = STANDING; // default direction
  player->last_d = STANDING; // default last direction
  player->current_frame = 0; // default frame
  player->current_image = 0; // default image
  player->speed = 6; // default speed

  return player;
}

void destroy_player(Player *p) {
  if( p == NULL ){
    return;
  }
  
  free(p);
  p = NULL;
}


static void player_changed_direction(Player *p) {
  if (p == NULL || (p->current_d == p->last_d)) {
    return;
  }

  p->current_frame = 0;
  p->current_image = 0;
}

static void update_player_frame(Player *p, AnimSprite *anim_sprite) {
  if (p == NULL) {
    return;
  }

  p->current_frame++;
  if (p->current_frame == get_anim_sprite_aspeed(anim_sprite)) {
    p->current_frame = 0;
    p->current_image++;
    if (p->current_image == get_anim_sprite_num_images(anim_sprite)) {
      p->current_image = 0;
    }
  }
}


void draw_player(Player *p, uint16_t x_initial_grid, uint16_t y_initial_grid) {
  if (p == NULL) {
    return;
  }

  // Calculate the position in pixels based on the grid position
  uint16_t x_pos = (p->x + x_initial_grid);
  uint16_t y_pos = (p->y + y_initial_grid) - 20;

  // Draw the player based on the current direction
  switch (p->current_d) {
    case LEFT:
      draw_anim_sprite(p->left_movement, x_pos, y_pos, p->current_image);
      update_player_frame(p, p->left_movement);
      break;
    case RIGHT:
      draw_anim_sprite(p->right_movement, x_pos, y_pos, p->current_image);
      update_player_frame(p, p->right_movement);
      break;
    case UP:
      draw_anim_sprite(p->up_movement, x_pos, y_pos, p->current_image);
      update_player_frame(p, p->up_movement);
      break;
    case DOWN:
      draw_anim_sprite(p->down_movement, x_pos, y_pos, p->current_image);
      update_player_frame(p, p->down_movement);
      break;
    case STANDING:
      draw_sprite(p->standing, x_pos, y_pos);
      break;
  }
}

void player_move_up(Player *p) {
  if (p == NULL) {
    return;
  }
  p->y -= 1;
  p->last_d = p->current_d;
  p->current_d = UP;
  player_changed_direction(p);
}

void player_move_down(Player *p) {
  if (p == NULL) {
    return;
  }
  p->y += 1;
  p->last_d = p->current_d;
  p->current_d = DOWN;
  player_changed_direction(p);
}

void player_move_left(Player *p) {
  if (p == NULL) {
    return;
  }
  p->x -= 1;
  p->last_d = p->current_d;
  p->current_d = LEFT;
  player_changed_direction(p);
}

void player_move_right(Player *p) {
  if (p == NULL) {
    return;
  }
  p->x += 1;
  p->last_d = p->current_d;
  p->current_d = RIGHT;
  player_changed_direction(p);
}

void player_stand(Player *p) {
  if (p == NULL) {
    return;
  }
  p->last_d = p->current_d;
  p->current_d = STANDING;
  player_changed_direction(p);
}
