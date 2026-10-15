#include "../../include/monster/monsterView.h" 
/** @file monsterView.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
struct monsterView{
    MonsterModel* model;
    AnimatedSprite* animation;

    bool flipped;

};

MonsterView* monsterViewNew(MonsterModel* model,AnimatedSprite* (*animationBuilder)()){
  MonsterView* view =malloc(sizeof(MonsterView));
  view->model=model; 
  view->animation= animationBuilder(); 
  return view;
}

void monsterViewFree(MonsterView* view){ 
  animatedSpriteFree(view->animation);
  free(view);
}


void monsterViewUpdate (MonsterView* view, Terminal* terminal,float delta_t){ 
  if(monsterModelGetVelX(view->model)>0){
    view->flipped=false;
  }
  else if (!(monsterModelGetVelX(view->model)==0))
  { 
    view->flipped=true;
  }
  if(!monsterModelGetDead(view->model)&& !monsterModelGetFinishedPath(view->model) ){// 
   animatedSpriteUpdate(monsterModelGetX(view->model),monsterModelGetY(view->model),view->animation,terminal,delta_t,view->flipped);
  }
}
