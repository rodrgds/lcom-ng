#include "./../include/player/playerIdle.h"
/** @file playerIdle.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "./../include/player/playerRunning.h"

void idleAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	playerModelSetVelocityX(model, 0);
	playerModelSetVelocityY(model, 0);
    rigidBodySetVelocity(playerModelGetRigidBody(model), 0, 0);
}

PlayerState* idleTransition(PlayerState* state, PlayerModel* model) {
	Vec2d dir = {playerModelGetDirX(model), playerModelGetDirY(model)};
	if (dir.x != 0 || dir.y != 0) return getRunningState();
    return state;
}

void idleOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	playerModelSetAnimatedSprite(model, playerModelGetIdleSprite(model));
}

PlayerState* getIdleState() {
	PlayerState* res = playerStateNew();
	playerStateSetFunctions(res, idleAct, idleOnEnter, idleTransition);
	return res;
}
