#include "../include/player/playerState.h"
/** @file playerState.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

struct PlayerState {
    void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
    void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
    PlayerState* (*transition)(PlayerState* state, PlayerModel* model);
};

PlayerState* playerStateNew() {
	PlayerState* res = malloc(sizeof(PlayerState));
	return res;
}

void playerStateFree(PlayerState* state) {
	free(state);
}

void playerStateSetFunctions(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model)) {
	state->act = act;
	state->onEnter = onEnter;
	state->transition = transition;
}

void playerStateOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	state->onEnter(state, model, terminal, arena, deltaTime);
}

void playerStateAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	state->act(state, model, terminal, arena, deltaTime);
}

PlayerState* playerStateTransition(PlayerState* state, PlayerModel* model) { 
	return state->transition(state, model);
}

