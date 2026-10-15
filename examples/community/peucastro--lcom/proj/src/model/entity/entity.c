#include <lcom/lcf.h>

#include "model/entity/entity.h"
#include "model/game/game.h"

#define CELL_SIZE 64

int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data) {
  if (e == NULL) {
    fprintf(stderr, "init_entity: entity pointer cannot be null.");
    return 1;
  }

  if (sp == NULL) {
    fprintf(stderr, "init_entity: sprite pointer cannot be null.");
    return 1;
  }

  e->x = x;
  e->y = y;
  e->dir = RIGHT;
  e->sprite = sp;
  e->active = true;
  e->data = data;

  e->move.px = x * CELL_SIZE;
  e->move.py = y * CELL_SIZE;

  e->move.tx = x;
  e->move.ty = y;
  e->move.sx = x;
  e->move.sy = y;

  e->anim = NULL;

  e->move.moving = false;
  e->move.tick = 0;
  e->move.total_ticks = 60;

  e->update = default_update;

  return 0;
}

int(reset_entity)(Entity *e) {
  if (e == NULL) {
    fprintf(stderr, "reset_entity: entity pointer cannot be null.");
    return 1;
  }

  e->x = 0;
  e->y = 0;
  e->sprite = NULL;
  e->data = 0;
  e->active = false;

  if (e->anim) {
    destroy_animSprite(e->anim);
    e->anim = NULL;
  }

  return 0;
}

static void check_explosion_damage(Entity *e, Game *game) {
  for (uint8_t i = 0; i < game->num_explosions; i++) {
    Entity *exp = &game->explosions[i];
    if (!exp->active)
      continue;
    if (exp->x == e->x && exp->y == e->y) {
      if (e == &game->player) {
        e->data--;
        if (e->data <= 0) {
          e->active = false;
          game->state = LOSE;
        }
      }
      else {
        e->data--;
        if (e->data <= 0)
          e->active = false;
      }
      break;
    }
  }
}


static void check_enemy_player_collision(Entity *e, Game *game) {
  if (!e->active || !game->player.active) return;

  // If this entity is the player, check if any enemy is on them
  if (e == &game->player) {
    for (uint8_t i = 0; i < game->num_enemies; i++) {
      Entity *enemy = &game->enemies[i];
      if (!enemy->active) continue;
      if (enemy->x == e->x && enemy->y == e->y) {
        e->data--;
        if (e->data <= 0) {
          e->active = false;
          game->state = LOSE;
        }
        break;
      }
    }
  }

  // If this entity is an enemy, check if it moved onto the player
  else {
    if (e->x == game->player.x && e->y == game->player.y) {
      game->player.data--;
      if (game->player.data <= 0) {
        game->player.active = false;
        game->state = LOSE;
      }
    }
  }
}


void(default_update)(Entity *e, Game *game, uint32_t counter) {
  if (!e->active) {
    return;
  }

  if (e->move.moving) {
    float step_x = (e->move.tx - e->x) * ((float) CELL_SIZE / e->move.total_ticks);
    float step_y = (e->move.ty - e->y) * ((float) CELL_SIZE / e->move.total_ticks);

    e->move.px += step_x;
    e->move.py += step_y;

    if (++e->move.tick >= e->move.total_ticks) {
      game->board.elements[e->move.sy][e->move.sx] = EMPTY_SPACE;
      e->x = e->move.tx;
      e->y = e->move.ty;
      e->move.px = e->x * CELL_SIZE;
      e->move.py = e->y * CELL_SIZE;
      e->move.moving = false;
      e->move.tick = 0;

      check_explosion_damage(e, game);
      check_enemy_player_collision(e, game);
    }
  }

  if (e->move.moving && e->anim) {
    update_animSprite(e->anim);
    e->sprite = e->anim->sp;
  }
}
