#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../graphics/graphics.h"
#include "../graphics/sprite.h"
#include "../graphics/font.h"
#include "../graphics/ui.h"
#include "../devices/rtc.h"

#include "ship.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "collision.h"
#include "state.h"
#include "powerup.h"

#include "boss.h"
#include "background.h"

#include "../assets/player.xpm"
#include "../assets/enemy.xpm"
#include "../assets/zigzag_enemy.xpm"
#include "../assets/formation_enemy.xpm"
#include "../assets/shooter_enemy.xpm"
#include "../assets/bullet.xpm"
#include "../assets/enemy_bullet.xpm"
#include "../assets/explosion.xpm"
#include "../assets/cursor.xpm"
#include "../assets/health_powerup.xpm"
#include "../assets/shield_powerup.xpm"
#include "../assets/fire_rate_powerup.xpm"
#include "../assets/multi_shot_powerup.xpm"

#define PLAYER_SPRITE_SCALE 2
#define PLAYER_DRAW_WIDTH  (PLAYER_WIDTH * PLAYER_SPRITE_SCALE)
#define PLAYER_DRAW_HEIGHT (PLAYER_HEIGHT * PLAYER_SPRITE_SCALE)

Sprite player_sprite;
Sprite enemy_sprite;
Sprite zigzag_enemy_sprite;
Sprite formation_enemy_sprite;
Sprite shooter_enemy_sprite;
Sprite bullet_sprite;
Sprite enemy_bullet_sprite;
Sprite explosion_sprite;
Sprite cursor_sprite;
Sprite health_powerup_sprite;
Sprite shield_powerup_sprite;
Sprite fire_rate_powerup_sprite;
Sprite multi_shot_powerup_sprite;

static bool sprites_loaded = false;

static void update_rtc_strings(Game *game) {
  RTC_Time t = rtc_get_time();
  snprintf(game->rtc_time_str, sizeof(game->rtc_time_str), "%02d:%02d:%02d", t.hours, t.minutes, t.seconds);
  snprintf(game->rtc_date_str, sizeof(game->rtc_date_str), "%02d/%02d/20%02d", t.day, t.month, t.year);
}

static void game_reset_round_powerups(Game *game) {
  game->weapon_powerup_spawned = false;
  game->weapon_powerup_spawn_timer = TARGET_FPS * 5;
  game->weapon_fire_rate_active = false;
  game->weapon_multi_shot_active = false;
  game->health_powerup_spawned = false;
  game->health_spawn_timer = TARGET_FPS * 5;
  game->shield_powerup_spawned = false;
  game->shield_spawn_timer = TARGET_FPS * 6;
}

void game_reset_play_state(Game *game) {
  game->ship.base.width = PLAYER_DRAW_WIDTH;
  game->ship.base.height = PLAYER_DRAW_HEIGHT;
  game->ship.base.x = SCREEN_WIDTH / 2 - game->ship.base.width / 2;
  game->ship.base.y = SCREEN_HEIGHT - 120;
  game->ship.speed = 7;
  game->ship.lives = 3;

  boss_reset(game);

  game->score = 0;
  game->level = 1;
  game->round_message_timer = TARGET_FPS * 3;
  game->basic_spawn_timer = TARGET_FPS;
  game->zigzag_spawn_timer = TARGET_FPS * 2;
  game->formation_spawn_timer = TARGET_FPS * 3;
  game->shooter_spawn_timer = TARGET_FPS * 4;
  game->health_spawn_timer = TARGET_FPS * 5; 
  game->shield_spawn_timer = TARGET_FPS * 6; 
  game->shield_timer = 0;
  game->health_powerup_spawned = false;
  game->shield_powerup_spawned = false;
  game_reset_round_powerups(game);
  game->frame_counter = 0;
  game->shoot_cooldown = 0;
  game->asteroids_destroyed = 0;
  game->boss_message_timer = 0;
  game->win_message_timer = 0;
  game->round10_boss_defeated = false;

  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    game->asteroids[i].base.active = false;
    game->asteroids[i].type = ENEMY_BASIC;
    game->asteroids[i].age = 0;
    game->asteroids[i].score_value = SCORE_PER_KILL;
    game->asteroids[i].hold_y = 0;
    game->asteroids[i].shoot_cooldown = 0;
    game->asteroids[i].costs_life = true;
    game->asteroids[i].formation_released = false;
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    game->bullets[i].base.active = false;
  }

  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    game->enemy_bullets[i].base.active = false;
  }

  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    game->explosions[i].base.active = false;
    game->explosions[i].age = 0;
  }

  for (int i = 0; i < MAX_POWERUPS; i++) {
    game->powerups[i].base.active = false;
  }

  for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
    game->floating_texts[i].active = false;
  }
}

static void load_sprites(void) {
  if (sprites_loaded) return;

  if (sprite_load(&player_sprite, player_xpm) != 0) printf("Error: could not load player sprite.\n");

  if (sprite_load(&enemy_sprite, enemy_xpm) != 0) printf("Error: could not load enemy sprite.\n");

  if (sprite_load(&zigzag_enemy_sprite, zigzag_enemy_xpm) != 0) printf("Error: could not load zigzag enemy sprite.\n");

  if (sprite_load(&formation_enemy_sprite, formation_enemy_xpm) != 0) printf("Error: could not load formation enemy sprite.\n");

  if (sprite_load(&shooter_enemy_sprite, shooter_enemy_xpm) != 0) printf("Error: could not load shooter enemy sprite.\n");

  if (sprite_load(&bullet_sprite, bullet_xpm) != 0) printf("Error: could not load bullet sprite.\n");

  if (sprite_load(&enemy_bullet_sprite, enemy_bullet_xpm) != 0) printf("Error: could not load enemy bullet sprite.\n");

  if (sprite_load(&explosion_sprite, explosion_xpm) != 0) printf("Error: could not load explosion sprite.\n");

  if (sprite_load(&cursor_sprite, cursor_xpm) != 0) printf("Error: could not load cursor sprite.\n");

  if (sprite_load(&health_powerup_sprite, health_powerup_xpm) != 0) printf("Error: could not load health powerup sprite.\n");

  if (sprite_load(&shield_powerup_sprite, shield_powerup_xpm) != 0) printf("Error: could not load shield powerup sprite.\n");

  if (sprite_load(&fire_rate_powerup_sprite, fire_rate_powerup_xpm) != 0) printf("Error: could not load fire rate powerup sprite.\n");

  if (sprite_load(&multi_shot_powerup_sprite, multi_shot_powerup_xpm) != 0) printf("Error: could not load multi shot powerup sprite.\n");

  sprites_loaded = true;
}

void game_init(Game *game) {
  game->state = STATE_MENU;
  game->score = 0;
  game->frame_counter = 0;
  game->shoot_cooldown = 0;
  game->asteroids_destroyed = 0;

  snprintf(game->rtc_time_str, sizeof(game->rtc_time_str), "00:00:00");
  snprintf(game->rtc_date_str, sizeof(game->rtc_date_str), "01/01/2026");

  game_reset_play_state(game);
  load_sprites();
  background_init();
  boss_init();
  state_init();
}

void game_destroy(void) {
  sprite_destroy(&player_sprite);
  sprite_destroy(&enemy_sprite);
  sprite_destroy(&zigzag_enemy_sprite);
  sprite_destroy(&formation_enemy_sprite);
  sprite_destroy(&shooter_enemy_sprite);
  sprite_destroy(&bullet_sprite);
  sprite_destroy(&enemy_bullet_sprite);
  sprite_destroy(&explosion_sprite);
  sprite_destroy(&cursor_sprite);
  sprite_destroy(&health_powerup_sprite);
  sprite_destroy(&shield_powerup_sprite);
  sprite_destroy(&fire_rate_powerup_sprite);
  sprite_destroy(&multi_shot_powerup_sprite);

  background_destroy();
  boss_destroy();

  sprites_loaded = false;
}

static void game_draw_win_message(const Game *game) {
  if (game->win_message_timer <= 0) {
    return;
  }

  ui_draw_text_centered("YOU WON", 0, SCREEN_HEIGHT / 2 - 45, SCREEN_WIDTH, 90, COLOR_WHITE, 5);
}

void game_update(Game *game, const InputState *input) {
  update_rtc_strings(game);
  
  state_update(game, input);
  if (game->state != STATE_PLAYING) {
    return;
  }

  game->frame_counter++;

  if (!game->boss.active && game->frame_counter % SCORE_TIME_INTERVAL_FRAMES == 0) {
    game->score++;
  }

  ship_update(&game->ship, input);

  if (game->shoot_cooldown > 0) {
    game->shoot_cooldown--;
  }

  if (input->shoot && game->shoot_cooldown == 0) {
    bullet_spawn(game);
    game->shoot_cooldown = game->weapon_fire_rate_active ? (SHOOT_COOLDOWN_FRAMES / 2) : SHOOT_COOLDOWN_FRAMES;
  }

  int target_level = (game->score / 5000) + 1;
  if (target_level > game->level) {
    if (game->level == 10 && game->round10_boss_defeated) {
      target_level = 10;
    }
    else if (game->level == 5 && target_level >= 6 && !game->boss.defeated) {
      if (!game->boss.active) {
        boss_spawn_round_5(game);
      }
    }
    else if (game->level == 10 &&
             game->score >= BOSS_ROUND_10_ENTRY_SCORE &&
             !game->round10_boss_defeated) {
      if (!game->boss.active) {
        boss_spawn_round_10(game);
      }
    }
    else {
      game->level = target_level;
      game->round_message_timer = TARGET_FPS * 3;
      game_reset_round_powerups(game);
    }
  }

  if (game->round_message_timer > 0) game->round_message_timer--;
  if (game->boss_message_timer > 0) game->boss_message_timer--;
  if (game->win_message_timer > 0) game->win_message_timer--;

  bool formation_holding = asteroid_has_holding_formation(game);
  bool boss_active = game->boss.active;
  bool stop_formations = game->level == 5 &&
                         game->score >= BOSS_ROUND_5_FORMATION_STOP_SCORE &&
                         !game->boss.defeated;

  if (game->level >= 1 && !formation_holding && !boss_active) {
    if (game->basic_spawn_timer > 0) game->basic_spawn_timer--;
    if (game->basic_spawn_timer <= 0) {
      asteroid_spawn(game, ENEMY_BASIC);

      // The spawn interval starts at 120 frames (2 seconds) at level 1.
      // Decreases by 2 frames per level, down to a minimum of 50 frames.
      int interval = 120 - (game->level - 1) * 2;
      game->basic_spawn_timer = interval < 50 ? 50 : interval;
    }
  }

  if (game->level >= 2 && !formation_holding && !boss_active) {
    if (game->zigzag_spawn_timer > 0) game->zigzag_spawn_timer--;
    if (game->zigzag_spawn_timer <= 0) {
      asteroid_spawn(game, ENEMY_ZIGZAG);

      // The spawn interval starts at 150 frames (2.5 seconds) at level 2.
      // Decreases by 3 frames per level, down to a minimum of 60 frames.
      int interval = 150 - (game->level - 2) * 3;
      game->zigzag_spawn_timer = interval < 60 ? 60 : interval;
    }
  }

  if (game->level >= 6 && !formation_holding && !boss_active) {
    if (game->shooter_spawn_timer > 0) game->shooter_spawn_timer--;
    if (game->shooter_spawn_timer <= 0) {
      asteroid_spawn(game, ENEMY_SHOOTER);

      // Shooter enemies start appearing in round 6 and become slightly more frequent.
      int interval = 180 - (game->level - 6) * 8;
      game->shooter_spawn_timer = interval < 90 ? 90 : interval;
    }
  }

  if (game->level >= 3 && game->level != 6 && game->level != 7 && !boss_active && !stop_formations) {
    if (game->formation_spawn_timer > 0) game->formation_spawn_timer--;
    if (game->formation_spawn_timer <= 0) {
      asteroid_spawn(game, ENEMY_FORMATION);

      // The spawn interval starts at 1200 frames (20 seconds) at level 3.
      // Decreases by 15 frames (0.25 seconds) per level, down to a minimum of 800 frames.
      int interval = 1200 - (game->level - 3) * 15;
      game->formation_spawn_timer = interval < 800 ? 800 : interval;
    }

  }

  if (!boss_active && game->ship.lives <= 2 && !game->health_powerup_spawned) {
    if (game->health_spawn_timer > 0) game->health_spawn_timer--;
    if (game->health_spawn_timer <= 0) {
      if (powerup_spawn(game, POWERUP_HEALTH)) {
        game->health_powerup_spawned = true;
      }
    }
  }

  if (game->level >= 4 && !boss_active && !game->shield_powerup_spawned) {
    if (game->shield_spawn_timer > 0) game->shield_spawn_timer--;
    if (game->shield_spawn_timer <= 0) {
      if (powerup_spawn(game, POWERUP_SHIELD)) {
        game->shield_powerup_spawned = true;
      }
    }
  }

  if (game->level >= 5 && !boss_active && !game->weapon_powerup_spawned) {
    if (game->weapon_powerup_spawn_timer > 0) game->weapon_powerup_spawn_timer--;
    if (game->weapon_powerup_spawn_timer <= 0) {
      PowerupType pt = (rand() % 2 == 0) ? POWERUP_FIRE_RATE : POWERUP_MULTI_SHOT;
      if (powerup_spawn(game, pt)) {
        game->weapon_powerup_spawned = true;
      }
    }
  }

  if (game->shield_timer > 0) {
    game->shield_timer--;
  }

  for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
    if (game->floating_texts[i].active) {
      game->floating_texts[i].age++;
      game->floating_texts[i].y--;
      if (game->floating_texts[i].age > TARGET_FPS * 2) { // 2 seconds
        game->floating_texts[i].active = false;
      }
    }
  }

  bullet_update_all(game);
  asteroid_update_all(game);
  powerup_update_all(game);
  boss_update(game);
  boss_check_bullet_collisions(game);
  check_collisions(game);
  explosion_update_all(game);
}

static void game_draw_playing_world(const Game *game) {
  background_draw(game);
  bullet_draw_all(game, &bullet_sprite);
  enemy_bullet_draw_all(game, &enemy_bullet_sprite);
  asteroid_draw_all(game,
                    &enemy_sprite,
                    &zigzag_enemy_sprite,
                    &formation_enemy_sprite,
                    &shooter_enemy_sprite);

  powerup_draw_all(game, &health_powerup_sprite, &shield_powerup_sprite, &fire_rate_powerup_sprite, &multi_shot_powerup_sprite);
  boss_draw_lasers(game);
  boss_draw(game);
  explosion_draw_all(game, &explosion_sprite);
  ship_draw(&game->ship, &player_sprite, game->shield_timer > 0);
  
  for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
    if (game->floating_texts[i].active) {
      font_draw_text(game->floating_texts[i].text, game->floating_texts[i].x, game->floating_texts[i].y, game->floating_texts[i].color, 1);
    }
  }
  state_draw_playing_hud(game);
  boss_draw_message(game);
  game_draw_win_message(game);
}

void game_draw(Game *game, const InputState *input) {
  if (game->state == STATE_MENU || game->state == STATE_GAME_OVER) {
    state_draw(game, input);
  }
  else if (game->state == STATE_PLAYING) {
    game_draw_playing_world(game);
  }
  else if (game->state == STATE_PAUSED) {
    game_draw_playing_world(game);
    state_draw(game, input);
  }
  else {
    graphics_clear_screen(COLOR_BLACK);
  }
  graphics_swap_buffers();
}

