#include <lcom/lcf.h>

#include "model/game/combat.h"

void(drop_bomb)(Game *game, int16_t x, int16_t y) {
  if (game == NULL) {
    fprintf(stderr, "drop_bomb: game pointer cannot be null.");
    return;
  }

  if (x < 0 || x >= game->board.width || y < 0 || y >= game->board.height) {
    fprintf(stderr, "drop_bomb: coordinates (%d, %d) are out of bounds.", x, y);
    return;
  }

  if (!game->player.active) {
    return;
  }

  int16_t dx = abs(x - game->player.x);
  int16_t dy = abs(y - game->player.y);

  if (game->num_bombs >= MAX_BOMBS || game->board.elements[y][x] != EMPTY_SPACE || dx > 1 || dy > 1) {
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "drop_bomb: failed to load resources.");
    return;
  }

  uint8_t bomb_index = game->num_bombs;

  if (init_entity(&game->bombs[bomb_index], x, y, resources->bomb_sprites[0], 180) != 0) {
    fprintf(stderr, "drop_bomb: failed to initialize bomb entity.");
    return;
  }

  game->board.elements[y][x] = BOMB;

  game->bombs[bomb_index].anim = create_animSprite(resources->bomb_sprites, BOMB_ANIM_FRAMES, BOMB_ANIM_SPEED, true);
  game->bombs[bomb_index].sprite = game->bombs[bomb_index].anim->sp;
  game->bombs[bomb_index].active = true;
  game->num_bombs++;
}

/**
 * @brief Adds an explosion entity at the specified coordinates
 *
 * Creates a new explosion entity with animation at the given position.
 * Used internally by explode_bomb to create explosion effects.
 *
 * @param game Pointer to the current game state
 * @param x X-coordinate for the explosion
 * @param y Y-coordinate for the explosion
 * @param frames Array of sprite frames for the explosion animation
 */
static void(add_explosion)(Game *game, int16_t x, int16_t y, Sprite **frames) {
  if (game == NULL) {
    fprintf(stderr, "add_explosion: game pointer cannot be null.");
    return;
  }

  if (game->num_explosions >= MAX_EXPLOSIONS)
    return;
  if (!frames || !frames[0]) {
    fprintf(stderr, "add_explosion: NULL frames");
    return;
  }

  Entity *e = &game->explosions[game->num_explosions];
  if (init_entity(e, x, y, frames[0], EXPLOSION_LIFETIME_TICKS) != 0)
    return;

  e->anim = create_animSprite(frames, EXPLOSION_ANIM_FRAMES, EXPLOSION_ANIM_SPEED, false);
  if (!e->anim) {
    e->active = false;
    return;
  }

  e->sprite = e->anim->sp;
  e->active = true;
  game->num_explosions++;
}

void(explode_bomb)(Game *game, uint8_t bomb_index) {
  if (game == NULL || bomb_index >= game->num_bombs) {
    fprintf(stderr, "explode_bomb: invalid arguments.");
    return;
  }

  Entity *bomb = &game->bombs[bomb_index];

  if (!bomb->active) {
    fprintf(stderr, "explode_bomb: trying to explode inactive bomb.");
    return;
  }

  int16_t bomb_x = bomb->x;
  int16_t bomb_y = bomb->y;

  const int8_t dx[4] = {0, 1, 0, -1};
  const int8_t dy[4] = {-1, 0, 1, 0};

  for (int dir = 0; dir < 4; dir++) {
    for (int range = 1; range <= 2; range++) {
      int16_t cell_x = bomb_x + dx[dir] * range;
      int16_t cell_y = bomb_y + dy[dir] * range;

      if (cell_x < 0 || cell_x >= game->board.width ||
          cell_y < 0 || cell_y >= game->board.height) {
        break;
      }

      board_element_t cell = game->board.elements[cell_y][cell_x];

      switch (cell) {
        case PLAYER:
          if (game->player.x == cell_x && game->player.y == cell_y) {
            game->player.data--;
            if (game->player.data <= 0) {
              game->player.active = false;
              game->board.elements[cell_y][cell_x] = EMPTY_SPACE;
              game->state = LOSE;
            }
          }
          break;

        case ENEMY:
          for (uint8_t i = 0; i < game->num_enemies; i++) {
            Entity *e = &game->enemies[i];
            if (e->active && e->x == cell_x && e->y == cell_y) {
              e->data--;
              if (e->data <= 0) {
                game->board.elements[e->move.sy][e->move.sx] = EMPTY_SPACE;
                game->board.elements[e->move.ty][e->move.tx] = EMPTY_SPACE;
                e->active = false;
                e->move.moving = false;

                uint8_t alive = 0;
                for (uint8_t j = 0; j < game->num_enemies; j++) {
                  if (game->enemies[j].active) {
                    if (j != alive)
                      game->enemies[alive] = game->enemies[j];
                    alive++;
                  }
                }
                game->num_enemies = alive;
              
              }
              break;
            }
          }
          break;

        case BRICK:
          for (uint8_t i = 0; i < game->num_bricks; i++) {
            if (game->bricks[i].active &&
                game->bricks[i].x == cell_x &&
                game->bricks[i].y == cell_y) {
              game->bricks[i].data--;
              if (game->bricks[i].data <= 0) {
                game->bricks[i].active = false;
                game->score += 10; // Increment score for destroying brick

                // Check if there's a door underneath this brick
                if (game->door.x == cell_x && game->door.y == cell_y) {
                  game->door.active = true;
                  game->board.elements[cell_y][cell_x] = DOOR;
                }
                else {
                  game->board.elements[cell_y][cell_x] = EMPTY_SPACE;
                }
                break;
              }
              game->bricks[i].sprite = get_resources()->brick_sprites[3 - game->bricks[i].data];
              break;
            }
          }
          break;

        case WALL:
          break;

        case BOMB:
          for (uint8_t i = 0; i < game->num_bombs; i++) {
            if (i != bomb_index && game->bombs[i].active &&
                game->bombs[i].x == cell_x &&
                game->bombs[i].y == cell_y) {
              game->bombs[i].data = 1;
              break;
            }
          }
          break;

        case EMPTY_SPACE:
        case POWERUP:
        case DOOR:
          continue;

        default:
          fprintf(stderr, "explode_bomb: unknown board element type %d", cell);
          break;
      }

      if (cell == WALL || cell == BRICK || cell == BOMB) {
        break;
      }
    }
  }

  if (bomb->anim) {
    destroy_animSprite(bomb->anim);
    bomb->anim = NULL;
  }

  bomb->active = false;
  add_explosion(game, bomb_x, bomb_y, (Sprite **) get_resources()->explosion_center_sprites);

  Sprite **arm_frames[4] = {
    (Sprite **) get_resources()->explosion_vert_sprites,
    (Sprite **) get_resources()->explosion_horiz_sprites,
    (Sprite **) get_resources()->explosion_vert_sprites,
    (Sprite **) get_resources()->explosion_horiz_sprites};
  Sprite **hand_frames[4] = {
    (Sprite **) get_resources()->explosion_hand_up_sprites,
    (Sprite **) get_resources()->explosion_hand_right_sprites,
    (Sprite **) get_resources()->explosion_hand_down_sprites,
    (Sprite **) get_resources()->explosion_hand_left_sprites};

  for (int dir = 0; dir < 4; dir++) {
    for (int range = 1; range <= 2; range++) {
      int16_t cell_x = bomb_x + dx[dir] * range;
      int16_t cell_y = bomb_y + dy[dir] * range;

      if (cell_x < 0 || cell_x >= game->board.width || cell_y < 0 || cell_y >= game->board.height)
        break;

      board_element_t cell = game->board.elements[cell_y][cell_x];

      if (cell == WALL) {
        break;
      }

      if (range == 2)
        add_explosion(game, cell_x, cell_y, hand_frames[dir]);
      else
        add_explosion(game, cell_x, cell_y, arm_frames[dir]);

      if (cell == BRICK || cell == BOMB)
        break;
    }
  }

  game->board.elements[bomb_y][bomb_x] = EMPTY_SPACE;

  uint8_t active_bricks = 0;
  for (uint8_t i = 0; i < game->num_bricks; i++) {
    if (game->bricks[i].active) {
      if (i != active_bricks) {
        game->bricks[active_bricks] = game->bricks[i];
      }
      active_bricks++;
    }
  }

  game->num_bricks = active_bricks;
}

void(update_bombs)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "update_bombs: game pointer cannot be null.");
    return;
  }

  uint8_t active_bombs = 0;

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    if (game->bombs[i].active) {
      game->bombs[i].data--;

      if (game->bombs[i].data == 0) {
        explode_bomb(game, i);
      }
      else {
        if (game->bombs[i].anim) {
          update_animSprite(game->bombs[i].anim);
          game->bombs[i].sprite = game->bombs[i].anim->sp;
        }
        if (i != active_bombs) {
          game->bombs[active_bombs] = game->bombs[i];
        }
        active_bombs++;
      }
    }
  }

  game->num_bombs = active_bombs;
}

void(update_explosions)(Game *game) {
  if (!game)
    return;

  uint8_t active = 0;
  for (uint8_t i = 0; i < game->num_explosions; i++) {
    Entity *e = &game->explosions[i];
    if (!e->active)
      continue;

    e->data--;
    if (e->anim) {
      update_animSprite(e->anim);
      e->sprite = e->anim->sp;
    }

    if (e->data == 0) {
      e->active = false;
      if (e->anim) {
        destroy_animSprite(e->anim);
        e->anim = NULL;
      }
      continue;
    }

    if (i != active)
      game->explosions[active] = *e;
    active++;
  }

  game->num_explosions = active;
}
