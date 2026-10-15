#include "../include/ui/statsBar.h"
/** @file statsBar.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../include/ui/numbers.h"
#include "../resources/ui/ui.h"

struct StatsBar {
    SpriteXPM* sprite;
    Number* banana;
    Number* avocado;
    Number* cherry;
    Number* carrot;
};

StatsBar* statsBarNew() {
    StatsBar* statsBar = malloc(sizeof(StatsBar));
    statsBar->banana = numberNew(99);
    statsBar->avocado = numberNew(99);
    statsBar->cherry = numberNew(99);
    statsBar->carrot = numberNew(99);

    statsBar->sprite = newSpriteXPM((xpm_map_t)uiXPM[STATS_BAR_INDEX], XPM_8_8_8, 0, 0);

    return statsBar;
}

void statsBarFree(StatsBar* statsBar) {
    freeSpriteXPM(statsBar->sprite);
    numberFree(statsBar->banana);
    numberFree(statsBar->avocado);
    numberFree(statsBar->cherry);
    numberFree(statsBar->carrot);
    free(statsBar);
}

void statsBarUpdate(StatsBar* statsBar, Terminal* terminal, Arena* arena) {
    drawSpriteXPM(statsBar->sprite, terminal, STATS_BAR_POSITION.x, STATS_BAR_POSITION.y, false);
    numberUpdate(statsBar->banana, BANANA_STATS_POSITION, terminal);
    numberUpdate(statsBar->avocado, AVOCADO_STATS_POSITION, terminal);
    numberUpdate(statsBar->cherry, CHERRY_STATS_POSITION, terminal);
    numberUpdate(statsBar->carrot, CARROT_STATS_POSITION, terminal);
}

int statsBarGetBanana(StatsBar* statsBar) {
    return numberGetNumber(statsBar->banana);
}

int statsBarGetAvocado(StatsBar* statsBar) {
    return numberGetNumber(statsBar->avocado);
}

int statsBarGetCherry(StatsBar* statsBar) {
    return numberGetNumber(statsBar->cherry);
}

int statsBarGetCarrot(StatsBar* statsBar) {
    return numberGetNumber(statsBar->carrot);
}

void statsBarSetBanana(StatsBar* statsBar, int n) {
    numberSetNumber(statsBar->banana, n);
}

void statsBarSetAvocado(StatsBar* statsBar, int n) {
    numberSetNumber(statsBar->avocado, n);
}

void statsBarSetCherry(StatsBar* statsBar, int n) {
    numberSetNumber(statsBar->cherry, n);
}

void statsBarSetCarrot(StatsBar* statsBar, int n) {
    numberSetNumber(statsBar->carrot, n);
}

void statsBarAddBanana(StatsBar* statsBar, int n) {
    n = numberGetNumber(statsBar->banana) + n;
    numberSetNumber(statsBar->banana, n);
}

void statsBarAddAvocado(StatsBar* statsBar, int n) {
    n = numberGetNumber(statsBar->avocado) + n;
    numberSetNumber(statsBar->avocado, n);
}

void statsBarAddCherry(StatsBar* statsBar, int n) {
    n = numberGetNumber(statsBar->cherry) + n;
    numberSetNumber(statsBar->cherry, n);
}

void statsBarAddCarrot(StatsBar* statsBar, int n) {
    n = numberGetNumber(statsBar->carrot) + n;
    numberSetNumber(statsBar->carrot, n);
}
