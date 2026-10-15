#include "GameController.h"

GameController *gameControllerCreate(int map_w, int map_h) {
    GameController *gc = malloc(sizeof(GameController));
    if (!gc) return NULL;
    gc->map_w = map_w;
    gc->map_h = map_h;
    return gc;
}

void gameControllerDestroy(GameController *gc) {
    free(gc);
}

void gameControllerUpdate(GameController *gc, Champion *p1, Champion *p2, Minions *minions,
                          Towers *towers, KeyboardState *kbd, MouseState *mouse, bool second_elapsed,
                          bool p1_looped, bool p2_looped) {
    if (!gc || !p1 || !p2 || !minions || !towers || !kbd || !mouse) return;

    championUpdate(p1, p1_looped, second_elapsed);
    championUpdate(p2, p2_looped, second_elapsed);

    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    if (kbd->p1.left) dx1--;
    if (kbd->p1.right) dx1++;
    if (kbd->p1.up) dy1--;
    if (kbd->p1.down) dy1++;
    if (kbd->p2.left) dx2--;
    if (kbd->p2.right) dx2++;
    if (kbd->p2.up) dy2--;
    if (kbd->p2.down) dy2++;

    championMove(p1, p2, towers, minions, dx1, dy1, gc->map_w, gc->map_h);
    championMove(p2, p1, towers, minions, dx2, dy2, gc->map_w, gc->map_h);

    if (mouse->p1.attack && championAttack(p1)) {
        if (championHits(p1, p2)) {
            championDecreaseHp(p1, p2, championGetAttackDmg(p1));
        }
        int p1_kills = minionsTakeHit(minions, championGetAttackBox(p1), championGetAttackDmg(p1), TEAM_BLUE);
        if (p1_kills > 0) championUpdatePoints(p1, p1_kills);
        towerTakeHit(&towers->red, championGetAttackBox(p1), championGetAttackDmg(p1));
    }
    if (mouse->p2.attack && championAttack(p2)) {
        if (championHits(p2, p1)) {
            championDecreaseHp(p2, p1, championGetAttackDmg(p2));
        }
        int p2_kills = minionsTakeHit(minions, championGetAttackBox(p2), championGetAttackDmg(p2), TEAM_RED);
        if (p2_kills > 0) championUpdatePoints(p2, p2_kills);
        towerTakeHit(&towers->blue, championGetAttackBox(p2), championGetAttackDmg(p2));
    }

    mouseClearEvents(mouse);
    minionsUpdate(minions, towers, p1, p2, gc->map_w, second_elapsed);
    towersUpdate(towers, minions, p1, p2);
}
