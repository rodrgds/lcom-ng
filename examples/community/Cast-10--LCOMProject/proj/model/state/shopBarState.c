#include "shopBarState.h"

#define SHOP_BAR_W 250
#define SHOP_BAR_H 40

#define BUY_BUTTON_W 80
#define BUY_BUTTON_H 40

ShopBarState *shopBarStateCreate(int x, int y, UpgradeType upgrade_type) {
    ShopBarState *bar = malloc(sizeof(ShopBarState));
    if (!bar) return NULL;

    bar->x = x;
    bar->y = y;
    bar->width = SHOP_BAR_W;
    bar->height = SHOP_BAR_H;

    bar->level = 0;
    bar->upgrade_type = upgrade_type;

    bar->buyButton = buttonStateCreate(
        x + SHOP_BAR_W + 15,
        y,
        BUY_BUTTON_W,
        BUY_BUTTON_H
    );

    if (!bar->buyButton) {
        free(bar);
        return NULL;
    }

    return bar;
}

void shopBarStateDestroy(ShopBarState *bar) {
    if (!bar) return;

    if (bar->buyButton) {
        buttonStateDestroy(bar->buyButton);
    }

    free(bar);
}

bool shopBarStateIsMaxed(ShopBarState *bar) {
    if (!bar) return false;
    return bar->level >= SHOP_BAR_MAX_LEVEL;
}

void shopBarStateBuy(ShopBarState *bar) {
    if (!bar) return;

    if (bar->level < SHOP_BAR_MAX_LEVEL) {
        bar->level++;
    }
}

void shopBarStateResetClick(ShopBarState *bar) {
    if (!bar || !bar->buyButton) return;

    bar->buyButton->clicked = false;
}

