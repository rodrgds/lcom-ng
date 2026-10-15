#include "MenuState.h"

#include <stdlib.h>

#define PLAY_BUTTON_X 250
#define PLAY_BUTTON_Y 350
#define PLAY_BUTTON_W 96
#define PLAY_BUTTON_H 32

MenuState *menuStateCreate() {
    MenuState *menu = malloc(sizeof(MenuState));
    if (!menu) return NULL;

    menu->playButton = buttonStateCreate(
        PLAY_BUTTON_X,
        PLAY_BUTTON_Y,
        PLAY_BUTTON_W,
        PLAY_BUTTON_H
    );

    if (!menu->playButton) {
        free(menu);
        return NULL;
    }

    return menu;
}

void menuStateDestroy(MenuState *menu) {
    if (!menu) return;

    if (menu->playButton) {
        buttonStateDestroy(menu->playButton);
    }

    free(menu);
}

bool menuStateShouldStart(MenuState *menu) {
    if (!menu || !menu->playButton) return false;
    return menu->playButton->clicked;
}

void menuStateReset(MenuState *menu) {
    if (!menu || !menu->playButton) return;
    menu->playButton->clicked = false;
}
