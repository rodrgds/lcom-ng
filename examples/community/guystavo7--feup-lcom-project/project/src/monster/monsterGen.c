#include "../../include/monster/monsterGen.h"
/** @file monsterGen.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../../include/monster/alienMonster.h"
#include "../../include/monster/spearManMonster.h"
#include "../../include/monster/pawnMonster.h"
#include "../../include/monster/bossMonster.h"

#include "../../include/arena.h"
#include "../../include/data.h"

#include <stdlib.h>

Monster* defaultMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path,0,&startPos);

    int r = rand() % 3;
    if (r == 0) {
        return alienMonsterNew(startPos.x,startPos.y, path);
    } else if (r == 1) {
        return pawnMonsterNew(startPos.x,startPos.y, path);
    } else {
        return spearManMonsterNew(startPos.x,startPos.y, path);
    }
}

Monster* pawnMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path,0,&startPos);

    return pawnMonsterNew(startPos.x,startPos.y, path);
}

Monster* alienMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path, 0, &startPos);
    return alienMonsterNew(startPos.x, startPos.y, path);
}

Monster* spearManMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path, 0, &startPos);
    return spearManMonsterNew(startPos.x, startPos.y, path);
}

Monster* bossMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path,0,&startPos);

    return bossMonsterNew(startPos.x,startPos.y, path);
}

int singleSpawnGenTimeFunc(float totalTimeElapsed, Timer* genTimer) {
    timerSetInitialTime(genTimer, 999999);
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
    return res;
}
 
int constantSlowGenTimeFunc(float totalTimeElapsed,Timer* genTimer) { 


    timerSetInitialTime(genTimer,4); 
    
    if(totalTimeElapsed<15 ){
        timerRestart(genTimer);;  
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}
int constantSlowGenTimeFunc_1(float totalTimeElapsed,Timer* genTimer) { 


    timerSetInitialTime(genTimer,16); 
    
    if(totalTimeElapsed<15 ){
        timerRestart(genTimer);;  
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}

int constantSlowGenTimeFunc_2(float totalTimeElapsed,Timer* genTimer) { 


    timerSetInitialTime(genTimer,16); 
    
    if(totalTimeElapsed<30 ){
        timerRestart(genTimer);;  
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}

int waveNoAutoSpawnGenTimeFunc(float totalTimeElapsed, Timer* genTimer) {
    (void)totalTimeElapsed;
    (void)genTimer;
    return 0;
}

int defaultGenTimeFunc(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=50;
    if(totalTimeElapsed<1*timeInterval){
        timerSetInitialTime(genTimer,3);
     //   printf("gen3\n");
    }
    else if(totalTimeElapsed<2*timeInterval){
        timerSetInitialTime(genTimer,2);
      //  printf("gen2\n");
    }
    else if(totalTimeElapsed<3*timeInterval){
        timerSetInitialTime(genTimer,1);
     //   printf("gen1\n");
    }
    else if(totalTimeElapsed<4*timeInterval){
        timerSetInitialTime(genTimer,0.5);
    //    printf("gen0.5\n");
    }
    else if(totalTimeElapsed<5*timeInterval){
        timerSetInitialTime(genTimer,0.25);
     //   printf("gen0.25\n");
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}

struct monsterGen {
    Vector* monsterList;
    Vector* availableMonsterIdStack;
    Vector* deactivatedMonsterIdList;
    Vector* activeMonsterIdSorted; // indices of active monsters (see monsterGenGetNextActiveMonsterIndex)
    Path* path;
    
    Monster* (*monsterCreator)(Path*);

    int (*genTimeFunc)(float,Timer*);
    float totalTimeElapsed;
    Timer* genTimer;

    unsigned int numberOfInstantFinishedMonsters;
};

void freeMonsterList(void* m) {
    Monster* monster = *(Monster**)m;
    monsterFree(monster);
}

MonsterGen* monsterGenNew(Path* path, Monster* (*monsterCreator)(Path*), int (*genTimeFunc)(float,Timer*)) {
    MonsterGen* mg = malloc(sizeof(MonsterGen));
    mg->path = path;

    mg->monsterCreator = monsterCreator;

    mg->genTimeFunc = genTimeFunc;
    mg->totalTimeElapsed =0;
    mg->genTimer=timerNew(0);

    mg->monsterList = vectorCreate(sizeof(Monster*), freeMonsterList);
    mg->availableMonsterIdStack = vectorCreate(sizeof(int), NULL);
    mg->activeMonsterIdSorted = vectorCreate(sizeof(int), NULL);
    mg->deactivatedMonsterIdList = vectorCreate(sizeof(bool), NULL);
    mg->numberOfInstantFinishedMonsters = 0;

    

    return mg;
}

void monsterGenFree(MonsterGen* mg) {
    vectorFree(mg->monsterList);
    vectorFree(mg->availableMonsterIdStack);
    vectorFree(mg->deactivatedMonsterIdList);
    vectorFree(mg->activeMonsterIdSorted);
    timerFree(mg->genTimer);
    free(mg);
}

void monsterGenAddMonster(MonsterGen* mg) {
    Monster* new_monster = mg->monsterCreator(mg->path);
    vectorPushBack(mg->monsterList, (char*)&new_monster);
    vectorPushBackVal(mg->deactivatedMonsterIdList, false);

    vectorPushBackVal(mg->activeMonsterIdSorted, vectorGetSize(mg->monsterList) - 1);
}

static void monsterGen_activate_or_add_one(MonsterGen* mg) {
    Vec2d startPos;
    pathGetPoint(mg->path, 0, &startPos);

    int stackSize = (int)vectorGetSize(mg->availableMonsterIdStack);
    if (stackSize > 0) {
        int monsterId = vectorGetVal(mg->availableMonsterIdStack, (unsigned int)stackSize - 1U);
        Monster* m = *(Monster**)vectorGet(mg->monsterList, (unsigned int)monsterId);
        monsterReset(m, &startPos);
        vectorPopBack(mg->availableMonsterIdStack);
        vectorSetVal(mg->deactivatedMonsterIdList, (unsigned int)monsterId, false);
        vectorPushBackVal(mg->activeMonsterIdSorted, monsterId);
    } else {
        monsterGenAddMonster(mg);
    }
}

void monsterGenSpawnOne(MonsterGen* mg) {
    monsterGen_activate_or_add_one(mg);
}

void monsterGenUpdate(MonsterGen* mg, Arena* arena, Terminal* terminal, float delta_t) {
    mg->totalTimeElapsed+=delta_t;
    timerUpdate(mg->genTimer,delta_t);
    int n_monster_to_render = mg->genTimeFunc(mg->totalTimeElapsed,mg->genTimer);
     
     

    mg->numberOfInstantFinishedMonsters = 0;

    for (unsigned int i = 0; i < vectorGetSize(mg->monsterList); i++) {
        Monster* m = *(Monster**)vectorGet(mg->monsterList, i);
        monsterUpdate(m, arena, terminal, delta_t);
        bool m_in_stack = vectorGetVal(mg->deactivatedMonsterIdList, i);

        if ((monsterIsDead(m) || monsterGetFinishedPath(m)) && !m_in_stack) {
            vectorSetVal(mg->deactivatedMonsterIdList, i, true);
            vectorPushBack(mg->availableMonsterIdStack, (char*)&i);

            if (!(monsterIsDead(m))) {
                mg->numberOfInstantFinishedMonsters++;
            }

            if (arena != NULL && monsterIsDead(m)) {
                BoxCollider* bc = monsterGetBoxCollider(m);
                float ax = boxColliderGetX(bc) + boxColliderGetOffsetX(bc);
                float ay = boxColliderGetY(bc) + boxColliderGetOffsetY(bc);
                Vec2d pos = {ax + boxColliderGetWidth(bc) * 0.5f - FRUIT_SIZE.x * 0.5f,
                             ay + boxColliderGetHeight(bc) * 0.5f - FRUIT_SIZE.y * 0.5f};
                arenaDeathFruit(arena, pos);
            }
        }
    }

    unsigned int new_size = 0;
    for (unsigned int i = 0; i < vectorGetSize(mg->activeMonsterIdSorted); i++) {
        int monsterId = vectorGetVal(mg->activeMonsterIdSorted, i);
        if (!vectorGetVal(mg->deactivatedMonsterIdList, monsterId)) {
            vectorSetVal(mg->activeMonsterIdSorted, new_size, monsterId);
            new_size++;
        }
    }
    unsigned int n_removes = vectorGetSize(mg->activeMonsterIdSorted) - new_size;

    for (unsigned int i = 0; i < n_removes; i++) {
        vectorPopBack(mg->activeMonsterIdSorted);
    }

    while (n_monster_to_render > 0) {
        monsterGen_activate_or_add_one(mg);
        n_monster_to_render--;
    }
}

int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex) {
    if (vectorGetSize(mg->activeMonsterIdSorted) == 0 || sortedIndex >= vectorGetSize(mg->activeMonsterIdSorted)) {
        return -1;
    }
    return vectorGetVal(mg->activeMonsterIdSorted, sortedIndex);
}

Monster* monsterGenGetMonster(MonsterGen* mg, unsigned int index) {
    return *(Monster**)vectorGet(mg->monsterList, index);
}

unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg) {

    return mg->numberOfInstantFinishedMonsters;
}

bool monsterGenBossDefeated(MonsterGen* bossGen) {
    if (bossGen == NULL) return false;
    if (vectorGetSize(bossGen->monsterList) == 0u) return false;
    Monster* m = *(Monster**)vectorGet(bossGen->monsterList, 0u);
    return monsterIsDead(m);
}

/*

*/
