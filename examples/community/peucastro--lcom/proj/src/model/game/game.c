#include <lcom/lcf.h>

#include "model/game/game.h"

int(load_next_level)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "load_next_level: game pointer cannot be null.");
    return 1;
  }

  uint8_t lives = game->player.data;
  game->level++;
  game->level_timer = 120;
  char board_path[128];
  snprintf(board_path, sizeof(board_path), "/home/lcom/labs/proj/src/assets/boards/level%u.txt", game->level);

  if (load_board(&game->board, board_path) != 0) {
    game->state = WIN;
    return 0;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "load_next_level: failed to load resources.");
    return 1;
  }

  game->num_enemies = 0;
  game->num_bricks = 0;
  game->num_walls = 0;
  game->num_bombs = 0;
  game->num_explosions = 0;

  uint8_t ei = 0, bri = 0, wi = 0, boi = 0, pi = 0;
  for (uint8_t r = 0; r < game->board.height; r++) {
    for (uint8_t c = 0; c < game->board.width; c++) {
      board_element_t el = game->board.elements[r][c];
      switch (el) {
        case PLAYER:
          if (init_entity(&game->player, c, r, resources->player_down_sprites[0], lives) != 0) {
            fprintf(stderr, "load_next_level: failed to initialize player entity.");
            return 1;
          }
          game->player.anim = get_player_anim(DOWN);
          game->player.sprite = game->player.anim->sp;
          game->player.active = true;
          break;

        case ENEMY:
          if (ei < MAX_ENEMIES) {
            Entity *en = &game->enemies[ei];
            if (init_entity(en, c, r, resources->enemy_a_sprites[0], 1) != 0) {
              fprintf(stderr, "init_game: failed to initialize enemy entity at index %d.", ei);
              return 1;
            }
            en->anim = create_animSprite(resources->enemy_a_sprites, ENEMY_ANIM_FRAMES, ENEMY_ANIM_SPEED, true);
            ei++;
          }
          break;

        case BRICK:
          if (bri < MAX_BRICKS) {
            if (init_entity(&game->bricks[bri], c, r, resources->brick_sprites[0], 3) != 0) {
              fprintf(stderr, "init_game: failed to initialize brick entity at index %d.", bri);
              return 1;
            }
            bri++;
          }
          break;

        case WALL:
          if (wi < MAX_WALLS) {
            if (init_entity(&game->walls[wi], c, r, resources->wall_sprite, 0) != 0) {
              fprintf(stderr, "init_game: failed to initialize wall entity at index %d.", wi);
              return 1;
            }
            wi++;
          }
          break;

        case BOMB:
          if (boi < MAX_BOMBS) {
            if (init_entity(&game->bombs[boi], c, r, resources->bomb_sprites[0], 0) != 0) {
              fprintf(stderr, "init_game: failed to initialize bomb entity at index %d.", boi);
              return 1;
            }
            boi++;
          }
          break;

        case POWERUP:
          if (pi < 1) {
            if (init_entity(&game->powerup, c, r, resources->powerup_sprite, 0) != 0) {
              fprintf(stderr, "init_game: failed to initialize powerup entity.");
              return 1;
            }
          }
          pi++;
          break;

        case DOOR:
          if (init_entity(&game->door, c, r, resources->door_sprite, 0) != 0) {
            fprintf(stderr, "load_next_level: failed to initialize door entity.");
            return 1;
          }
          game->door.active = true;

          if (bri < MAX_BRICKS) {
            if (init_entity(&game->bricks[bri], c, r, resources->brick_sprites[0], 3) != 0) {
              fprintf(stderr, "load_next_level: failed to initialize brick entity over door.");
              return 1;
            }
            bri++;
            game->board.elements[r][c] = BRICK;
          }
          break;

        case EMPTY_SPACE:
          // nothing to do for empty tiles
          break;

        default:
          fprintf(stderr, "load_next_level: invalid board element.");
          return 1;
      }
    }
  }
  game->num_enemies = ei;
  game->num_bricks = bri;
  game->num_walls = wi;
  game->num_bombs = boi;

  return 0;
}

int(init_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "init_game: game pointer cannot be null.");
    return 1;
  }

  game->state = START;
  game->menu_option = 0;
  game->level = 0;
  game->score = 0;

  game->num_enemies = 0;
  game->num_bricks = 0;
  game->num_walls = 0;
  game->num_bombs = 0;
  game->num_explosions = 0;

  game->player.data = 3;
  game->door_timer = 0;

  if (load_next_level(game) != 0) {
    fprintf(stderr, "init_game: failed to load next level.");
    return 1;
  }

  return 0;
}

int(reset_game)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "reset_game: game pointer cannot be null.");
    return 1;
  }

  game->score = 0;

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    reset_entity(&game->enemies[i]);
  }
  game->num_enemies = 0;

  for (uint8_t i = 0; i < game->num_bricks; i++) {
    reset_entity(&game->bricks[i]);
  }
  game->num_bricks = 0;

  for (uint8_t i = 0; i < game->num_walls; i++) {
    reset_entity(&game->walls[i]);
  }
  game->num_walls = 0;

  for (uint8_t i = 0; i < game->num_bombs; i++) {
    reset_entity(&game->bombs[i]);
  }
  game->num_bombs = 0;

  reset_entity(&game->powerup);
  reset_entity(&game->door);
  game->num_bombs = 0;
  game->door_timer = 0;

  if (reset_board(&game->board) != 0) {
    fprintf(stderr, "reset_game: failed to reset game board.");
  }

  return 0;
}

void(update_door_timer)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "update_door_timer: game pointer cannot be null.");
    return;
  }

  if (game->num_enemies == 0 && game->door.active) {
    if (game->player.active &&
        game->player.x == game->door.x &&
        game->player.y == game->door.y) {

      game->door_timer++;

      if (game->door_timer >= 3) {
        game->door_timer = 0;
        if (load_next_level(game) != 0) {
          game->state = WIN;
        }
      }
    }
    else {
      game->door_timer = 0;
    }
  }
  else {
    game->door_timer = 0;
  }
}

void(update_level_timer)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "update_level_timer: game pointer cannot be null.");
    return;
  }

  if (game->state == GAME && game->level_timer > 0) {
    game->level_timer--;
    if (game->level_timer == 0) {
      game->state = LOSE;
    }
  }
}
