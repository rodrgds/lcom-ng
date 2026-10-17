#include "Player.h"
#include "video.h"
#include "kbd.h"

Player player(int h, int d){
  Player p;
  p.hp = h;
  p.dmg = d;
  p.invulnerable = 0;
  p.e = entity(PLAYER);
  p.dir.down = false;
  p.dir.left = false;
  p.dir.right = false;
  p.dir.up = false;
  p.fc = 0;
  return p;
}

void drawPlayer(Player *p){
  static int count = 0;
  if(p->invulnerable == 0){
    draw_sproot(p->e.img, p->e.p.x, p->e.p.y);
  }
  if(p->invulnerable == 1){
    count++;
    if(count % 2 == 0) draw_sproot(p->e.img, p->e.p.x, p->e.p.y);
  }
}

void movePlayerLeft(Player * p){
  if(p->e.p.x > 0) {
    if (p->e.p.x >= p->e.xspeed)  setX(&(p->e), p->e.p.x - p->e.xspeed);
    else setX(&(p->e), 0);
  }
}

void movePlayerRight(Player * p){
  if (p->e.p.x < getHRes() - p->e.img.width){
    if (p->e.p.x + p->e.xspeed < getHRes() - p->e.img.width) setX(&(p->e), p->e.p.x + p->e.xspeed);
    else setX(&(p->e), getHRes() - p->e.img.width);
  }
}

void movePlayerUp(Player * p){
  int playerCeiling = p->e.img.height *2;
  if(p->e.p.y > playerCeiling) {
    if (p->e.p.y >= p->e.yspeed)  setY(&(p->e), p->e.p.y - p->e.yspeed);
    else setY(&(p->e), playerCeiling);
  }
}

void movePlayerDown(Player * p){
  if(p->e.p.y < getVRes() - p->e.img.height){
    if (p->e.p.y + p->e.yspeed < getVRes() - p->e.img.height)  setY(&(p->e), p->e.p.y + p->e.yspeed);
    else setY(&(p->e), getVRes() - p->e.img.height);
  }
}

void movePlayer(Player * p){
  if(p->dir.right == true){
    movePlayerRight(p);
  }
  if(p->dir.left == true){
    movePlayerLeft(p);
  }
  if(p->dir.up == true){
    movePlayerUp(p);
  }
  if(p->dir.down == true){
    movePlayerDown(p);
  }
}

bool alive(Player const * p){
  return p->hp > 0;
}
