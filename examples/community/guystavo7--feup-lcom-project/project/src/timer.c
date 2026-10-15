#include "../include/utils/timer.h"
/** @file timer.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

struct Timer {
    float initialTime;
    float counter;
    bool stopped;
};

Timer* timerNew(float time) {
    Timer* timer = malloc(sizeof(Timer));
    timer->initialTime = time;
    timer->counter = time;
    timer->stopped = true;

    return timer;
}

float timerGetInitialTime(Timer* timer) {
    return timer->initialTime;
}

void timerSetInitialTime(Timer* timer, float initialTime) {
    timer->initialTime = initialTime;
}

void timerFree(Timer* timer) {
    free(timer);
}

void timerStart(Timer* timer) {
    timer->stopped = false;
}

void timerRestart(Timer* timer) {
    timer->stopped = false;
    timer->counter = timer->initialTime;
}

void timerStop(Timer* timer) {
    timer->stopped = true;
}

bool timerIsRunning(Timer* timer) {
    return (timer->counter > 0) && (!timer->stopped);
}

void timerUpdate(Timer* timer, float deltaTime) {
    if (!timer->stopped) {
        timer->counter -= deltaTime;
    }
}

