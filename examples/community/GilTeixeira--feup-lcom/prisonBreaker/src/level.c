#include "level.h"

void displayLevel(Level* level) {
	drawBitmap(level->instructionBitmap, 0, 0, ALIGN_LEFT);
}

short isAcceptedDirection(Level* level, short direction) {
	int i;
	for (i = 0; i < level->numAcceptedDirections; i++) {
		if (level->acceptedDirections[i] == direction)
			return 1;
	}
	return 0;
}

void freeLevel(Level* level) {
	free(level->acceptedDirections);
	deleteBitmap(level->instructionBitmap);
	free(level);

}
