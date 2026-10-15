#include "projectile.h"

Projectile *createProjectile(int x, int y, char type, Sprite *sprite) {
    Projectile *projectile = (Projectile *)malloc(sizeof(Projectile));
    if (projectile == NULL) {
        return NULL;
    }
    projectile->x = x;
    projectile->y = y;
    projectile->type = type;
    projectile->sprite = sprite;
    projectile->active = true;
    return projectile;
}

void destroyProjectile(Projectile *projectile) {
    if (projectile != NULL) {
        free(projectile);
    }
}

void drawProjectile(Projectile *projectile) {
    if (projectile != NULL && projectile->active && projectile->sprite != NULL) {
        draw_sprite(projectile->sprite, projectile->x, projectile->y);
    }
}

void projectileMove(Projectile *projectile) {
    if (projectile == NULL) return;
    if(projectile->type == 'P') projectile->y -= 10;
    else if (projectile->type == 'A') projectile->y += 10;

    if (projectile->y < 0 || 
        projectile->y > 600 - projectile->sprite->height)
        projectile->active = false; 
}

bool checkCollisionAlien(Projectile *projectile, Alien *alien) {
  if (projectile->x < alien->x + alien->sprite->width &&
      projectile->x + projectile->sprite->width > alien->x &&
      projectile->y < alien->y + alien->sprite->height &&
      projectile->y + projectile->sprite->height > alien->y) {
    return true;
  }
  return false;
}

bool checkCollisionBarrier(Projectile *projectile, Barrier *barrier) {
  if (projectile->x < barrier->x + barrier->sprite->width &&
      projectile->x + projectile->sprite->width > barrier->x &&
      projectile->y < barrier->y + barrier->sprite->height &&
      projectile->y + projectile->sprite->height > barrier->y) {
    return true;
  }
  return false;
}

bool checkCollisionPlayer(Projectile *projectile, Player *player) {
  if (projectile->x < player->x + player->sprite->width &&
      projectile->x + projectile->sprite->width > player->x &&
      projectile->y < player->y + player->sprite->height &&
      projectile->y + projectile->sprite->height > player->y) {
    return true;
  }
  return false;
}
