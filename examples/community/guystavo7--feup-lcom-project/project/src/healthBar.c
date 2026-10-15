#include "../include/ui/healthBar.h"
/** @file healthBar.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../include/ui/numbers.h"
#include "../resources/ui/ui.h"

struct HealthBar {
    int health;

    SpriteXPM* sprite;
    Number* number;
};

HealthBar* healthBarNew(int health) {
    HealthBar* healthBar = malloc(sizeof(HealthBar));
    healthBar->health = health;
    healthBar->sprite = newSpriteXPM((xpm_map_t)uiXPM[HEALTH_BAR_INDEX], XPM_8_8_8, 0, 0);
    healthBar->number = numberNew(health);

    return healthBar;
}

void healthBarFree(HealthBar* healthBar) {
    freeSpriteXPM(healthBar->sprite);
    numberFree(healthBar->number);
    free(healthBar);
}

void healthBarUpdate(HealthBar* healthBar, Terminal* terminal, Arena* arena) {
    drawSpriteXPM(healthBar->sprite, terminal, HEALTH_BAR_POSITION.x, HEALTH_BAR_POSITION.y, false);
    numberUpdate(healthBar->number, HEALTH_VALUE_POSITION, terminal);
}

int healthBarGetHealth(HealthBar* healthBar) {
	return healthBar->health;
}

void healthBarSetHealth(HealthBar* healthBar, int health) {
	healthBar->health = health;
	numberSetNumber(healthBar->number, healthBar->health);
}

void healthBarAddHealth(HealthBar* healthBar, int health) {
	healthBar->health += health;
	numberSetNumber(healthBar->number, healthBar->health);
}


