#include "../../include/monster/monsterGen.h"
/** @file waveFunctions.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

 

int progressiveTimeFunction50(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=50;
    float SecondsBetweenSpawns=8;

    for(unsigned int i=1;SecondsBetweenSpawns>0;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 

int progressiveTimeFunction100_1(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=100;
    float SecondsBetweenSpawns=32;

    for(unsigned int i=1;SecondsBetweenSpawns>0.2;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 

int progressiveTimeFunction100_2(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=100;
    float SecondsBetweenSpawns=32;

    for(unsigned int i=1;SecondsBetweenSpawns>0;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30+timeInterval){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 

int progressiveTimeFunction200_1(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=200;
    float SecondsBetweenSpawns=32;

    for(unsigned int i=1;SecondsBetweenSpawns>0.2;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 

int progressiveTimeFunction200_2(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=200;
    float SecondsBetweenSpawns=32;

    for(unsigned int i=1;SecondsBetweenSpawns>0;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30+timeInterval){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 

 
int singleAppear450(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=450;
    float SecondsBetweenSpawns=32;

    for(unsigned int i=1;SecondsBetweenSpawns>0;i++){
        if(totalTimeElapsed<timeInterval*i){
            timerSetInitialTime(genTimer,SecondsBetweenSpawns); 
            break;
        }
        SecondsBetweenSpawns/=2;
    }
    if(totalTimeElapsed<30+timeInterval){
        timerRestart(genTimer);
    }

    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
} 
