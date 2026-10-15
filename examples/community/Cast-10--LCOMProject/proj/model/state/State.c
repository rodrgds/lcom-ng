#include "State.h"

bool stateInit(State *state) {
    if (state == NULL) return false;

    state->state = STATE_MENU;
    state->running = true;
    state->winner = 0;

    state->game = NULL;
    state->menu = NULL;
    state->shop = NULL;
    state->gameOver = NULL;

    state->game = gameStateCreate();
    if (state->game == NULL) {
        return false;
    }

    state->menu = menuStateCreate();
    if (state->menu == NULL) {
        stateDestroy(state);
        return false;
    }

    state->shop = shopStateCreate(
        40,    // player1X
        590,   // player2X

        520,   // attackY
        585,   // hpY
        650,   // speedY

        300,   // barWidth
        35,    // barHeight

        80,    // buyButtonWidth
        35,    // buyButtonHeight

        15     // buyButtonGap
    );

    if (state->shop == NULL) {
        stateDestroy(state);
        return false;
    }

    state->gameOver = gameOverStateCreate();
    if (state->gameOver == NULL) {
        stateDestroy(state);
        return false;
    }

    return true;
}

void stateDestroy(State *state) {
    if (state == NULL) return;

    if (state->gameOver != NULL) {
        gameOverStateDestroy(state->gameOver);
        state->gameOver = NULL;
    }

    if (state->shop != NULL) {
        shopStateDestroy(state->shop);
        state->shop = NULL;
    }

    if (state->menu != NULL) {
        menuStateDestroy(state->menu);
        state->menu = NULL;
    }

    if (state->game != NULL) {
        gameStateDestroy(state->game);
        state->game = NULL;
    }
}

void gameStart(State *state) {
    if (state == NULL) return;

    state->state = STATE_PLAYING;
    state->winner = 0;
}

void gamePause(State *state) {
    if (state == NULL) return;

    state->state = STATE_PAUSED;
}

void gameResume(State *state) {
    if (state == NULL) return;

    state->state = STATE_PLAYING;
}

void gameOver(State *state, int winner) {
    if (state == NULL) return;

    state->state = STATE_GAME_OVER;
    state->winner = winner;
}

void gameQuit(State *state) {
    if (state == NULL) return;

    state->running = false;
}

void gameRestart(State *state) {
    if (state == NULL) return;

    gameStateDestroy(state->game);
    state->game = gameStateCreate();
    if (state->game == NULL) {
        state->running = false;
        return;
    }

    state->winner = 0;
    state->state = STATE_PLAYING;
}

