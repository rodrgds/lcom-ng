#include <lcom/lcf.h>

#include "elements.h"
#include "sprite.h"
#include "video.h"

static int check_width_limits_collision(Sprite *sp) {
  //check collision with left limit
  if (sp->x + sp->xspeed < FIELD_LEFT_LIMIT_PX) {
    sp->x = FIELD_LEFT_LIMIT_PX;
    return 1;
  }
  //check collision with right limit
  if (sp->x + sp->xspeed + sp->width > FIELD_RIGHT_LIMIT_PX) {
    sp->x = FIELD_RIGHT_LIMIT_PX - sp->width;
    return 1;
  }
  return 0;
}

static int check_height_limits_collision(Sprite *sp) {
  //check collision with top limit
  if (sp->y + sp->yspeed < FIELD_TOP_LIMIT_PX) {
    sp->y = FIELD_TOP_LIMIT_PX;
    return 1;
  }
  //check collision with bottom limit
  if (sp->y + sp->yspeed + sp->height > FIELD_BOTTOM_LIMIT_PX) {
    sp->y = FIELD_BOTTOM_LIMIT_PX - sp->height;
    return 1;
  }
  return 0;
}

/**
 * @brief Detect if sp1 is going to collide with sp2 and return the direction that sp1 is going in
 * 
 * @param sp1 sprite that is the subject of this verification
 * @param sp2 sprite that sp1 could possibly collide with
 * @return the direction of the detected collision (LEFT, RIGHT, UP, DOWN); NO if the sprites don't collide
 */
static direction check_collision(Sprite *sp1, Sprite *sp2) {
  //check if the sprites share a horizontal line
  if (sp1->y + sp1->yspeed < sp2->y + sp2->height && sp1->y + sp1->yspeed + sp1->height > sp2->y) {
    //check leftwards collision
    if (sp1->x >= sp2->x + sp2->width && sp1->x + sp1->xspeed < sp2->x + sp2->width && sp1->xspeed < 0) {
      return LEFT;
    }
    //check rightwards collision
    if (sp1->x + sp1->width <= sp2->x && sp1->x + sp1->xspeed + sp1->width > sp2->x && sp1->xspeed > 0) {
      return RIGHT;
    }
  }
  //check if the sprites share a vertical line
  if (sp1->x + sp1->xspeed < sp2->x + sp2->width && sp1->x + sp1->xspeed + sp1->width > sp2->x) {
    //check upwards collision
    if (sp1->y >= sp2->y + sp2->height && sp1->y + sp1->yspeed < sp2->y + sp2->height && sp1->yspeed < 0) {
      return UP;
    }
    //check downwards collision
    if (sp1->y + sp1->height <= sp2->y && sp1->y + sp1->yspeed + sp1->height > sp2->y && sp1->yspeed > 0) {
      return DOWN;
    }
  }
  return NO;
}

/**
 * @brief Check if the ball sprite's movement makes it collide horizontally with the player sprite, and move both accordingly.
 * 
 * @param ball ball sprite
 * @param player player sprite
 * @return 1 if the sprites collide horizontally, -1 if they collide near a limit, 0 if they don't collide
 */
static int check_ball_to_player_sprite_hor_collision(Sprite *ball, Sprite *player) {
  switch (check_collision(ball, player)) {
    case LEFT:
      //check if the player is near the left limit
      if (player->x + ball->xspeed + (player->xspeed > 0 ? player->xspeed : 0) < FIELD_LEFT_LIMIT_PX) {
        player->x = FIELD_LEFT_LIMIT_PX;
        ball->x = player->x + player->width;
        return -1;
      }
      else {
        ball->x += ball->xspeed + (player->xspeed > 0 ? player->xspeed : 0);
        player->x += ball->xspeed + (player->xspeed > 0 ? player->xspeed : 0);
        return 1;
      }
      break;
    case RIGHT:
      //check if the player is near the right limit
      if (player->x + ball->xspeed + (player->xspeed < 0 ? player->xspeed : 0) + player->width > FIELD_RIGHT_LIMIT_PX) {
        player->x = FIELD_RIGHT_LIMIT_PX - player->width;
        ball->x = player->x - ball->width;
        return -1;
      }
      else {
        ball->x += ball->xspeed + (player->xspeed < 0 ? player->xspeed : 0);
        player->x += ball->xspeed + (player->xspeed < 0 ? player->xspeed : 0);
        return 1;
      }
      break;
    default:
      return 0;
      break;
  }
}

/**
 * @brief Check if the ball sprite's movement makes it collide vertically with the player sprite, and move both accordingly.
 * 
 * @param ball ball sprite
 * @param player player sprite
 * @return 1 if the sprites collide vertically, -1 if they collide near a limit, 0 if they don't collide
 */
static int check_ball_to_player_sprite_ver_collision(Sprite *ball, Sprite *player) {
  switch (check_collision(ball, player)) {
    case UP:
      //check if the player is near the top limit
      if (player->y + ball->yspeed + (player->yspeed > 0 ? player->yspeed : 0) < FIELD_TOP_LIMIT_PX) {
        player->y = FIELD_TOP_LIMIT_PX;
        ball->y = player->y + player->height;
        return -1;
      }
      else {
        ball->y += ball->yspeed + (player->yspeed > 0 ? player->yspeed : 0);
        player->y += ball->yspeed + (player->yspeed > 0 ? player->yspeed : 0);
        return 1;
      }
      break;
    case DOWN:
      //check if the player is near the bottom limit
      if (player->y + ball->yspeed + (player->yspeed < 0 ? player->yspeed : 0) + player->height > FIELD_BOTTOM_LIMIT_PX) {
        player->y = FIELD_BOTTOM_LIMIT_PX - player->height;
        ball->y = player->y - ball->height;
        return -1;
      }
      else {
        ball->y += ball->yspeed + (player->yspeed < 0 ? player->yspeed : 0);
        player->y += ball->yspeed + (player->yspeed < 0 ? player->yspeed : 0);
        return 1;
      }
      break;
    default:
      return 0;
      break;
  }
}

/**
 * @brief Check if the player sprite's movement makes it collide horizontally with the ball sprite, and move both accordingly.
 * 
 * @param player player sprite
 * @param ball ball sprite
 * @return 1 if the sprites collide horizontally, -1 if they collide near a limit, 0 if they don't collide
 */
static int check_player_to_ball_sprite_hor_collision(Sprite *player, Sprite *ball) {
  switch (check_collision(player, ball)) {
    case LEFT:
      //check if the ball is near the left limit
      if (ball->x + player->xspeed + (ball->xspeed > 0 ? ball->xspeed : 0) < FIELD_LEFT_LIMIT_PX) {
        //check if the ball is near the left goal
        if (ball->y >= GOAL_TOP_POST_LIMIT_PX && ball->y + ball->height <= GOAL_BOTTOM_POST_LIMIT_PX) {
          player->x += player->xspeed + (ball->xspeed > 0 ? ball->xspeed : 0);
          ball->x += player->xspeed + (ball->xspeed > 0 ? ball->xspeed : 0);
        }
        else {
          player->x = FIELD_LEFT_LIMIT_PX;
          ball->x = player->x + player->width;
        }
        return -1;
      }
      else {
        int ball_xspeed = ball->xspeed;
        ball->xspeed += ball->xspeed >= 0 ? player->xspeed : 0;

        Sprite *player2;

        if (player->x == get_elements()[RED_PLAYER_IDX]->x && player->y == get_elements()[RED_PLAYER_IDX]->y)
          player2 = get_elements()[BLUE_PLAYER_IDX];
        else
          player2 = get_elements()[RED_PLAYER_IDX];

        //check if the resulting collision also affects the other player
        int player_col = check_ball_to_player_sprite_hor_collision(ball, player2);

        ball->xspeed = ball_xspeed;

        if (player_col != -1)
          player->x += player->xspeed + (ball->xspeed > 0 ? ball->xspeed : 0);

        if (player_col == 0)
          ball->x += player->xspeed + (ball->xspeed > 0 ? ball->xspeed : 0);

        return 1;
      }
      break;
    case RIGHT:
      //check if the ball is near the right limit
      if (ball->x + player->xspeed + (ball->xspeed < 0 ? ball->xspeed : 0) + ball->width > FIELD_RIGHT_LIMIT_PX) {
        //check if the ball is near the right goal
        if (ball->y >= GOAL_TOP_POST_LIMIT_PX && ball->y + ball->height <= GOAL_BOTTOM_POST_LIMIT_PX) {
          player->x += player->xspeed + (ball->xspeed < 0 ? ball->xspeed : 0);
          ball->x += player->xspeed + (ball->xspeed < 0 ? ball->xspeed : 0);
        }
        else {
          player->x = FIELD_RIGHT_LIMIT_PX - player->width;
          ball->x = player->x - ball->width;
        }
        return -1;
      }
      else {
        int ball_xspeed = ball->xspeed;
        ball->xspeed += ball->xspeed <= 0 ? player->xspeed : 0;

        Sprite *player2;

        if (player->x == get_elements()[RED_PLAYER_IDX]->x && player->y == get_elements()[RED_PLAYER_IDX]->y)
          player2 = get_elements()[BLUE_PLAYER_IDX];
        else
          player2 = get_elements()[RED_PLAYER_IDX];

        //check if the resulting collision also affects the other player
        int player_col = check_ball_to_player_sprite_hor_collision(ball, player2);

        ball->xspeed = ball_xspeed;

        if (player_col != -1)
          player->x += player->xspeed + (ball->xspeed < 0 ? ball->xspeed : 0);

        if (player_col == 0)
          ball->x += player->xspeed + (ball->xspeed < 0 ? ball->xspeed : 0);

        return 1;
      }
      break;
    default:
      return 0;
      break;
  }
}

/**
 * @brief Check if the player sprite's movement makes it collide vertically with the ball sprite, and move both accordingly.
 * 
 * @param player player sprite
 * @param ball ball sprite
 * @return 1 if the sprites collide vertically, -1 if they collide near a limit, 0 if they don't collide
 */
static int check_player_to_ball_sprite_ver_collision(Sprite *player, Sprite *ball) {
  switch (check_collision(player, ball)) {
    case UP:
      //check if the ball is near the top limit
      if (ball->y + player->yspeed + (ball->yspeed > 0 ? ball->yspeed : 0) < FIELD_TOP_LIMIT_PX) {
        player->y = FIELD_TOP_LIMIT_PX;
        ball->y = player->y + player->height;
        return -1;
      }
      else {
        int ball_yspeed = ball->yspeed;
        ball->yspeed += ball->yspeed >= 0 ? player->yspeed : 0;

        Sprite *player2;

        if (player->x == get_elements()[RED_PLAYER_IDX]->x && player->y == get_elements()[RED_PLAYER_IDX]->y)
          player2 = get_elements()[BLUE_PLAYER_IDX];
        else
          player2 = get_elements()[RED_PLAYER_IDX];

        //check if the resulting collision also affects the other player
        int player_col = check_ball_to_player_sprite_ver_collision(ball, player2);

        ball->yspeed = ball_yspeed;

        if (player_col != -1)
          player->y += player->yspeed + (ball->yspeed > 0 ? ball->yspeed : 0);

        if (player_col == 0)
          ball->y += player->yspeed + (ball->yspeed > 0 ? ball->yspeed : 0);

        return 1;
      }
      break;
    case DOWN:
      //check if the ball is near the bottom limit
      if (ball->y + player->yspeed + (ball->yspeed < 0 ? ball->yspeed : 0) + ball->height > FIELD_BOTTOM_LIMIT_PX) {
        player->y = FIELD_BOTTOM_LIMIT_PX - player->height;
        ball->y = player->y - ball->height;
        return -1;
      }
      else {
        int ball_yspeed = ball->yspeed;
        ball->yspeed += ball->yspeed <= 0 ? player->yspeed : 0;

        Sprite *player2;

        if (player->x == get_elements()[RED_PLAYER_IDX]->x && player->y == get_elements()[RED_PLAYER_IDX]->y)
          player2 = get_elements()[BLUE_PLAYER_IDX];
        else
          player2 = get_elements()[RED_PLAYER_IDX];

        //check if the resulting collision also affects the other player
        int player_col = check_ball_to_player_sprite_ver_collision(ball, player2);

        ball->yspeed = ball_yspeed;

        if (player_col != -1)
          player->y += player->yspeed + (ball->yspeed < 0 ? ball->yspeed : 0);

        if (player_col == 0)
          ball->y += player->yspeed + (ball->yspeed < 0 ? ball->yspeed : 0);

        return 1;
      }
      break;
    default:
      return 0;
      break;
  }
}

/**
 * @brief Check if player sprite sp1's movement makes it collide horizontally with player sprite sp2, and move both accordingly.
 * 
 * @param sp1 first player sprite
 * @param sp2 second player sprite
 * @return 1 if the sprites collide horizontally, 0 otherwise
 */
static int check_player_to_player_sprite_hor_collision(Sprite *sp1, Sprite *sp2) {
  switch (check_collision(sp1, sp2)) {
    case LEFT:
      //check if sp2 is near the left limit
      if (sp2->x + sp1->xspeed + (sp2->xspeed > 0 ? sp2->xspeed : 0) < FIELD_LEFT_LIMIT_PX) {
        sp2->x = FIELD_LEFT_LIMIT_PX;
        sp1->x = sp2->x + sp2->width;
      }
      else {
        int sp2_xspeed = sp2->xspeed;
        sp2->xspeed += sp2->xspeed >= 0 ? sp1->xspeed : 0;

        //check if the resulting collision also affects the ball
        int ball_col = check_player_to_ball_sprite_hor_collision(sp2, get_elements()[BALL_IDX]);

        sp2->xspeed = sp2_xspeed;

        if (ball_col != -1)
          sp1->x += sp1->xspeed + (sp2->xspeed > 0 ? sp2->xspeed : 0);

        if (ball_col == 0)
          sp2->x += sp1->xspeed + (sp2->xspeed > 0 ? sp2->xspeed : 0);
      }
      return 1;
      break;
    case RIGHT:
      //check if sp2 is near the right limit
      if (sp2->x + sp1->xspeed + (sp2->xspeed < 0 ? sp2->xspeed : 0) + sp2->width > FIELD_RIGHT_LIMIT_PX) {
        sp2->x = FIELD_RIGHT_LIMIT_PX - sp2->width;
        sp1->x = sp2->x - sp1->width;
      }
      else {
        int sp2_xspeed = sp2->xspeed;
        sp2->xspeed += sp2->xspeed <= 0 ? sp1->xspeed : 0;

        //check if the resulting collision also affects the ball
        int ball_col = check_player_to_ball_sprite_hor_collision(sp2, get_elements()[BALL_IDX]);

        sp2->xspeed = sp2_xspeed;

        if (ball_col != -1)
          sp1->x += sp1->xspeed + (sp2->xspeed < 0 ? sp2->xspeed : 0);

        if (ball_col == 0)
          sp2->x += sp1->xspeed + (sp2->xspeed < 0 ? sp2->xspeed : 0);
      }
      return 1;
      break;
    default:
      return 0;
      break;
  }
}

/**
 * @brief Check if player sprite sp1's movement makes it collide vertically with player sprite sp2, and move both accordingly.
 * 
 * @param sp1 first player sprite
 * @param sp2 second player sprite
 * @return 1 if the sprites collide vertically, 0 otherwise
 */
static int check_player_to_player_sprite_ver_collision(Sprite *sp1, Sprite *sp2) {
  switch (check_collision(sp1, sp2)) {
    case UP:
      //check if sp2 is near the top limit
      if (sp2->y + sp1->yspeed + (sp2->yspeed > 0 ? sp2->yspeed : 0) < FIELD_TOP_LIMIT_PX) {
        sp2->y = FIELD_TOP_LIMIT_PX;
        sp1->y = sp2->y + sp2->height;
      }
      else {
        int sp2_yspeed = sp2->yspeed;
        sp2->yspeed += sp2->yspeed >= 0 ? sp1->yspeed : 0;

        //check if the resulting collision also affects the ball
        int ball_col = check_player_to_ball_sprite_ver_collision(sp2, get_elements()[BALL_IDX]);

        sp2->yspeed = sp2_yspeed;

        if (ball_col != -1)
          sp1->y += sp1->yspeed + (sp2->yspeed > 0 ? sp2->yspeed : 0);

        if (ball_col == 0)
          sp2->y += sp1->yspeed + (sp2->yspeed > 0 ? sp2->yspeed : 0);
      }
      return 1;
      break;
    case DOWN:
      //check if sp2 is near the bottom limit
      if (sp2->y + sp1->yspeed + (sp2->yspeed < 0 ? sp2->yspeed : 0) + sp2->height > FIELD_BOTTOM_LIMIT_PX) {
        sp2->y = FIELD_BOTTOM_LIMIT_PX - sp2->height;
        sp1->y = sp2->y - sp1->height;
      }
      else {
        int sp2_yspeed = sp2->yspeed;
        sp2->yspeed += sp2->yspeed <= 0 ? sp1->yspeed : 0;

        //check if the resulting collision also affects the ball
        int ball_col = check_player_to_ball_sprite_ver_collision(sp2, get_elements()[BALL_IDX]);

        sp2->yspeed = sp2_yspeed;

        if (ball_col != -1)
          sp1->y += sp1->yspeed + (sp2->yspeed < 0 ? sp2->yspeed : 0);

        if (ball_col == 0)
          sp2->y += sp1->yspeed + (sp2->yspeed < 0 ? sp2->yspeed : 0);
      }
      return 1;
      break;
    default:
      return 0;
      break;
  }
}

static void move_sprite(Sprite *sp) {
  Sprite *player1, *player2, *ball;

  if (sp->x == get_elements()[RED_PLAYER_IDX]->x && sp->y == get_elements()[RED_PLAYER_IDX]->y) {
    player1 = get_elements()[RED_PLAYER_IDX];
    player2 = get_elements()[BLUE_PLAYER_IDX];
    ball = get_elements()[BALL_IDX];
  }
  else if (sp->x == get_elements()[BLUE_PLAYER_IDX]->x && sp->y == get_elements()[BLUE_PLAYER_IDX]->y) {
    player1 = get_elements()[BLUE_PLAYER_IDX];
    player2 = get_elements()[RED_PLAYER_IDX];
    ball = get_elements()[BALL_IDX];
  }
  else
    return;

  //check if there are any horizontal collisions
  if (!check_width_limits_collision(sp) && !check_player_to_player_sprite_hor_collision(player1, player2) && !check_player_to_ball_sprite_hor_collision(player1, ball))
    sp->x += sp->xspeed;

  //check if there are any vertical collisions
  if (!check_height_limits_collision(sp) && !check_player_to_player_sprite_ver_collision(player1, player2) && !check_player_to_ball_sprite_ver_collision(player1, ball))
    sp->y += sp->yspeed;
}

/** Creates a new sprite with pixmap "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed) {
  //allocate space for the "object"
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;

  if (sp == NULL)
    return NULL;

  // read the sprite pixmap
  sp->map = xpm_load(xpm, XPM_8_8_8_8, &img);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->x = x;
  sp->y = y;

  sp->width = img.width;
  sp->height = img.height;

  sp->xspeed = xspeed;
  sp->yspeed = yspeed;

  return sp;
}

void draw_sprite(Sprite *sp) {
  draw_pixmap(sp->map, sp->x, sp->y, sp->width, sp->height);
}

void erase_sprite(Sprite *sp) {
  erase_pixmap(sp->x, sp->y, sp->width, sp->height);
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL)
    return;
  if (sp->map)
    free(sp->map);
  free(sp);
  sp = NULL; // XXX: pointer is passed by value
             // should do this @ the caller
}

void animate_sprites(Sprite **sprites) {
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    erase_sprite(sprites[i]);
  }
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    move_sprite(sprites[i]);
  }
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    draw_sprite(sprites[i]);
  }
}

void set_direction(Sprite *sp, direction dir) {
  switch (dir) {
    case LEFT:
      sp->xspeed = -2;
      break;
    case RIGHT:
      sp->xspeed = 2;
      break;
    case UP:
      sp->yspeed = -2;
      break;
    case DOWN:
      sp->yspeed = 2;
      break;
    default:
      break;
  }
}

void clear_direction(Sprite *sp, direction dir) {
  switch (dir) {
    case LEFT:
      sp->xspeed = 0;
      break;
    case RIGHT:
      sp->xspeed = 0;
      break;
    case UP:
      sp->yspeed = 0;
      break;
    case DOWN:
      sp->yspeed = 0;
      break;
    default:
      break;
  }
}
