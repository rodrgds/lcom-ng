#include "../include/game.h"
/** @file game.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/gameState.h"

static GameState* state;

 
void startGame(Terminal* terminal) {
    state = mainMenuNew(terminal); 
}

void exitGame() {
    gameStateFree(state);
}

bool gameLoop(Terminal* terminal, float deltaTime) {
    gameStateUpdate(state, terminal, deltaTime);

    GameState* newState = gameStateTransition(state, terminal);
	if (!newState) return false;
    if (newState != state) {
        gameStateFree(state);
        state = newState;
    }

    return true;
}
