#include "GameViewer.h"
#include "sprite/BuildSprites.h"
#include "../assets/game/minion_p1/minion_p1_xpms.h"
#include "../assets/game/minion_p2/minion_p2_xpms.h"
#include "../assets/game/tower/tower_p1_xpm.h"
#include "../assets/game/tower/tower_p2_xpm.h"

#include <stdlib.h>

#define HUD_SCALE   2
#define HUD_SPACING 1

GameViewer *gameViewerCreate() {
    GameViewer *gv = malloc(sizeof(GameViewer));
    if (!gv) return NULL;

    gv->p1 = NULL;
    gv->p2 = NULL;
    gv->minion_blue = NULL;
    gv->minion_red = NULL;
    gv->tower = NULL;

    AnimSprite **anim1 = buildAnimations(1);
    AnimSprite **anim2 = buildAnimations(2);

    if (!anim1 || !anim2) {
        if (anim1) {
            for (int i = 0; i < NUM_ANIMS; i++) {
                if (anim1[i]) destroyAnimSprite(anim1[i]);
            }
            free(anim1);
        }

        if (anim2) {
            for (int i = 0; i < NUM_ANIMS; i++) {
                if (anim2[i]) destroyAnimSprite(anim2[i]);
            }
            free(anim2);
        }

        free(gv);
        return NULL;
    }

    gv->p1 = createChampionViewer(anim1, NUM_ANIMS);
    gv->p2 = createChampionViewer(anim2, NUM_ANIMS);

    gv->minion_blue = createMinionViewer(
        (xpm_row_t *) minion_p1_stop,
        (xpm_row_t *) minion_p1_attack
    );

    gv->minion_red = createMinionViewer(
        (xpm_row_t *) minion_p2_stop,
        (xpm_row_t *) minion_p2_attack
    );

    gv->tower = createTowerViewer(
        (xpm_map_t) blue_tower_xpm,
        (xpm_map_t) red_tower_xpm
    );

    if (!gv->p1 || !gv->p2 || !gv->minion_blue || !gv->minion_red || !gv->tower) {
        gameViewerDestroy(gv);
        return NULL;
    }

    return gv;
}

void gameViewerDestroy(GameViewer *gv) {
    if (!gv) return;

    if (gv->p1) destroyChampionViewer(gv->p1);
    if (gv->p2) destroyChampionViewer(gv->p2);
    if (gv->minion_blue) destroyMinionViewer(gv->minion_blue);
    if (gv->minion_red) destroyMinionViewer(gv->minion_red);
    if (gv->tower) destroyTowerViewer(gv->tower);

    free(gv);
}

void gameViewerUpdate(GameViewer *gv, Champion *p1, Champion *p2, Minions *minions,
                      bool *p1_looped, bool *p2_looped) {
    if (!gv || !p1 || !p2 || !minions) return;

    *p1_looped = championViewerUpdate(gv->p1, p1);
    *p2_looped = championViewerUpdate(gv->p2, p2);

    minionViewerUpdate(gv->minion_blue);
    minionViewerUpdate(gv->minion_red);
}

void gameViewerDraw(GameViewer *gv, Champion *p1, Champion *p2, Minions *minions, Towers *towers) {
    if (!gv || !p1 || !p2 || !minions || !towers) return;

    towerViewerDraw(gv->tower, towers);
    minionViewerDraw(gv->minion_blue, minions, TEAM_BLUE);
    minionViewerDraw(gv->minion_red, minions, TEAM_RED);
    championDraw(gv->p1, p1);
    championDraw(gv->p2, p2);
}
