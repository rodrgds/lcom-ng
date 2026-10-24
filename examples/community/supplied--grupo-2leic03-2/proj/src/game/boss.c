#include "boss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../graphics/graphics.h"
#include "../graphics/sprite.h"
#include "../graphics/font.h"
#include "../graphics/ui.h"

#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "collision.h"
#include "powerup.h"

#include "../assets/boss.xpm"

static Sprite boss_sprite;

void boss_init(void) {
  if (sprite_load(&boss_sprite, boss_xpm) != 0) {
    printf("Error: could not load boss sprite.\n");
  }
}

void boss_reset(Game *game) {
  game->boss.base.active = false;
  game->boss.base.width = BOSS_WIDTH;
  game->boss.base.height = BOSS_HEIGHT;
  game->boss.base.x = graphics_get_width() / 2 - BOSS_WIDTH / 2;
  game->boss.base.y = -BOSS_HEIGHT;
  game->boss.base.vx = BOSS_MOVE_SPEED;
  game->boss.base.vy = BOSS_ENTRY_SPEED;
  game->boss.hp = 0;
  game->boss.max_hp = BOSS_ROUND_5_HP;
  game->boss.shoot_cooldown = BOSS_FIRE_MIN_INTERVAL_FRAMES;
  game->boss.laser_warning_timer = 0;
  game->boss.laser_fire_timer = 0;
  game->boss.laser_cooldown = BOSS_LASER_COOLDOWN_FRAMES;
  game->boss.laser_count = 0;
  game->boss.shield_spawn_timer = 0;
  game->boss.shields_spawned = 0;
  game->boss.shield_spawn_limit = 0;
  game->boss.round = 0;
  game->boss.sprite_scale = BOSS_SPRITE_SCALE;
  game->boss.reward_score = BOSS_ROUND_5_SCORE;
  game->boss.next_level = 6;
  game->boss.entry_y = BOSS_ENTRY_Y;
  game->boss.active = false;
  game->boss.defeated = false;
  game->boss.laser_hit_ship = false;

  for (int i = 0; i < BOSS_MAX_LASERS; i++) {
    game->boss.laser_x[i] = 0;
  }
}

void boss_destroy(void) {
  sprite_destroy(&boss_sprite);
}

static int boss_random_value(const Game *game, int salt, int limit) {
  if (limit <= 0) {
    return 0;
  }

  uint32_t value = (uint32_t) game->frame_counter * 1103515245u;
  value += (uint32_t) game->score * 97u;
  value += (uint32_t) game->boss.base.x * 193u;
  value += (uint32_t) salt * 7919u;
  value ^= value >> 11;
  value *= 2654435761u;

  return (int) (value % (uint32_t) limit);
}

static void boss_set_next_fire_cooldown(Game *game) {
  game->boss.shoot_cooldown = BOSS_FIRE_MIN_INTERVAL_FRAMES +
                              boss_random_value(game, 31, BOSS_FIRE_RANDOM_INTERVAL_FRAMES);
}

static void boss_spawn(Game *game,
                       int round,
                       int width,
                       int height,
                       int hp,
                       int sprite_scale,
                       int reward_score,
                       int next_level,
                       int entry_y) {
  game->boss.base.active = true;
  game->boss.base.width = width;
  game->boss.base.height = height;
  game->boss.base.x = graphics_get_width() / 2 - width / 2;
  game->boss.base.y = -height;
  game->boss.base.vx = BOSS_MOVE_SPEED;
  game->boss.base.vy = BOSS_ENTRY_SPEED;
  game->boss.hp = hp;
  game->boss.max_hp = hp;
  game->boss.shoot_cooldown = BOSS_FIRE_MIN_INTERVAL_FRAMES;
  game->boss.laser_warning_timer = 0;
  game->boss.laser_fire_timer = 0;
  game->boss.laser_cooldown = TARGET_FPS * 6;
  game->boss.laser_count = 0;
  game->boss.shield_spawn_timer = TARGET_FPS * 2;
  game->boss.shields_spawned = 0;
  game->boss.shield_spawn_limit = round == 10 ? 7 + rand() % 2 : 2 + rand() % 2;
  game->boss.round = round;
  game->boss.sprite_scale = sprite_scale;
  game->boss.reward_score = reward_score;
  game->boss.next_level = next_level;
  game->boss.entry_y = entry_y;
  game->boss.active = true;
  game->boss.defeated = false;
  game->boss.laser_hit_ship = false;

  for (int i = 0; i < BOSS_MAX_LASERS; i++) {
    game->boss.laser_x[i] = 0;
  }

  game->boss_message_timer = BOSS_MESSAGE_FRAMES;
}

void boss_spawn_round_5(Game *game) {
  boss_spawn(game,
             5,
             BOSS_WIDTH,
             BOSS_HEIGHT,
             BOSS_ROUND_5_HP,
             BOSS_SPRITE_SCALE,
             BOSS_ROUND_5_SCORE,
             6,
             BOSS_ENTRY_Y);
}

void boss_spawn_round_10(Game *game) {
  boss_spawn(game,
             10,
             BOSS_ROUND_10_WIDTH,
             BOSS_ROUND_10_HEIGHT,
             BOSS_ROUND_10_HP,
             BOSS_ROUND_10_SCALE,
             BOSS_ROUND_10_SCORE,
             11,
             BOSS_ROUND_10_ENTRY_Y);
}

static void boss_spawn_defeat_explosions(Game *game) {
  int center_x = game->boss.base.x + game->boss.base.width / 2;
  int center_y = game->boss.base.y + game->boss.base.height / 2;

  explosion_spawn(game, center_x, center_y);
  explosion_spawn(game, game->boss.base.x + 18, game->boss.base.y + 8);
  explosion_spawn(game, game->boss.base.x + game->boss.base.width - 18, game->boss.base.y + 8);
  explosion_spawn(game, game->boss.base.x + 36, game->boss.base.y + game->boss.base.height - 6);
  explosion_spawn(game, game->boss.base.x + game->boss.base.width - 36, game->boss.base.y + game->boss.base.height - 6);
}

void boss_defeat(Game *game) {
  boss_spawn_defeat_explosions(game);

  game->boss.base.active = false;
  game->boss.active = false;
  game->boss.defeated = true;
  game->boss.hp = 0;
  game->score += game->boss.reward_score;
  game->level = game->boss.next_level;
  game->round_message_timer = TARGET_FPS * 3;
  game->basic_spawn_timer = TARGET_FPS;
  game->zigzag_spawn_timer = TARGET_FPS * 2;
  game->shooter_spawn_timer = TARGET_FPS * 2;
  game->formation_spawn_timer = TARGET_FPS * 4;

  if (game->boss.round == 10) {
    game->round10_boss_defeated = true;
    game->level = 10;
    game->round_message_timer = 0;
    game->win_message_timer = WIN_MESSAGE_FRAMES;
  }
  else {
    game->health_spawn_timer = TARGET_FPS * 5;
    game->shield_spawn_timer = TARGET_FPS * 6;
    game->health_powerup_spawned = false;
    game->shield_powerup_spawned = false;
    game->weapon_powerup_spawn_timer = TARGET_FPS * 5;
    game->weapon_powerup_spawned = false;
    game->weapon_fire_rate_active = false;
    game->weapon_multi_shot_active = false;

    for (int i = 0; i < MAX_POWERUPS; i++) {
      game->powerups[i].base.active = false;
    }
  }

  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    game->enemy_bullets[i].base.active = false;
  }
}

static void boss_start_laser_warning(Game *game) {
  int margin = 80;
  int usable_width = graphics_get_width() - margin * 2;

  game->boss.laser_count = 3 + boss_random_value(game, 47, 2);
  game->boss.laser_warning_timer = BOSS_LASER_WARNING_FRAMES;
  game->boss.laser_fire_timer = 0;
  game->boss.laser_hit_ship = false;

  for (int i = 0; i < game->boss.laser_count; i++) {
    game->boss.laser_x[i] = margin + boss_random_value(game, 101 + i * 37, usable_width);
  }
}

static void boss_damage_ship(Game *game) {
  explosion_spawn(game,
                  game->ship.base.x + game->ship.base.width / 2,
                  game->ship.base.y + game->ship.base.height / 2);

  if (game->shield_timer > 0) {
    return;
  }

  if (game->ship.lives > 0) {
    game->ship.lives--;
    game->weapon_fire_rate_active = false;
    game->weapon_multi_shot_active = false;
  }

  if (game->ship.lives <= 0) {
    game->state = STATE_GAME_OVER;
  }
}

static void boss_check_laser_collisions(Game *game) {
  if (game->boss.laser_fire_timer <= 0 || game->boss.laser_hit_ship) {
    return;
  }

  for (int i = 0; i < game->boss.laser_count; i++) {
    int laser_x = game->boss.laser_x[i] - BOSS_LASER_WIDTH / 2;

    if (!rects_collide(laser_x, 0, BOSS_LASER_WIDTH, graphics_get_height(),
                       game->ship.base.x, game->ship.base.y,
                       game->ship.base.width, game->ship.base.height)) {
      continue;
    }

    game->boss.laser_hit_ship = true;
    boss_damage_ship(game);
    return;
  }
}

static void boss_update_lasers(Game *game) {
  if (game->boss.laser_fire_timer > 0) {
    game->boss.laser_fire_timer--;
    boss_check_laser_collisions(game);

    if (game->boss.laser_fire_timer == 0) {
      game->boss.laser_cooldown = BOSS_LASER_COOLDOWN_FRAMES +
                                  boss_random_value(game, 59, BOSS_LASER_RANDOM_COOLDOWN_FRAMES);
      game->boss.laser_count = 0;
      game->boss.laser_hit_ship = false;
    }

    return;
  }

  if (game->boss.laser_warning_timer > 0) {
    game->boss.laser_warning_timer--;

    if (game->boss.laser_warning_timer == 0) {
      game->boss.laser_fire_timer = BOSS_LASER_FIRE_FRAMES;
      game->boss.laser_hit_ship = false;
    }

    return;
  }

  if (game->boss.laser_cooldown > 0) {
    game->boss.laser_cooldown--;
    return;
  }

  boss_start_laser_warning(game);
}

static void boss_update_shield_spawns(Game *game) {
  if (game->boss.shields_spawned >= game->boss.shield_spawn_limit) {
    return;
  }

  if (game->boss.shield_spawn_timer > 0) {
    game->boss.shield_spawn_timer--;
    return;
  }

  if (powerup_spawn(game, POWERUP_SHIELD)) {
    game->boss.shields_spawned++;
  }

  if (game->boss.round == 10) {
    game->boss.shield_spawn_timer = TARGET_FPS * (2 + rand() % 2);
  }
  else {
    game->boss.shield_spawn_timer = TARGET_FPS * (4 + rand() % 2);
  }
}

static void boss_fire(Game *game) {
  int left_thruster_x = game->boss.base.x + game->boss.base.width / 4;
  int right_thruster_x = game->boss.base.x + (game->boss.base.width * 3) / 4;
  int thruster_y = game->boss.base.y + game->boss.base.height - 12;
  int ship_center_x = game->ship.base.x + game->ship.base.width / 2;
  int ship_center_y = game->ship.base.y + game->ship.base.height / 2;
  int left_offset = boss_random_value(game, 67, 45) - 22;
  int right_offset = boss_random_value(game, 71, 45) - 22;
  int left_target_x = ship_center_x + left_offset;
  int right_target_x = ship_center_x + right_offset;
  int target_y = ship_center_y;
  int pattern = boss_random_value(game, 79, 4);

  if (game->boss.round == 10) {
    enemy_bullet_spawn(game, left_thruster_x, thruster_y, ship_center_x, ship_center_y);
    enemy_bullet_spawn(game, right_thruster_x, thruster_y, ship_center_x, ship_center_y);
    return;
  }

  if (pattern == 1) {
    left_target_x = 80 + boss_random_value(game, 83, graphics_get_width() - 160);
    right_target_x = ship_center_x + right_offset;
    target_y = graphics_get_height() - 60;
  }
  else if (pattern == 2) {
    left_target_x = graphics_get_width() - 120 - boss_random_value(game, 89, 240);
    right_target_x = 120 + boss_random_value(game, 97, 240);
    target_y = graphics_get_height() - 80;
  }
  else if (pattern == 3) {
    left_target_x = 80 + boss_random_value(game, 101, graphics_get_width() - 160);
    right_target_x = 80 + boss_random_value(game, 107, graphics_get_width() - 160);
    target_y = graphics_get_height() - 40 - boss_random_value(game, 109, 180);
  }

  enemy_bullet_spawn(game, left_thruster_x, thruster_y, left_target_x, target_y);
  enemy_bullet_spawn(game, right_thruster_x, thruster_y, right_target_x, target_y);
}

void boss_update(Game *game) {
  if (!game->boss.active) {
    return;
  }

  if (game->boss.base.y < game->boss.entry_y) {
    game->boss.base.y += game->boss.base.vy;

    if (game->boss.base.y >= game->boss.entry_y) {
      game->boss.base.y = game->boss.entry_y;
      game->boss.base.vy = 0;
      boss_set_next_fire_cooldown(game);
    }

    return;
  }

  boss_update_shield_spawns(game);

  game->boss.base.x += game->boss.base.vx;

  if (game->boss.base.x <= 32) {
    game->boss.base.x = 32;
    game->boss.base.vx = BOSS_MOVE_SPEED;
  }
  else if (game->boss.base.x + game->boss.base.width >= graphics_get_width() - 32) {
    game->boss.base.x = graphics_get_width() - 32 - game->boss.base.width;
    game->boss.base.vx = -BOSS_MOVE_SPEED;
  }

  if (game->boss.shoot_cooldown > 0) {
    game->boss.shoot_cooldown--;
  }

  if (game->boss.shoot_cooldown <= 0) {
    boss_fire(game);
    boss_set_next_fire_cooldown(game);
  }

  boss_update_lasers(game);
}

void boss_check_bullet_collisions(Game *game) {
  if (!game->boss.active) {
    return;
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!game->bullets[i].base.active) {
      continue;
    }

    if (!rects_collide(game->bullets[i].base.x, game->bullets[i].base.y,
                       game->bullets[i].base.width, game->bullets[i].base.height,
                       game->boss.base.x, game->boss.base.y,
                       game->boss.base.width, game->boss.base.height)) {
      continue;
    }

    game->bullets[i].base.active = false;
    game->boss.hp--;
    explosion_spawn(game,
                    game->bullets[i].base.x + game->bullets[i].base.width / 2,
                    game->bullets[i].base.y + game->bullets[i].base.height / 2);

    if (game->boss.hp <= 0) {
      boss_defeat(game);
      return;
    }
  }
}

static void boss_draw_hp_bar(const Game *game) {
  if (!game->boss.active || game->boss.max_hp <= 0) {
    return;
  }

  int panel_w = 420;
  int panel_h = 28;
  int panel_x = graphics_get_width() / 2 - panel_w / 2;
  int panel_y = 28;
  int bar_x = panel_x + 88;
  int bar_y = panel_y + 9;
  int bar_w = panel_w - 108;
  int bar_h = 10;
  int fill_w = (bar_w * game->boss.hp) / game->boss.max_hp;

  if (fill_w < 0) {
    fill_w = 0;
  }

  ui_draw_panel(panel_x, panel_y, panel_w, panel_h, COLOR_RED, COLOR_BLACK);
  font_draw_text("BOSS", panel_x + 18, panel_y + 8, COLOR_RED, 2);
  graphics_draw_rectangle_clipped(bar_x, bar_y, bar_w, bar_h, COLOR_WHITE);
  graphics_draw_rectangle_clipped(bar_x + 2, bar_y + 2, bar_w - 4, bar_h - 4, COLOR_BLACK);

  if (fill_w > 4) {
    graphics_draw_rectangle_clipped(bar_x + 2, bar_y + 2, fill_w - 4, bar_h - 4, COLOR_RED);
  }
}

void boss_draw_lasers(const Game *game) {
  if (!game->boss.active) {
    return;
  }

  if (game->boss.laser_warning_timer > 0) {
    for (int i = 0; i < game->boss.laser_count; i++) {
      graphics_draw_rectangle_clipped(game->boss.laser_x[i] - BOSS_LASER_WARNING_WIDTH / 2,
                                      0,
                                      BOSS_LASER_WARNING_WIDTH,
                                      graphics_get_height(),
                                      COLOR_WHITE);
    }
  }

  if (game->boss.laser_fire_timer > 0) {
    for (int i = 0; i < game->boss.laser_count; i++) {
      graphics_draw_rectangle_clipped(game->boss.laser_x[i] - BOSS_LASER_WIDTH / 2,
                                      0,
                                      BOSS_LASER_WIDTH,
                                      graphics_get_height(),
                                      COLOR_RED);
      graphics_draw_rectangle_clipped(game->boss.laser_x[i] - 1,
                                      0,
                                      2,
                                      graphics_get_height(),
                                      COLOR_MAGENTA);
    }
  }
}

void boss_draw(const Game *game) {
  if (!game->boss.active) {
    return;
  }

  if (boss_sprite.loaded) {
    sprite_draw_scaled(&boss_sprite, game->boss.base.x, game->boss.base.y, game->boss.sprite_scale);
  }
  else {
    int x = game->boss.base.x;
    int y = game->boss.base.y;
    int w = game->boss.base.width;
    int h = game->boss.base.height;

    graphics_draw_rectangle_clipped(game->boss.base.x,
                                    game->boss.base.y,
                                    game->boss.base.width,
                                    game->boss.base.height,
                                    COLOR_BLACK);
    graphics_draw_rectangle_clipped(x + 8, y + h / 3, w - 16, h / 3, COLOR_BLUE);
    graphics_draw_rectangle_clipped(x + w / 4, y + 12, w / 2, h - 24, COLOR_BLACK);
    graphics_draw_rectangle_clipped(x + w / 2 - 10, y + 16, 20, h - 32, COLOR_RED);
    graphics_draw_rectangle_clipped(x + 24, y + h - 24, 42, 10, COLOR_RED);
    graphics_draw_rectangle_clipped(x + w - 66, y + h - 24, 42, 10, COLOR_RED);
    graphics_draw_rectangle_clipped(x, y + h / 2 - 4, w, 8, COLOR_RED);
  }

  boss_draw_hp_bar(game);
}

void boss_draw_message(const Game *game) {
  if (game->boss_message_timer <= 0) {
    return;
  }

  font_draw_text_centered("BOSS", graphics_get_width() / 2, graphics_get_height() / 2 - 90, COLOR_RED, 5);
}
