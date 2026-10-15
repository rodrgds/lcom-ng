#include "GameState.h"

GameState *gameStateCreate() {
    GameState *gs = malloc(sizeof(GameState));
    if (!gs) return NULL;

    gs->p1 = createChampion(BLUE_SPAWN, EAST, CHAMPION_W, CHAMPION_H);
    gs->p2 = createChampion(RED_SPAWN, WEST, CHAMPION_W, CHAMPION_H);

    if (!gs->p1 || !gs->p2) {
        if (gs->p1) destroyChampion(gs->p1);
        if (gs->p2) destroyChampion(gs->p2);
        free(gs);
        return NULL;
    }

    minionsInit(&gs->minions);
    minionsControllerInit(&gs->minions, MODE105_WIDTH);
    towersInit(&gs->towers, MODE105_WIDTH);

    return gs;
}

void gameStateDestroy(GameState *gs) {
    if (!gs) return;

    if (gs->p1) destroyChampion(gs->p1);
    if (gs->p2) destroyChampion(gs->p2);

    free(gs);
}
