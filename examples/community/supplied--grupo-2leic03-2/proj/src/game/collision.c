#include "collision.h"
#include "explosion.h"
#include <string.h>
#include "../graphics/graphics.h"

static void spawn_floating_text(Game *game, int x, int y, const char* text, uint32_t color) {
  for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
    if (!game->floating_texts[i].active) {
      game->floating_texts[i].active = true;
      game->floating_texts[i].x = x;
      game->floating_texts[i].y = y;
      game->floating_texts[i].age = 0;
      game->floating_texts[i].color = color;
      strncpy(game->floating_texts[i].text, text, sizeof(game->floating_texts[i].text) - 1);
      game->floating_texts[i].text[sizeof(game->floating_texts[i].text) - 1] = '\0';
      break;
    }
  }
}

bool rects_collide(int ax, int ay, int aw, int ah,
                   int bx, int by, int bw, int bh) {
  return ax < bx + bw &&
         ax + aw > bx &&
         ay < by + bh &&
         ay + ah > by;
}

void check_collisions(Game *game) {
  // bullets vs asteroids
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!game->bullets[i].base.active) {
      continue;
    }

    for (int j = 0; j < MAX_ASTEROIDS; j++) {
      if (!game->asteroids[j].base.active) {
        continue;
      }
      if (rects_collide(game->bullets[i].base.x, game->bullets[i].base.y,
                        game->bullets[i].base.width, game->bullets[i].base.height,
                        game->asteroids[j].base.x, game->asteroids[j].base.y,
                        game->asteroids[j].base.width, game->asteroids[j].base.height)) {
        
        explosion_spawn(game,
                        game->asteroids[j].base.x + game->asteroids[j].base.width / 2,
                        game->asteroids[j].base.y + game->asteroids[j].base.height / 2);
        game->bullets[i].base.active = false;
        game->asteroids[j].base.active = false;
        game->score += game->asteroids[j].score_value;
        game->asteroids_destroyed++;
        break;
      }
    }
  }

  // enemy bullets vs ship
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (!game->enemy_bullets[i].base.active) {
      continue;
    }

    int ship_x = game->ship.base.x;
    int ship_y = game->ship.base.y;
    int ship_w = game->ship.base.width;
    int ship_h = game->ship.base.height;

    if (game->shield_timer > 0) {
      ship_x -= 5;
      ship_y -= 5;
      ship_w += 10;
      ship_h += 10;
    }

    if (rects_collide(game->enemy_bullets[i].base.x, game->enemy_bullets[i].base.y,
                      game->enemy_bullets[i].base.width, game->enemy_bullets[i].base.height,
                      ship_x, ship_y, ship_w, ship_h)) {

      explosion_spawn(game,
                      game->enemy_bullets[i].base.x + game->enemy_bullets[i].base.width / 2,
                      game->enemy_bullets[i].base.y + game->enemy_bullets[i].base.height / 2);
      game->enemy_bullets[i].base.active = false;

      if (game->shield_timer > 0) {
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "DEFLECTED!", COLOR_CYAN);
      } else {
        if (game->ship.lives > 0) {
          game->ship.lives--;
          game->weapon_fire_rate_active = false;
          game->weapon_multi_shot_active = false;
        }
        if (game->ship.lives <= 0) {
          game->state = STATE_GAME_OVER;
        }
      }
      break;
    }
  }

  // ship vs asteroids
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (!game->asteroids[i].base.active) {
      continue;
    }

    int ship_x = game->ship.base.x;
    int ship_y = game->ship.base.y;
    int ship_w = game->ship.base.width;
    int ship_h = game->ship.base.height;

    if (game->shield_timer > 0) {
      ship_x -= 5;
      ship_y -= 5;
      ship_w += 10;
      ship_h += 10;
    }

    if (rects_collide(ship_x, ship_y, ship_w, ship_h,
                      game->asteroids[i].base.x, game->asteroids[i].base.y,
                      game->asteroids[i].base.width, game->asteroids[i].base.height)) {
      
      explosion_spawn(game,
                      game->asteroids[i].base.x + game->asteroids[i].base.width / 2,
                      game->asteroids[i].base.y + game->asteroids[i].base.height / 2);
      game->asteroids[i].base.active = false;
      if (game->shield_timer > 0) {
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "BLOCKED!", COLOR_CYAN);
      }
      else {
        if (game->ship.lives > 0) {
          game->ship.lives--;
          game->weapon_fire_rate_active = false;
          game->weapon_multi_shot_active = false;
        }
        if (game->ship.lives <= 0) {
          game->state = STATE_GAME_OVER;
        }
      }
      break;
    }
  }

  // ship vs powerups
  for (int i = 0; i < MAX_POWERUPS; i++) {
    if (!game->powerups[i].base.active) {
      continue;
    }

    int ship_x = game->ship.base.x;
    int ship_y = game->ship.base.y;
    int ship_w = game->ship.base.width;
    int ship_h = game->ship.base.height;

    if (game->shield_timer > 0) {
      ship_x -= 5;
      ship_y -= 5;
      ship_w += 10;
      ship_h += 10;
    }

    if (rects_collide(ship_x, ship_y, ship_w, ship_h,
                      game->powerups[i].base.x, game->powerups[i].base.y,
                      game->powerups[i].base.width, game->powerups[i].base.height)) {
      
      game->powerups[i].base.active = false;
      
      if (game->powerups[i].type == POWERUP_HEALTH) {
        game->ship.lives++;
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "+1 LIFE!", COLOR_GREEN);
      } else if (game->powerups[i].type == POWERUP_SHIELD) {
        game->shield_timer = TARGET_FPS * 8; // 8 seconds
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "SHIELD ON!", COLOR_CYAN);
      } else if (game->powerups[i].type == POWERUP_FIRE_RATE) {
        game->weapon_fire_rate_active = true;
        game->weapon_multi_shot_active = false;
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "FAST FIRE!", COLOR_YELLOW);
      } else if (game->powerups[i].type == POWERUP_MULTI_SHOT) {
        game->weapon_multi_shot_active = true;
        game->weapon_fire_rate_active = false;
        spawn_floating_text(game, game->ship.base.x, game->ship.base.y - 20, "MULTI SHOT!", COLOR_YELLOW);
      }
    }
  }
}
