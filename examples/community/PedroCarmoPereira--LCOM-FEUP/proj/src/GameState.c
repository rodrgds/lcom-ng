#include <stdlib.h>
#include <math.h>
#include "GameState.h"
#include "video.h"
#include "Laser.h"
#include "assets/assets.h"
#include "assets/background.h"


GameState gamestate(){
  GameState g;
  g.score = 0;
  g.phase = 1;
  g.ek = 0;
  g.player = player(100, 10);
  g.obstacles = NULL;
  g.laser = NULL;
  g.enemies = NULL;
  g.enemyFire = NULL;
  return g;
}

void drawObstacles(EntityVector v){
  EntityVector iterator = v;
  while(iterator){
    drawEntity(&(iterator->curr));
    iterator = iterator->next;
  }
}

void drawLaser(LaserVector v){
  LaserVector iterator = v;
  while(iterator){
    drawEntity(&(iterator->curr.e));
    iterator = iterator->next;
  }

}

void drawEnemies(EnemyVector v){
  EnemyVector iterator = v;
  while(iterator){
    drawEntity(&(iterator)->curr.e);
    iterator = iterator->next;
  }
}

void drawGS(GameState g){
  drawPlayer(&g.player);
  drawLaser(g.laser);
  drawLaser(g.enemyFire);
  drawEnemies(g.enemies);
  drawObstacles(g.obstacles);
}

void addObstacle(EntityVector *v, Entity *e){

  EntityVector node = (EntityVector) malloc(sizeof(struct entityVector));
  node->curr = *e;
  node->next = NULL;

  if(*v == NULL){
    *v = node;
    return;
  }

  EntityVector iterator = *v;
  while(iterator->next) iterator = iterator->next;
  iterator->next = node;
}

void addEnemy(EnemyVector *v, Enemy *e){
  EnemyVector node = (EnemyVector) malloc(sizeof(struct enemyVector));
  node->curr = *e;
  node->next = NULL;

  if (*v == NULL){
    *v = node;
    return;
  }

  EnemyVector iterator = *v;
  while(iterator->next) iterator = iterator->next;
  iterator->next = node;
}

void moveObstacles(EntityVector *v){
  EntityVector iterator = *v;
  while(iterator){
    setPosition(&iterator->curr, iterator->curr.p.x + iterator->curr.xspeed,iterator->curr.p.y + iterator->curr.yspeed);
    iterator = iterator->next;
  }
}

void moveLasers(GameState *g){
  LaserVector iterator = g->laser;
  while(iterator){
  setPosition(&iterator->curr.e, iterator->curr.e.p.x + iterator->curr.e.xspeed * cos(iterator->curr.angle), iterator->curr.e.p.y + iterator->curr.e.yspeed * sin(iterator->curr.angle));
    iterator = iterator->next;
  }

  iterator = g->enemyFire;
  while(iterator){
    if(iterator->curr.angle == 0){
      setY(&iterator->curr.e, iterator->curr.e.p.y + iterator->curr.e.yspeed);
    }
    else{
    setPosition(&iterator->curr.e, iterator->curr.e.p.x + iterator->curr.e.xspeed * cos(iterator->curr.angle), iterator->curr.e.p.y + iterator->curr.e.yspeed * sin(iterator->curr.angle));
    }
    iterator = iterator->next;
  }
}

void freeObstacles(EntityVector * v){

  if(!(*v)) return;

  if (isOffScreen((*v)->curr)){
    EntityVector tmp = (*v);
    (*v) = (*v)->next;
    free(tmp);
    freeObstacles(v);
  }

  else freeObstacles(&((*v)->next));

}

int freeEnemies(EnemyVector *v){
  if (!(*v)) return 0;

  if ((*v)->curr.hp <= 0){
    EnemyVector tmp = (*v);
    (*v) = (*v)->next;
    free(tmp);
    return 1 + freeEnemies(v);
  }

  else return freeEnemies(&(*v)->next);
}

void dmgEnemies(GameState *g){

  LaserVector iteratorLaser = g->laser;
  while(iteratorLaser){
    EnemyVector iteratorEnemy = g->enemies;
    while(iteratorEnemy){
      if (colision(&iteratorLaser->curr.e, &iteratorEnemy->curr.e)){
         iteratorEnemy->curr.hp -= g->player.dmg;
         iteratorLaser->curr.e.p.y = getVRes();
       }
      iteratorEnemy = iteratorEnemy->next;
    }
  iteratorLaser = iteratorLaser->next;
  }

}

void LaserColide(GameState *g){
  EntityVector iterator = g->obstacles;
  LaserVector it1 = g->laser, it2 = g->enemyFire;
  while(it1){
    while(iterator){
      if (it1 && colision(&it1->curr.e, &iterator->curr)){
        it1->curr.e.p.y = getVRes();
      }
      iterator = iterator->next;
    }
    it1 = it1->next;
  }
  while(it2){
    while(iterator){
      if (it2 && colision(&it2->curr.e, &iterator->curr)){
        it2->curr.e.p.y = getVRes();
      }
      iterator = iterator->next;
    }
    it2 = it2->next;
  }

}

bool invalidPosition(GameState g, Entity e){
  EntityVector iterator = g.obstacles;
  int x = e.p.x, y = e.p.y;
  while(iterator){
    if (x == iterator->curr.p.x && y == iterator->curr.p.y) return true;
    iterator = iterator->next;
  }
  return false;
}

void generateAsteroids(GameState *g){

  unsigned noAsteroids = rand() % 4, direction, yMod;
  for (unsigned i = 0; i < noAsteroids;  i++){
    Entity e;
    do{
      e = entity(OBSTACLE);
      direction = rand() % 2 + 1; // If 0 is left to right, else is right to left
      if(direction == 2){
        e.p.x = getHRes() - e.img.width - 100;
        e.xspeed *= -1;
      }
      else e.p.x = 0;

      yMod = rand() % 3 + 1;

      e.p.y = getVRes() - yMod * 176;//SIZE OF ASTEROID

    }while(invalidPosition(*g, e));

    addObstacle(&g->obstacles, &e);
  }
}

void generateEnemies(GameState * g){

  unsigned xmod = 0, type;
  for (unsigned i = 0; i < 3; i++){
    Enemy e;
    do{
      if(g->phase == 1){
      e = enemy(ENEMY1);
      e.e.p.y = 0;
      e.e.p.x = xmod * 480;
     }
      else if(g->phase == 2 || g->phase == 3){
        type = rand() % 2;
        if(type == 0){
          e = enemy(ENEMY1);
          e.e.p.y = 0;
          e.e.p.x = xmod * 480;
        }
        else if(type == 1){
          e = enemy(ENEMY2);
          e.e.p.y = 0;
          e.e.p.x = xmod * 480;
        }
     }
     xmod++;
    } while(invalidPosition(*g, e.e));

      addEnemy(&g->enemies, &e);
  }
}

void generateBoss(GameState * g){
  Enemy e;
  e = enemy(BOSS);
  e.e.p.y = 0;
  e.e.p.x = 500;
  addEnemy(&g->enemies, &e);
}

bool playerCrashed(const GameState * g){

  EntityVector iterator = g->obstacles;

  while(iterator){
    if (colision(&g->player.e, &iterator->curr)) return true;
    iterator = iterator->next;
  }

  return false;

}

void enemiesFire(GameState *g){
  EnemyVector iterator = g->enemies;
  while(iterator){
    for (int i = 0; i < iterator->curr.fr; i++){
      if(iterator->curr.e.type == ENEMY1){
        float angle = ((rand() % 180) * M_PI)/180;
        Laser l = laser(iterator->curr.e.p.x + iterator->curr.e.img.width/2, iterator->curr.e.p.y, iterator->curr.e.p.x + iterator->curr.e.img.width/2, getVRes(), iterator->curr.dmg, false, angle);
        addLaser(&g->enemyFire, &l);
      }
      if(iterator->curr.e.type == ENEMY2){
        float angle = rand() % 3;
        if(angle == 0) angle = M_PI/2;
        if(angle == 1) angle = M_PI/4;
        if(angle == 2) angle = 3*M_PI/4;
        Laser l = laser(iterator->curr.e.p.x + iterator->curr.e.img.width/2, iterator->curr.e.p.y, iterator->curr.e.p.x + iterator->curr.e.img.width/2, getVRes(), iterator->curr.dmg, false, angle);
        addLaser(&g->enemyFire, &l);
      }
      if(iterator->curr.e.type == BOSS){
        float deltaX = g->player.e.p.x - iterator->curr.e.p.x;
        float deltaY = g->player.e.p.y - iterator->curr.e.p.y;
        float angle = atan2(deltaY, deltaX);
        Laser l1 = laser(iterator->curr.e.p.x + iterator->curr.e.img.width/2, iterator->curr.e.p.y, iterator->curr.e.p.x + iterator->curr.e.img.width/2, getVRes(), iterator->curr.dmg, false, angle + 0.5);
        Laser l2 = laser(iterator->curr.e.p.x + iterator->curr.e.img.width/2, iterator->curr.e.p.y, iterator->curr.e.p.x + iterator->curr.e.img.width/2, getVRes(), iterator->curr.dmg, false, angle);
        Laser l3 = laser(iterator->curr.e.p.x + iterator->curr.e.img.width/2, iterator->curr.e.p.y, iterator->curr.e.p.x + iterator->curr.e.img.width/2, getVRes(), iterator->curr.dmg, false, angle - 0.5);
        addLaser(&g->enemyFire, &l1);
        addLaser(&g->enemyFire, &l2);
        addLaser(&g->enemyFire, &l3);
      }
    }
    iterator = iterator->next;
  }
}

void generateLaser(GameState * g, int mousex, int mousey){

  if (g->player.e.p.y - mousey > 0){
    Laser l = laser(g->player.e.p.x, g->player.e.p.y - 30, mousex, mousey, g->player.dmg, true, 0);
    addLaser(&g->laser, &l);
  }
}

void dmgPlayer(GameState *g){

  if (playerCrashed(g) && g->player.invulnerable == 0){
     g->player.hp = g->player.hp - 25;
     g->player.invulnerable = 1;
   }

  LaserVector iterator = g->enemyFire;
  while(iterator){
    if (colision(&g->player.e, &iterator->curr.e ) && g->player.invulnerable == 0){
       g->player.hp = g->player.hp - iterator->curr.dmg;
       g->player.invulnerable = 1;
       iterator->curr.e.p.y = getVRes();
       break;
     }
    iterator = iterator->next;
  }
}
