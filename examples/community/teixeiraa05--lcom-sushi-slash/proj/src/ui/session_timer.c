#include "../../include/ui/session_timer.h"
#include <stdio.h>

#define SESSION_TIMER_TICKS_PER_SECOND 60

void session_timer_reset(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    timer->elapsed_ticks = 0;
    timer->last_tick = current_tick;
    timer->running = false;
}

void session_timer_start(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    timer->last_tick = current_tick;
    timer->running = true;
}

void session_timer_pause(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL) return;

    session_timer_update(timer, current_tick);
    timer->running = false;
}

void session_timer_update(GameSessionTimer *timer, int current_tick) {
    if (timer == NULL || !timer->running) return;

    if (current_tick > timer->last_tick) {
        timer->elapsed_ticks += (uint32_t)(current_tick - timer->last_tick);
    }

    timer->last_tick = current_tick;
}

uint32_t session_timer_ticks(const GameSessionTimer *timer) {
    if (timer == NULL) return 0;

    return timer->elapsed_ticks;
}

uint32_t session_timer_seconds(const GameSessionTimer *timer) {
    if (timer == NULL) return 0;

    return timer->elapsed_ticks / SESSION_TIMER_TICKS_PER_SECOND;
}
