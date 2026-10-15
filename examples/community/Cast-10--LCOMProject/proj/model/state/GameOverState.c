#include "GameOverState.h"

#define BUTTON_W 220
#define BUTTON_H 60

#define RESTART_X 280
#define RESTART_Y 540

#define QUIT_X 525
#define QUIT_Y 540

GameOverState *gameOverStateCreate() {
    GameOverState *state = malloc(sizeof(GameOverState));
    if (!state) return NULL;

    state->restartButton = buttonStateCreate(RESTART_X, RESTART_Y, BUTTON_W, BUTTON_H);
    state->quitButton = buttonStateCreate(QUIT_X, QUIT_Y, BUTTON_W, BUTTON_H);

    if (!state->restartButton || !state->quitButton) {
        gameOverStateDestroy(state);
        return NULL;
    }

    return state;
}

void gameOverStateDestroy(GameOverState *state) {
    if (!state) return;

    if (state->restartButton) {
        buttonStateDestroy(state->restartButton);
    }

    if (state->quitButton) {
        buttonStateDestroy(state->quitButton);
    }

    free(state);
}
