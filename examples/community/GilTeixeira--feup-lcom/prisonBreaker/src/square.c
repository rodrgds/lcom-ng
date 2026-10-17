#include "square.h"
#include "game.h"

Square* initSquare() {

	Square* square = (Square*) malloc(sizeof(Square));

	square->direction = NOTHING_DIR;
	square->bitmap = 0;
	square->deslocX = 0;
	square->deslocY = 0;

	square->bitmap = loadBitmap(
			"/home/lcom/lcom1718-t6g08/prisonBreaker/res/square.bmp");

	return square;
}

void updateSquare(Square* square) {

	switch (square->direction) {

	case LEFT_DIR:
		square->deslocX -= SQUARE_DELTA_MOVE;
		break;

	case RIGHT_DIR:
		square->deslocX += SQUARE_DELTA_MOVE;
		break;

	case UP_DIR:
		square->deslocY -= SQUARE_DELTA_MOVE;
		break;
	case DOWN_DIR:
		square->deslocY += SQUARE_DELTA_MOVE;
		break;

	}

	if (square->deslocX > SQUARE_MAX_MOVE
			|| square->deslocY > SQUARE_MAX_MOVE
			|| square->deslocX < -SQUARE_MAX_MOVE
			|| square->deslocY < -SQUARE_MAX_MOVE) {
		square->deslocX *= -1;
		square->deslocY *= -1;
	}
}

short hasFinishedMovement(Square* square) {
	if (square->deslocX == 0 && square->deslocY == 0) {
		square->direction = NOTHING_DIR;
		return 1;
	}
	return 0;
}

void displaySquare(Square* square) {
	drawBitmap(square->bitmap, SQUARE_X_COORD + square->deslocX,
	SQUARE_Y_COORD + square->deslocY, ALIGN_LEFT);
}

void freeSquare(Square* square) {
	deleteBitmap(square->bitmap);
	free(square);
}
