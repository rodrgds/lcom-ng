#include "../../include/monster/monsterController.h"
/** @file monsterController.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

struct monsterController {
    MonsterModel* model;
    RigidBody* rb;
    BoxCollider* bc;
    Path* path;
};

MonsterController* monsterControllerNew(MonsterModel* model, Path* path) {
    MonsterController* m = malloc(sizeof(MonsterController));

    m->model = model;

    m->rb = rigidBodyNew();

    Vec2d mPos;
    monsterModelGetPos(model, &mPos);
    m->bc = boxColliderNew(mPos, (Vec2d){0, 0}, monsterModelGetSize(model), MonsterTag);
    m->path = path;
    pathSubscribe(m->path, m->model);

    return m;
}

void monsterControllerUpdate(MonsterController* controller, Arena* arena, float delta_t) {

    MonsterModel* model = controller->model;

    monsterModelSetDead(model, !(monsterModelGetHP(model) > 0));

    
    if(pathGetNumOfPoints(controller->path)-1==pathGetCurPointIndex(controller->path,controller->model)){
      monsterModelSetFinishedPath(model,true); 
    }
 
   // printf("\nhere1\n");
   // printf("path index:%d",pathGetNumOfPoints(controller->path)-1);//pathGetCurPointIndex(controller->path,model)

    if (!monsterModelGetDead(model)&& !monsterModelGetFinishedPath(model) ) {//

        Vec2d delta_x, monsterPos;

        monsterModelGetPos(model, &monsterPos);

        pathFollowingDir(controller->path, model, &monsterPos, &delta_x);

        monsterModelSetDirWithVec(model, &delta_x);

        monsterModelGetVel(model, &delta_x);
        vec2dMultiply(&delta_x, delta_t);

        vec2dAddVec(&monsterPos, &delta_x);

        monsterModelSetPos(model, &monsterPos);

        rigidBodyUpdate(controller->rb, &monsterPos, arena, controller->bc, delta_t);
        // printf("\nhere2\n");
    }
}

void monsterControllerFree(MonsterController* controller) {
    rigidBodyFree(controller->rb);
    boxColliderFree(controller->bc);
    free(controller);
}

RigidBody* monsterControllerGetRigidBody(MonsterController* m) {
    return m->rb;
}

BoxCollider* monsterControllerGetBoxCollider(MonsterController* m) {
    return m->bc;
}

Path* monsterControllerGetPath(MonsterController* m) {
    return m->path;
}

void monsterControllerGoBackToStart(MonsterController* m){
    pathSetCurPointIndex(m->path,m->model,0);
    
}

Vec2d monsterControllerGetFutureDisplacement(MonsterController* controller,float delta_t){
    Vec2d delta_x, monsterPos;


    MonsterModel* model = controller->model;
    monsterModelGetPos(model, &monsterPos);

    pathFollowingDir(controller->path, model, &monsterPos, &delta_x);

    monsterModelSetDirWithVec(model, &delta_x);

    monsterModelGetVel(model, &delta_x);
    vec2dMultiply(&delta_x, delta_t);

    return delta_x;

}

