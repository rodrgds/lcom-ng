#include "include/bullet.h"
#include "include/config.h"
#include "include/player.h"

#define MAX_BULLETS 64

// Estado privado do módulo de balas
static Bullet bullets[MAX_BULLETS];
static int bullet_count = 0;

void bullet_init(void) {
    bullet_count = 0;
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].is_active = false;
    }
}

int bullet_spawn(int x, int y, int direction, int speed, BulletType type) {
  // Procura primeiro um slot inativo para reutilizar
  int slot = -1;
  for (int i = 0; i < bullet_count; i++) {
    if (!bullets[i].is_active) {
      slot = i;
      break;
    }
  }

  if (slot == -1) {
    if (bullet_count >= MAX_BULLETS) return -1; // todas as 64 balas estão ativas
    slot = bullet_count++;
  }

  bullets[slot].x = x;
  bullets[slot].y = y;

  bullets[slot].direction = direction;
  bullets[slot].speed = speed;
  bullets[slot].type = type;

  bullets[slot].is_active = true;

  return slot;
}

void bullet_update(void) {
    for (int i = 0; i < bullet_count; i++) {
        if (!bullets[i].is_active) continue;

        // Move a bala na direção vertical
        bullets[i].y += bullets[i].direction * bullets[i].speed;

        // Remove bala se sair do ecrã
        if (bullets[i].y < -16 || bullets[i].y > SCREEN_H) {
            bullets[i].is_active = false;
        }
    }
}

Bullet *bullet_get_all(void) {
    return bullets;
}

int bullet_get_count(void) {
    return bullet_count;
}

void bullet_deactivate(int index) {
    if (index >= 0 && index < bullet_count) {
        bullets[index].is_active = false;
    }
}

void bullet_cleanup(void) {
    bullet_init();
}
