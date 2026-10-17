#include "multiplayer.h"
#include "general.h"
int player = 1;

int getPlayer() {
	return player;
}
void changePlayer() {

	if (player == 1) {

		player = 2;
		setFlagColor(2);
	} else {
		player = 1;
		setFlagColor(1);
	}
}
