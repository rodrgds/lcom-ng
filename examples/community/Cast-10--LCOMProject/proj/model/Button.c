#include "Button.h"

ButtonState *buttonStateCreate() {
    ButtonState *button = malloc(sizeof(ButtonState));
    if (!button) return NULL;

    button->x = 0;
    button->y = 0;
    button->width = 0;
    button->height = 0;

    button->hovered = false;
    button->clicked = false;

    return button;
}

void buttonStateDestroy(ButtonState *button) {
    if (!button) return;

    free(button);
}

void buttonStateSetPosition(ButtonState *button, int x, int y) {
    if (!button) return;

    button->x = x;
    button->y = y;
}

void buttonStateSetSize(ButtonState *button, int width, int height) {
    if (!button) return;

    button->width = width;
    button->height = height;
}

bool buttonStateContains(ButtonState *button, int x, int y) {
    if (!button) return false;

    return x >= button->x &&
           x < button->x + button->width &&
           y >= button->y &&
           y < button->y + button->height;
}

void buttonStateResetClick(ButtonState *button) {
    if (!button) return;

    button->clicked = false;
}

