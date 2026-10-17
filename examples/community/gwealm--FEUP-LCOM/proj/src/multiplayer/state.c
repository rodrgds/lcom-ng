#include "multiplayer/state.h"

#include "state/state.h"

#include "multiplayer/controller/multiplayer.h"

void (multiplayer_init)() {
  state_register(MULTIPLAYER, multiplayer_state_start, multiplayer_state_end);
}

void (multiplayer_cleanup)() {
  
}

void (multiplayer_state_start)() {
  multiplayer_start();
}

void (multiplayer_state_end)() {
  multiplayer_end();
}
