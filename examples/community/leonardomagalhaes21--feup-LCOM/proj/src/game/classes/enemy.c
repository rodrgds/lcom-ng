#include "enemy.h"

enemy *createEnemy(int life, int damage, int x, int y, int speed_x, int speed_y, Sprite *sp, bool alive) {
    enemy *p = (enemy *)malloc(sizeof(*p));
    if (p == NULL)
        return NULL;

    p->life = life;
    p->damage = damage;
    p->x = x;
    p->y = y;
    p->speed_x = speed_x;
    p->speed_y = speed_y;
    p->sprite = sp;
    p->alive = alive;

    return p;
}

enemy *createEnemyWithDefaultShot(int life, int damage, int x, int y, int speed_x, int speed_y, Sprite *sp, bool alive) {
    return createEnemy(life, damage, x, y, speed_x, speed_y, sp, alive);
}



void destroyEnemy(enemy *p) {
    free(p);
}

void reviveEnemy(enemy *p, int x, int y) {
    p->x = x;
    p->y = y;
    p->alive = true;
    p->shot = NULL; 
}
