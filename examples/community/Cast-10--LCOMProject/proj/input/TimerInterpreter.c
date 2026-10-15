#include "TimerInterpreter.h"

void timerStateInit(TimerState *state, int ticks_per_update) {
    state->ticks_per_update = ticks_per_update;
    state->tick_counter = 0;
    state->total_ticks = 0;
    state->should_update = false;
}

void timerInterpretInterrupt(TimerState *state) {
    state->total_ticks++;
    state->tick_counter++;
    state->should_update = (state->tick_counter >= state->ticks_per_update);
    if (state->should_update) state->tick_counter = 0;
    state->second_elapsed = (state->total_ticks % sys_hz() == 0);
}
