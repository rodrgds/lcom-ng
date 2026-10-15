#include "Minion.h"

void minionsInit(Minions *minions) {
    for (int i = 0; i < MAX_WAVES; i++) {
        minions->red_waves[i].active = false;
        minions->blue_waves[i].active = false;
    }
    minions->red_count = 0;
    minions->blue_count = 0;
    minions->spawn_timer = SPAWN_SECONDS;
}
