#include "../include/player/player.h"
/** @file player.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

#include <stdlib.h>

struct Player {
	PlayerController* controller;
	PlayerModel* model;
};

Player* playerNew(Vec2d pos) {
	PlayerModel* model = playerModelNew(pos);
	PlayerController* controller = playerControllerNew(model);

	Player* res = malloc(sizeof(Player));
	res->model = model;
	res->controller = controller;

	return res;
}

void playerFree(Player* player) {
	playerModelFree(player->model);
	playerControllerFree(player->controller);
	free(player);
}

void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime) {
	playerControllerUpdate(player->controller, terminal, arena, deltaTime);
}

PlayerModel* playerGetPlayerModel(Player* player) {
	return player->model;
}

PlayerController* playerGetPlayerController(Player* player) {
	return player->controller;
}
