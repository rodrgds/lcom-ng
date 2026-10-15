#include "game.h"

#include <stdlib.h>

#include "../config.h"
#include "terrain/terrain.h"

static const int team_spawn_start_x[MAX_TEAMS] = {
  TEAM0_WORM0_START_X,
  TEAM1_WORM0_START_X
};

static const int team_spawn_end_x[MAX_TEAMS] = {
  TEAM0_WORM1_START_X,
  TEAM1_WORM1_START_X
};

struct GameContext {
  EventQueue *queue;
  Cursor *cursor;
  Worm *worms[MAX_WORMS];
  int current_team;
  int selected_worm[MAX_TEAMS];
  Projectile *projectile;
  Bazooka    *bazooka;
  Grenade    *grenade;
  Explosion  *explosions[MAX_EXPLOSIONS];
  Water      *water;
  uint32_t    turn_ticks;
};

static int first_living_worm_on_team(const GameContext *context, int team) {
  if (context == NULL || team < 0 || team >= MAX_TEAMS) return -1;

  int start = team * WORMS_PER_TEAM;
  for (int i = 0; i < WORMS_PER_TEAM; i++) {
    int worm_index = start + i;
    if (worm_is_alive(context->worms[worm_index])) return worm_index;
  }

  return -1;
}

static bool is_valid_team_worm(int team, int worm_index) {
  if (team < 0 || team >= MAX_TEAMS) return false;
  if (worm_index < 0 || worm_index >= MAX_WORMS) return false;
  return game_context_get_worm_team(worm_index) == team;
}

static void ensure_selected_worm_is_alive(GameContext *context, int team) {
  if (context == NULL || team < 0 || team >= MAX_TEAMS) return;

  int selected = context->selected_worm[team];
  if (is_valid_team_worm(team, selected) && worm_is_alive(context->worms[selected])) return;

  context->selected_worm[team] = first_living_worm_on_team(context, team);
}

static int find_spawn_y(int x) {
  Hitbox hitbox;

  for (int y = 0; y + WORM_HITBOX_HEIGHT + 1 < SCREEN_H; y++) {
    hitbox = hitbox_create((uint16_t) x,
                           (uint16_t) y,
                           WORM_HITBOX_WIDTH,
                           WORM_HITBOX_HEIGHT);
    if (terrain_collides_hitbox(hitbox)) continue;

    hitbox_set_position(&hitbox, (uint16_t) x, (uint16_t) (y + 1));
    if (terrain_collides_hitbox(hitbox)) return y;
  }

  for (int y = 0; y + WORM_HITBOX_HEIGHT < SCREEN_H; y++) {
    hitbox = hitbox_create((uint16_t) x,
                           (uint16_t) y,
                           WORM_HITBOX_WIDTH,
                           WORM_HITBOX_HEIGHT);
    if (!terrain_collides_hitbox(hitbox)) return y;
  }

  return WORM_START_Y;
}

static int spawn_x_for_worm_index(int index) {
  int team = game_context_get_worm_team(index);
  int team_worm = index % WORMS_PER_TEAM;

  if (team < 0 || team >= MAX_TEAMS) return SCREEN_W / 2;
  if (WORMS_PER_TEAM <= 1) {
    return (team_spawn_start_x[team] + team_spawn_end_x[team]) / 2;
  }

  int start = team_spawn_start_x[team];
  int end = team_spawn_end_x[team];
  return start + ((end - start) * team_worm) / (WORMS_PER_TEAM - 1);
}

static Worm *create_worm_at_index(int index) {
  if (index < 0 || index >= MAX_WORMS) return NULL;

  int x = spawn_x_for_worm_index(index);
  return worm_create(x, find_spawn_y(x));
}

GameContext *game_context_create(void) {
  GameContext *context = malloc(sizeof(GameContext));
  if (context == NULL) return NULL;

  context->queue = event_queue_create(EVENT_QUEUE_CAPACITY);
  context->cursor = cursor_create();
  for (int i = 0; i < MAX_WORMS; i++) {
    context->worms[i] = create_worm_at_index(i);
  }
  context->current_team = 0;
  for (int i = 0; i < MAX_TEAMS; i++) {
    context->selected_worm[i] = i * WORMS_PER_TEAM;
  }
  context->projectile     = NULL;
  context->bazooka        = NULL;
  context->grenade        = NULL;
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    context->explosions[i] = NULL;
  }
  context->water = water_create(WATER_LEVEL_Y);
  context->turn_ticks = TURN_DURATION_TICKS;

  if (context->queue == NULL || context->cursor == NULL || context->water == NULL) {
    game_context_destroy(context);
    return NULL;
  }

  for (int i = 0; i < MAX_WORMS; i++) {
    if (context->worms[i] == NULL) {
      game_context_destroy(context);
      return NULL;
    }
  }

  return context;
}

void game_context_destroy(GameContext *context) {
  if (context == NULL) return;
  event_queue_destroy(context->queue);
  cursor_destroy(context->cursor);
  for (int i = 0; i < MAX_WORMS; i++) {
    worm_destroy(context->worms[i]);
  }
  projectile_destroy(context->projectile);
  bazooka_destroy(context->bazooka);
  grenade_destroy(context->grenade);
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    explosion_destroy(context->explosions[i]);
  }
  water_destroy(context->water);
  free(context);
}

EventQueue *game_context_get_queue(GameContext *context) {
  if (context == NULL) return NULL;
  return context->queue;
}

Cursor *game_context_get_cursor(const GameContext *context) {
  if (context == NULL) return NULL;
  return context->cursor;
}

Worm *game_context_get_worm(const GameContext *context, int index){
  if (context == NULL) return NULL;
  if (index < 0 || index >= MAX_WORMS) return NULL;
  return context->worms[index];
}

int game_context_get_current_team(const GameContext *context) {
  if (context == NULL) return 0;
  return context->current_team;
}

int game_context_get_active_worm_index(const GameContext *context) {
  if (context == NULL) return 0;

  int team = context->current_team;
  if (team < 0 || team >= MAX_TEAMS) return 0;

  int selected = context->selected_worm[team];
  if (!is_valid_team_worm(team, selected) || !worm_is_alive(context->worms[selected])) {
    return first_living_worm_on_team(context, team);
  }
  return selected;
}

Worm *game_context_get_active_worm(const GameContext *context) {
  if (context == NULL) return NULL;
  return game_context_get_worm(context, game_context_get_active_worm_index(context));
}

int game_context_get_worm_team(int worm_index) {
  if (worm_index < 0 || worm_index >= MAX_WORMS) return -1;
  return worm_index / WORMS_PER_TEAM;
}

bool game_context_is_team_alive(const GameContext *context, int team) {
  return first_living_worm_on_team(context, team) >= 0;
}

int game_context_select_worm(GameContext *context, int worm_index) {
  if (context == NULL) return 1;
  if (!is_valid_team_worm(context->current_team, worm_index)) return 1;
  if (!worm_is_alive(context->worms[worm_index])) return 1;

  context->selected_worm[context->current_team] = worm_index;
  return 0;
}

int game_context_select_worm_at_position(GameContext *context, int16_t x, int16_t y) {
  if (context == NULL || x < 0 || y < 0) return 1;

  for (int i = 0; i < MAX_WORMS; i++) {
    Worm *worm = game_context_get_worm(context, i);
    if (game_context_get_worm_team(i) != context->current_team || !worm_is_alive(worm)) continue;

    Hitbox hitbox = worm_get_hitbox(worm);
    if (hitbox_contains_point(&hitbox, (uint16_t) x, (uint16_t) y)) {
      return game_context_select_worm(context, i);
    }
  }

  return 1;
}

Projectile *game_context_get_projectile(const GameContext *context){
  if (context == NULL) return NULL;
  return context->projectile;
}

uint32_t game_context_ticks_left(const GameContext *context) {
  if (context == NULL) return 0;
  return context->turn_ticks;
}

Explosion *game_context_get_explosion(const GameContext *context, int index) {
  if (context == NULL) return NULL;
  if (index < 0 || index >= MAX_EXPLOSIONS) return NULL;
  return context->explosions[index];
}

void game_context_add_explosion(GameContext *context, Explosion *e) {
  if (context == NULL || e == NULL) return;
  
  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    if (context->explosions[i] == NULL) {
      context->explosions[i] = e;
      return;
    } else if (!explosion_is_active(context->explosions[i])) {
      explosion_destroy(context->explosions[i]);
      context->explosions[i] = e;
      return;
    }
  }
  explosion_destroy(e);
}

const Water *game_context_get_water(const GameContext *context) {
  if (context == NULL) return NULL;
  return context->water;
}

void game_context_set_projectile(GameContext *context, Projectile *projectile) {
  if (context == NULL) {
    projectile_destroy(projectile);
    return;
  }
  projectile_destroy(context->projectile);
  context->projectile = projectile;
}

Bazooka *game_context_get_bazooka(const GameContext *context) {
  if (context == NULL) return NULL;
  return context->bazooka;
}

void game_context_set_bazooka(GameContext *context, Bazooka *bazooka) {
  if (context == NULL) {
    bazooka_destroy(bazooka);
    return;
  }
  bazooka_destroy(context->bazooka);
  context->bazooka = bazooka;
}

Grenade *game_context_get_grenade(const GameContext *context) {
  if (context == NULL) return NULL;
  return context->grenade;
}

void game_context_set_grenade(GameContext *context, Grenade *grenade) {
  if (context == NULL) {
    grenade_destroy(grenade);
    return;
  }
  grenade_destroy(context->grenade);
  context->grenade = grenade;
}

void game_context_tick(GameContext *context) {
  if (context == NULL) return;
  if (context->turn_ticks == 0) return;
  context->turn_ticks--;
}

void game_context_reset_turn(GameContext *context) {
  if (context == NULL) return;
  context->turn_ticks = TURN_DURATION_TICKS;
}

void game_context_reset(GameContext *context) {
  if (context == NULL) return;

  GameEvent e;
  while (!event_queue_empty(context->queue))
    event_queue_pop(context->queue, &e);

  for (int i = 0; i < MAX_WORMS; i++) {
    worm_destroy(context->worms[i]);
    context->worms[i] = create_worm_at_index(i);
  }

  bazooka_destroy(context->bazooka);
  context->bazooka = NULL;
  grenade_destroy(context->grenade);
  context->grenade = NULL;
  projectile_destroy(context->projectile);
  context->projectile = NULL;

  for (int i = 0; i < MAX_EXPLOSIONS; i++) {
    explosion_destroy(context->explosions[i]);
    context->explosions[i] = NULL;
  }

  context->current_team = 0;
  for (int i = 0; i < MAX_TEAMS; i++)
    context->selected_worm[i] = i * WORMS_PER_TEAM;
  context->turn_ticks = TURN_DURATION_TICKS;
}

void game_context_next_team_turn(GameContext *context) {
  if (context == NULL) return;

  for (int i = 0; i < MAX_TEAMS; i++) {
    context->current_team = (context->current_team + 1) % MAX_TEAMS;
    if (game_context_is_team_alive(context, context->current_team)) break;
  }

  ensure_selected_worm_is_alive(context, context->current_team);
  game_context_reset_turn(context);
}
