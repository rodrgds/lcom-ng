#include "enemy_bullet.h"
#include <math.h>

enemy_bullet *createEnemyBullet(int x, int y, int playerX, int playerY, int damage, Sprite *sp) {
    enemy_bullet *p = (enemy_bullet *)malloc(sizeof(enemy_bullet));
    if (p == NULL)
        return NULL;

    p->x = x;
    p->y = y;
    p->playerX = playerX;
    p->playerY = playerY;
    p->damage = damage;
    p->sprite = sp;
    p->active = true;

    float distance = sqrt((playerX - x) * (playerX - x) + (playerY - y) * (playerY - y));
    p->dx = (playerX - x) / distance;
    p->dy = (playerY - y) / distance;

    return p;
}
void destroyEnemyBullets(enemy_bullet *p) {
    free(p);
}

void moveEnemyBullet(enemy_bullet *p) {
    if (p == NULL || !p->active) return;

    p->x += p->dx * 5; 
    p->y += p->dy * 5;

    if (p->x < 0 || p->x >= 1024 || p->y < 0 || p->y >= 768) { 
        p->active = false;
    }
}

