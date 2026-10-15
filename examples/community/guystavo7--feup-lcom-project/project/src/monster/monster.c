#include "../../include/monster/monster.h"
/** @file monster.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

struct monster {

    MonsterModel* model;
    MonsterView* view;
    MonsterController* controller;
};

unsigned int monsterSizeof() {
    return sizeof(Monster);
}

Monster* monsterNew(float x, float y, float sizeX, float sizeY, float velocity, int hp, AnimatedSprite* (*animationBuilder)(), Path* path) {
    Monster* m = malloc(sizeof(Monster));
    m->model = monsterModelNew(x, y, sizeX, sizeY, velocity, 0, hp);
    m->view = monsterViewNew(m->model, animationBuilder);
    m->controller = monsterControllerNew(m->model, path);
    return m;
}

void monsterSumHP(Monster* m, int val) {
    monsterModelSumHP(m->model, val);
}

bool monsterIsDead(Monster* m) {
    return monsterModelGetDead(m->model);
}

bool monsterGetFinishedPath(Monster* m) {
    return monsterModelGetFinishedPath(m->model);
}

void monsterUpdate(Monster* m, Arena* arena, Terminal* terminal, float delta_t) {
    monsterControllerUpdate(m->controller, arena, delta_t);
    monsterViewUpdate(m->view, terminal, delta_t);
}

void monsterFree(Monster* m) {
    monsterModelFree(m->model);
    monsterViewFree(m->view);
    monsterControllerFree(m->controller);
    free(m);
}

RigidBody* monsterGetRigidBody(Monster* m) {
    return monsterControllerGetRigidBody(m->controller);
}

BoxCollider* monsterGetBoxCollider(Monster* m) {
    return monsterControllerGetBoxCollider(m->controller);
}

void monsterReset(Monster* m, Vec2d* pos) {
    monsterModelSetHP(m->model, monsterModelGetInitialHP(m->model));

    monsterModelSetDead(m->model, false);
    monsterModelSetFinishedPath(m->model, false);
    monsterModelSetPos(m->model, pos);
    monsterControllerGoBackToStart(m->controller);
}

Vec2d monsterGetFutureDisplacement(Monster* m,float delta_t){
  return monsterControllerGetFutureDisplacement(m->controller,delta_t);
}
