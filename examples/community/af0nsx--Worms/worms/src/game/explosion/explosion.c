#include "explosion.h"
#include <stdlib.h>

struct Explosion {
  int32_t x;
  int32_t y;
  uint8_t current_tick;
  uint8_t total_ticks;
  bool is_active;
};

Explosion* explosion_create(int32_t x, int32_t y, uint8_t duration_ticks) {
  Explosion *e = (Explosion *)malloc(sizeof(Explosion));
  if (e == NULL) return NULL;

  e->x = x;
  e->y = y;
  e->current_tick = 0;
  e->total_ticks = duration_ticks;
  e->is_active = true;

  return e;
}

void explosion_destroy(Explosion *e) {
  if (e == NULL) return;
  free(e);
}

int32_t explosion_get_x(const Explosion *e) {
  if (e == NULL) return 0;
  return e->x;
}

int32_t explosion_get_y(const Explosion *e) {
  if (e == NULL) return 0;
  return e->y;
}

uint8_t explosion_get_current_tick(const Explosion *e) {
  if (e == NULL) return 0;
  return e->current_tick;
}

uint8_t explosion_get_total_ticks(const Explosion *e) {
  if (e == NULL) return 0;
  return e->total_ticks;
}

bool explosion_is_active(const Explosion *e) {
  if (e == NULL) return false;
  return e->is_active;
}

void explosion_advance_tick(Explosion *e) {
  if (e == NULL || !e->is_active) return;
  
  e->current_tick++;
  if (e->current_tick >= e->total_ticks) {
    e->is_active = false;
  }
}
