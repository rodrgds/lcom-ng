#include "asteroid.h"
#include "bullet.h"
#include "../config.h"
#include "../graphics/graphics.h"
#include <stddef.h>

#define ASTEROID_SPAWN_COLUMNS 8
#define ASTEROID_SPAWN_MARGIN 48
#define ASTEROID_SAFE_SPAWN_DISTANCE 90
#define ASTEROID_SAFE_SPAWN_FRAMES (TARGET_FPS * 3)
#define ASTEROID_FORMATION_COLUMNS 8
#define ASTEROID_FORMATION_ROWS 3
#define ASTEROID_FORMATION_COUNT (ASTEROID_FORMATION_COLUMNS * ASTEROID_FORMATION_ROWS)
#define ASTEROID_FORMATION_Y_GAP 4
#define ASTEROID_FORMATION_START_Y 210
#define ASTEROID_ZIGZAG_SPEED 2
#define ASTEROID_ZIGZAG_CHANGE_MIN_FRAMES 18
#define ASTEROID_ZIGZAG_CHANGE_RANGE_FRAMES 28
#define ASTEROID_ZIGZAG_LEFT_LIMIT 24
#define ASTEROID_ZIGZAG_RIGHT_LIMIT (SCREEN_WIDTH - ASTEROID_SIZE - 24)
#define SHOOTER_FIRE_INTERVAL_FRAMES 45

static int asteroid_abs(int value) {
  return value < 0 ? -value : value;
}

static int asteroid_spawn_x_from_slot(int slot) {
  int usable_width = SCREEN_WIDTH - 2 * ASTEROID_SPAWN_MARGIN - ASTEROID_SIZE;
  int step = usable_width / (ASTEROID_SPAWN_COLUMNS - 1);

  return ASTEROID_SPAWN_MARGIN + slot * step;
}

static int asteroid_current_speed(const Game *game) {
  int level = game->asteroids_destroyed / ASTEROID_SPEEDUP_KILLS;
  int speed = ASTEROID_SPEED + level;

  if (speed > ASTEROID_MAX_SPEED) {
    speed = ASTEROID_MAX_SPEED;
  }

  return speed;
}

static int asteroid_score_for_type(EnemyType type, bool formation_member) {
  if (formation_member) {
    return SCORE_PER_FORMATION_KILL;
  }

  return SCORE_PER_KILL;
}

static void asteroid_setup(Asteroid *asteroid,
                           int x,
                           int y,
                           EnemyType type,
                           int speed,
                           bool costs_life,
                           bool formation_member) {
  asteroid->base.active = true;
  asteroid->base.width = ASTEROID_SIZE;
  asteroid->base.height = ASTEROID_SIZE;
  asteroid->base.x = x;
  asteroid->base.y = y;
  asteroid->base.vx = 0;
  asteroid->base.vy = speed;
  asteroid->type = type;
  asteroid->age = 0;
  asteroid->score_value = asteroid_score_for_type(type, formation_member);
  asteroid->hold_y = y;
  asteroid->shoot_cooldown = SHOOTER_FIRE_INTERVAL_FRAMES;
  asteroid->costs_life = costs_life;
  asteroid->formation_released = false;

  if (type == ENEMY_ZIGZAG) {
    asteroid->base.vx = ASTEROID_ZIGZAG_SPEED;
  }
}

void asteroid_spawn(Game *game, EnemyType type) {
  if (type == ENEMY_FORMATION) {
    int usable_width = SCREEN_WIDTH - 2 * ASTEROID_SPAWN_MARGIN - ASTEROID_SIZE;
    int step = usable_width / (ASTEROID_FORMATION_COLUMNS - 1);
    int start_y = -(ASTEROID_FORMATION_ROWS * (ASTEROID_SIZE + ASTEROID_FORMATION_Y_GAP));
    int spawned = 0;

    for (int row = 0; row < ASTEROID_FORMATION_ROWS; row++) {
      for (int col = 0; col < ASTEROID_FORMATION_COLUMNS; col++) {
        while (spawned < MAX_ASTEROIDS && game->asteroids[spawned].base.active) {
          spawned++;
        }
        if (spawned >= MAX_ASTEROIDS) return;

        int hold_y = ASTEROID_FORMATION_START_Y + row * (ASTEROID_SIZE + ASTEROID_FORMATION_Y_GAP);

        asteroid_setup(&game->asteroids[spawned],
                      ASTEROID_SPAWN_MARGIN + col * step,
                      start_y + row * (ASTEROID_SIZE + ASTEROID_FORMATION_Y_GAP),
                      ENEMY_FORMATION,
                      ASTEROID_SPEED,
                      false,
                      true);
        game->asteroids[spawned].hold_y = hold_y;
          spawned++;
      }
    }
    return;
  }

  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (!game->asteroids[i].base.active) {
      int slot = (game->frame_counter * 37 + game->asteroids_destroyed * 13 + game->score * 3) % ASTEROID_SPAWN_COLUMNS;
      int asteroid_x = asteroid_spawn_x_from_slot(slot);
      int asteroid_center = asteroid_x + ASTEROID_SIZE / 2;
      int ship_center = game->ship.base.x + game->ship.base.width / 2;
      int speed = asteroid_current_speed(game);

      if (game->frame_counter < ASTEROID_SAFE_SPAWN_FRAMES &&
          asteroid_abs(asteroid_center - ship_center) < ASTEROID_SAFE_SPAWN_DISTANCE) {
        slot = (slot + 3) % ASTEROID_SPAWN_COLUMNS;
        asteroid_x = asteroid_spawn_x_from_slot(slot);
      }

      asteroid_setup(&game->asteroids[i],
                     asteroid_x,
                     -ASTEROID_SIZE,
                     type,
                     speed,
                     true,
                     false);
      return;
    }
  }
}

static int asteroid_random_direction(const Game *game, int index) {
  int value = (game->frame_counter * 17 + index * 31 + game->asteroids_destroyed * 7) % 5;

  if (value == 0) {
    return -ASTEROID_ZIGZAG_SPEED;
  }

  if (value == 1) {
    return ASTEROID_ZIGZAG_SPEED;
  }

  if (value == 2) {
    return 0;
  }

  return game->asteroids[index].base.vx;
}

static bool asteroid_should_change_zigzag_direction(const Game *game, int index) {
  int interval = ASTEROID_ZIGZAG_CHANGE_MIN_FRAMES +
                 ((index * 11 + game->asteroids_destroyed * 3) % ASTEROID_ZIGZAG_CHANGE_RANGE_FRAMES);

  return interval > 0 && game->asteroids[index].age % interval == 0;
}

void asteroid_update_all(Game *game) {
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (!game->asteroids[i].base.active) {
      continue;
    }

    game->asteroids[i].age++;

    if (game->asteroids[i].type == ENEMY_ZIGZAG) {
      if (asteroid_should_change_zigzag_direction(game, i)) {
        game->asteroids[i].base.vx = asteroid_random_direction(game, i);
      }

      if (game->asteroids[i].base.x <= ASTEROID_ZIGZAG_LEFT_LIMIT) {
        game->asteroids[i].base.vx = ASTEROID_ZIGZAG_SPEED;
      }
      else if (game->asteroids[i].base.x >= ASTEROID_ZIGZAG_RIGHT_LIMIT) {
        game->asteroids[i].base.vx = -ASTEROID_ZIGZAG_SPEED;
      }
    }
    else if (game->asteroids[i].type == ENEMY_FORMATION &&
             !game->asteroids[i].formation_released &&  
             game->asteroids[i].base.vy == 0) {
      // Formation hold time (pause) in the center of the screen.
      // Starts at the base value (ASTEROID_FORMATION_HOLD_FRAMES, 6 sec) at level 3.
      // Decreases by 10 frames (approx. 1/6 of a second) for each level above 3.
      // Has a fixed minimum of 3 seconds (TARGET_FPS * 3).        
      int hold_time = ASTEROID_FORMATION_HOLD_FRAMES - (game->level - 3) * 10;
      if (hold_time < TARGET_FPS * 3) {
        hold_time = TARGET_FPS * 3;
      }
      if (game->asteroids[i].age >= hold_time) {
        game->asteroids[i].base.vy = ASTEROID_SPEED;
        game->asteroids[i].formation_released = true;
      }
    }
    else if (game->asteroids[i].type == ENEMY_SHOOTER) {
      if (game->asteroids[i].shoot_cooldown > 0) {
        game->asteroids[i].shoot_cooldown--;
      }

      if (game->asteroids[i].shoot_cooldown <= 0 &&
          game->asteroids[i].base.y > 0 &&
          game->asteroids[i].base.y < SCREEN_HEIGHT - game->asteroids[i].base.height) {
        int ship_center_x = game->ship.base.x + game->ship.base.width / 2;
        int ship_center_y = game->ship.base.y + game->ship.base.height / 2;
        int enemy_y = game->asteroids[i].base.y + game->asteroids[i].base.height;

        enemy_bullet_spawn(game,
                           game->asteroids[i].base.x + 10,
                           enemy_y,
                           ship_center_x,
                           ship_center_y);
        enemy_bullet_spawn(game,
                           game->asteroids[i].base.x + game->asteroids[i].base.width - 10,
                           enemy_y,
                           ship_center_x,
                           ship_center_y);
        game->asteroids[i].shoot_cooldown = SHOOTER_FIRE_INTERVAL_FRAMES;
      }
    }

    game->asteroids[i].base.x += game->asteroids[i].base.vx;
    game->asteroids[i].base.y += game->asteroids[i].base.vy;

    if (game->asteroids[i].type == ENEMY_FORMATION &&
        !game->asteroids[i].formation_released &&
        game->asteroids[i].base.vy > 0 &&
        game->asteroids[i].base.y >= game->asteroids[i].hold_y) {
      game->asteroids[i].base.y = game->asteroids[i].hold_y;
      game->asteroids[i].base.vy = 0;
      game->asteroids[i].age = 0;
    }

    if (game->asteroids[i].base.y > SCREEN_HEIGHT) {
      game->asteroids[i].base.active = false;

      if (game->score > SCORE_LOST_PER_MISSED_ENEMY) {
        game->score -= SCORE_LOST_PER_MISSED_ENEMY;
      }
      else {
        game->score = 0;
      }
    }
  }
}

bool asteroid_has_holding_formation(const Game *game) {
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (game->asteroids[i].base.active &&
        game->asteroids[i].type == ENEMY_FORMATION &&
        !game->asteroids[i].formation_released &&
        game->asteroids[i].base.vy == 0) {
      return true;
    }
  }

  return false;
}

static uint32_t asteroid_fallback_color(EnemyType type) {
  if (type == ENEMY_ZIGZAG) {
    return COLOR_RED;
  }

  if (type == ENEMY_FORMATION) {
    return COLOR_BROWN;
  }

  if (type == ENEMY_SHOOTER) {
    return COLOR_CYAN;
  }

  return COLOR_GREEN;
}

static void asteroid_draw_shooter_aim(const Game *game, const Asteroid *asteroid) {
  int ship_center_x = game->ship.base.x + game->ship.base.width / 2;
  int enemy_center_x = asteroid->base.x + asteroid->base.width / 2;
  int enemy_y = asteroid->base.y + asteroid->base.height - 8;

  if (ship_center_x < enemy_center_x - 12) {
    graphics_draw_rectangle_clipped(asteroid->base.x + 10, enemy_y, 10, 3, COLOR_BROWN);
    graphics_draw_rectangle_clipped(asteroid->base.x + 8, enemy_y + 3, 6, 3, COLOR_BROWN);
  }
  else if (ship_center_x > enemy_center_x + 12) {
    graphics_draw_rectangle_clipped(asteroid->base.x + asteroid->base.width - 20, enemy_y, 10, 3, COLOR_BROWN);
    graphics_draw_rectangle_clipped(asteroid->base.x + asteroid->base.width - 14, enemy_y + 3, 6, 3, COLOR_BROWN);
  }
  else {
    graphics_draw_rectangle_clipped(enemy_center_x - 4, enemy_y, 8, 8, COLOR_BROWN);
  }
}

static const Sprite *asteroid_sprite_for_type(EnemyType type,
                                              const Sprite *basic_sprite,
                                              const Sprite *zigzag_sprite,
                                              const Sprite *formation_sprite,
                                              const Sprite *shooter_sprite) {
  if (type == ENEMY_ZIGZAG) {
    return zigzag_sprite;
  }

  if (type == ENEMY_FORMATION) {
    return formation_sprite;
  }

  if (type == ENEMY_SHOOTER) {
    return shooter_sprite;
  }

  return basic_sprite;
}

void asteroid_draw_all(const Game *game,
                       const Sprite *basic_sprite,
                       const Sprite *zigzag_sprite,
                       const Sprite *formation_sprite,
                       const Sprite *shooter_sprite) {
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (!game->asteroids[i].base.active) {
      continue;
    }

    const Sprite *sprite = asteroid_sprite_for_type(game->asteroids[i].type,
                                                    basic_sprite,
                                                    zigzag_sprite,
                                                    formation_sprite,
                                                    shooter_sprite);

    if (sprite != NULL && sprite->loaded) {
      sprite_draw_scaled(sprite, game->asteroids[i].base.x, game->asteroids[i].base.y, 1);
      if (game->asteroids[i].type == ENEMY_SHOOTER) {
        asteroid_draw_shooter_aim(game, &game->asteroids[i]);
      }
    } else {
      graphics_draw_rectangle_clipped(game->asteroids[i].base.x,
                                      game->asteroids[i].base.y,
                                      game->asteroids[i].base.width,
                                      game->asteroids[i].base.height,
                                      asteroid_fallback_color(game->asteroids[i].type));
    }
  }
}
