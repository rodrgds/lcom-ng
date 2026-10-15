#include <lcom/lcf.h>

#include "model/game/movement.h"

void(move_player)(Entity *p, Game *game, int16_t xmov, int16_t ymov) {
  if (p == NULL || game == NULL) {
    fprintf(stderr, "move_player: invalid player or game pointer.");
    return;
  }

  if (p->move.moving) {
    return;
  }

  if (!p->active) {
    fprintf(stderr, "move_player: player not active.");
    return;
  }

  int16_t new_x = p->x + xmov;
  int16_t new_y = p->y + ymov;

  if (new_x < 0 || new_x >= game->board.width ||
      new_y < 0 || new_y >= game->board.height) {
    fprintf(stderr, "move_player: invalid coordinate.");
    return;
  }

  const Resources *resources = get_resources();
  if (resources == NULL) {
    fprintf(stderr, "move_player: failed to load resources.");
    return;
  }

  if (xmov > 0) {
    p->dir = RIGHT;
  }
  else if (xmov < 0) {
    p->dir = LEFT;
  }
  else if (ymov > 0) {
    p->dir = DOWN;
  }
  else if (ymov < 0) {
    p->dir = UP;
  }
  p->anim = get_player_anim(p->dir);
  p->anim->cur_fig = 0;
  p->anim->cur_aspeed = 0;
  p->sprite = p->anim->sp;

  board_element_t destination = game->board.elements[new_y][new_x];

  switch (destination) {
    case ENEMY:
    case EMPTY_SPACE:
      game->board.elements[p->y][p->x] = EMPTY_SPACE;
      game->board.elements[new_y][new_x] = PLAYER;
      p->move.sx = p->x;
      p->move.sy = p->y;
      p->move.tx = new_x;
      p->move.ty = new_y;
      p->move.tick = 0;
      p->move.total_ticks = PLAYER_ANIM_SPEED * PLAYER_ANIM_FRAMES;
      p->move.moving = true;
      break;

    case POWERUP:
      if (game->powerup.active) {
        game->powerup.active = false;

        game->board.elements[p->y][p->x] = EMPTY_SPACE;
        game->board.elements[new_y][new_x] = PLAYER;
        game->player.data++;

        p->move.sx = p->x;
        p->move.sy = p->y;
        p->move.tx = new_x;
        p->move.ty = new_y;
        p->move.tick = 0;
        p->move.total_ticks = PLAYER_ANIM_SPEED * PLAYER_ANIM_FRAMES;
        p->move.moving = true;
      }
      break;

    case DOOR:
      if (game->door.active && game->num_enemies == 0) {
        game->board.elements[p->y][p->x] = EMPTY_SPACE;
        game->board.elements[new_y][new_x] = PLAYER;
        p->move.sx = p->x;
        p->move.sy = p->y;
        p->move.tx = new_x;
        p->move.ty = new_y;
        p->move.tick = 0;
        p->move.total_ticks = PLAYER_ANIM_SPEED * PLAYER_ANIM_FRAMES;
        p->move.moving = true;
      }
      break;

    case WALL:
    case BRICK:
    case BOMB:
      // block movement
      break;
    default:
      fprintf(stderr, "move_player: invalid destination.");
      return;
  }
}

void(schedule_enemy_moves)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "schedule_enemy_moves: game pointer cannot be null.");
    return;
  }

  static const int8_t dx[4] = {0, 1, 0, -1};
  static const int8_t dy[4] = {-1, 0, 1, 0};

  for (uint8_t i = 0; i < game->num_enemies; i++) {
    Entity *e = &game->enemies[i];
    if (!e->active || e->move.moving)
      continue;

    int valid[4], vc = 0;
    for (int d = 0; d < 4; d++) {
      int nx = e->x + dx[d], ny = e->y + dy[d];
      if (nx < 0 || nx >= game->board.width || ny < 0 || ny >= game->board.height)
        continue;
      board_element_t dest = game->board.elements[ny][nx];
      if (dest == EMPTY_SPACE || dest == PLAYER)
        valid[vc++] = d;
    }
    if (!vc)
      continue;

    int choice = valid[rand() % vc];

    e->move.sx = e->x;
    e->move.sy = e->y;

    e->move.tx = e->x + dx[choice];
    e->move.ty = e->y + dy[choice];
    game->board.elements[e->move.ty][e->move.tx] = ENEMY;

    e->move.moving = true;
    e->move.tick = 0;
    e->move.total_ticks = 16;

    e->anim->aspeed = e->move.total_ticks / e->anim->num_fig;
    e->anim->cur_aspeed = 0;
    e->anim->cur_fig = 0;
    e->sprite = e->anim->sp;
  }
}
