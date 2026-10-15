#include "../../include/monster/monsterModel.h"
/** @file monsterModel.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
 

struct monsterModel
{

  Vec2d pos;
  Vec2d size;
  Vec2d vel; 

  bool dead;
  int hp;
  int initialHP;

  bool finishedPath;

};


MonsterModel* monsterModelNew(float x,float y,float sizeX,float sizeY,float velX,float velY,int hp){
  MonsterModel* m=malloc(sizeof(MonsterModel));
  m->pos= (Vec2d){x,y};
  m->size=(Vec2d){sizeX,sizeY};
  m->vel=(Vec2d){velX,velY}; 
  m->hp=hp; 
  m->initialHP=hp;
  m->dead=false;
  m->finishedPath=false;

  return m;
}


 void monsterModelFree(MonsterModel* m){  
  free(m); 
}


 

int monsterModelGetHP(MonsterModel* m){
  return m->hp;
}

void monsterModelSetHP(MonsterModel* m,int hp){
   m->hp=hp;
}

int monsterModelGetInitialHP(MonsterModel* m){
  return m->initialHP;
}

void monsterModelSetInitialHP(MonsterModel* m,int hp){
   m->initialHP=hp;
}

void monsterModelSumHP(MonsterModel* m,int val){
   m->hp+=val;
}

bool monsterModelGetDead(MonsterModel* m){
  return m->dead;
}

void monsterModelSetDead(MonsterModel* m,bool dead){
   m->dead=dead;
}

bool monsterModelGetFinishedPath(MonsterModel* m){
  return m->finishedPath;
}

void monsterModelSetFinishedPath(MonsterModel* m,bool finishedPath){
   m->finishedPath=finishedPath;
}

float monsterModelGetX(MonsterModel* m){
  return vec2dGetX(&m->pos);
}
float monsterModelGetY(MonsterModel* m){
  return vec2dGetY(&m->pos);
  
}

float monsterModelGetSizeX(MonsterModel* m){
  return vec2dGetX(&m->size);
}
float monsterModelGetSizeY(MonsterModel* m){
  return vec2dGetY(&m->size);
}

Vec2d monsterModelGetSize(MonsterModel* m){
  return  m->size;
}

float monsterModelGetVelX(MonsterModel* m){
  return vec2dGetX(&m->vel);
}
float monsterModelGetVelY(MonsterModel* m){
  return vec2dGetY(&m->vel);
}



void monsterModelGetPos(MonsterModel*m,Vec2d* pos){
  vec2dSetX(pos,vec2dGetX(&m->pos));
  vec2dSetY(pos,vec2dGetY(&m->pos));
}


void monsterModelSetPos(MonsterModel* m,Vec2d* pos){
  vec2dSetWithVec2d(&m->pos,pos);
}


void monsterModelGetVel(MonsterModel* m,Vec2d* pos){
    vec2dSetWithVec2d(pos,&m->vel);
}

void monsterModelSetVel(MonsterModel* m,Vec2d* pos){
    vec2dSetWithVec2d(&m->vel,pos);
}





void monsterModelSetX(MonsterModel* m,float x){
  vec2dSetX(&m->pos,x);
}
void monsterModelSetY(MonsterModel* m,float y){
  vec2dSetY(&m->pos,y);
}

void monsterModelSetVelX(MonsterModel* m,float x){
  vec2dSetX(&m->vel,x);
}
void monsterModelSetVelY(MonsterModel* m,float y){
  vec2dSetY(&m->vel,y);
}



void monsterModelSetDir(MonsterModel* m,float dx, float dy){
  Vec2d v={dx,dy};
  monsterModelSetDirWithVec(m,&v); 
}

void monsterModelSetDirWithVec(MonsterModel* m,Vec2d* v){
  float dm2=vec2dModule2(v);
  float cur_m=vec2dModule(&m->vel);
  if( (dm2>(1-EPSILON)) && ((1+EPSILON)>dm2) ){
    
    vec2dSetWithVec2d(&m->vel,v);
    vec2dMultiply(&m->vel,cur_m);
  }
  else{
    
    vec2dSetWithVec2d(&m->vel,v);
    vec2dNormalize(&m->vel);
    vec2dMultiply(&m->vel,cur_m);
  }
}
