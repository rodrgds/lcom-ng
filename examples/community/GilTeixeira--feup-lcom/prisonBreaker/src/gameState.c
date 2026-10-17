#include "gameState.h"

GameState* initGameState() {

	GameState* gameState = (GameState*) malloc(sizeof(GameState));

	gameState->done = 0;

	gameState->game = initGame();

	gameState->currState = MAINMENU;

	gameState->LoseMenuBitmap = loadBitmap(
			"/home/lcom/lcom1718-t6g08/prisonBreaker/res/lose.bmp");
	gameState->MainMenuBitmap = loadBitmap(
			"/home/lcom/lcom1718-t6g08/prisonBreaker/res/menu.bmp");

	return gameState;

}

void displayGameState(GameState* gameState, Timer* timer) {

	switch (gameState->currState) {
	case MAINMENU:
		drawBitmap(gameState->MainMenuBitmap, 0, 0, ALIGN_LEFT);
		break;
	case LOSEMENU:
		drawBitmap(gameState->LoseMenuBitmap, 0, 0, ALIGN_LEFT);
		displayScore(gameState->game);
		break;
	case GAME:
		displayGame(gameState->game, timer);
		break;
	}
}

void gameStateUpdate(GameState* gameState, Timer* timer, Mouse* mouse) {

	switch (gameState->currState) {
	case MAINMENU:
		break;
	case LOSEMENU:
		break;
	case GAME:
		gameUpdate(gameState->game, timer, mouse);
		if (gameState->game->result == LOSE)
			gameState->currState = LOSEMENU;
		break;
	}
}

void gameStateUpdateKeyboard(GameState* gameState, unsigned long scancode,
		Timer* timer, Mouse* mouse) {

	switch (gameState->currState) {
	case MAINMENU:
		if (scancode == KEY_ESC)
			gameState->done = 1;

		else if (scancode == KEY_ENTER) {
			resetGame(gameState->game);
			resetMouse(mouse);
			resetTimer(timer);
			gameState->currState = GAME;
		}

		break;
	case LOSEMENU:
		if (scancode == KEY_ESC)
			gameState->currState = MAINMENU;

		else if (scancode == KEY_ENTER) {
			resetGame(gameState->game);
			resetMouse(mouse);
			resetTimer(timer);
			gameState->currState = GAME;
		}
		break;
	case GAME:
		gameUpdateKeyboard(gameState->game, scancode);
		break;
	}
}

void gameStateUpdateMouse(GameState* gameState, Mouse* mouse) {

	switch (gameState->currState) {
	case MAINMENU:
		break;
	case LOSEMENU:
		break;
	case GAME:
		gameUpdateMouse(gameState->game, mouse);
		if (gameState->game->result == LOSE)
			gameState->currState = LOSEMENU;
		break;
	}
}

void stopGameState(GameState* gameState) {
	deleteBitmap(gameState->LoseMenuBitmap);
	deleteBitmap(gameState->MainMenuBitmap);

	freeGame(gameState->game);

	free(gameState);
}
