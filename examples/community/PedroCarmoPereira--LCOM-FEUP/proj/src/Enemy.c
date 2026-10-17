#include "Enemy.h"

Enemy enemy(EntityType t){
  Enemy e;
  e.e = entity(t);
  switch (t){
    case ENEMY1:
      e.dmg = 10;
      e.hp = 20;
      e.fr = 1;
      break;
    case ENEMY2:
      e.dmg = 20;
      e.hp = 50;
      e.fr = 1;
      break;
    case BOSS:
      e.dmg = 20;
      e.hp = 200;
      e.fr = 1;
      break;
    default:
      break;
  }
  return e;
}
