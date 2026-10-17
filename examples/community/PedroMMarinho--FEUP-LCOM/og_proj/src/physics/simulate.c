#include "simulate.h"
#include "events.h"
#include "evolve.h"
#include "resolve/resolver.h"
#include "utilities.h"

void processShot(Table *table) {
  Event shotEvent = {0, STICK_BALL, table->balls[0], NULL, NULL,NULL,-1};
  table->nextEvent = shotEvent;
}

bool updatePhysics(Table *table, double dt) {
  Event nextEvent = table->nextEvent;
  double elapsed = 0;
  while (nextEvent.type != INVALID) {

    if (elapsed + nextEvent.time < dt) {
      // Event still happens before dt passed
      evolveBalls(table, nextEvent.time);
      resolveEvent(table, nextEvent);
      elapsed += nextEvent.time;
    }
    else {
      evolveBalls(table, dt - elapsed);
      nextEvent.time -= dt - elapsed;
      table->nextEvent = nextEvent;
      return true;
    }
    nextEvent = getNextEvent(table);
    table->nextEvent = nextEvent;

  }

  
  return false;

}

void evolveBalls(Table *table, double time) {
  for (size_t i = 0; i < table->ballNumber; i++) {
    evolveBallMotion(table, table->balls[i], time);
  }
}
