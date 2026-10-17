#include "battleship.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core.h"
/*********************************************************
 ____ _  _ _ ___
 [__  |__| | |__]
 ___] |  | | |

 **********************************************************/

/* Built a ship and return if ok */
int ship_build(struct ship *pShip, char type, int positions[5][3]) {
	int health, i;
	/* (1) Check type of ship */
	switch (type) {
	case 'a':
		health = 5;
		break;
	case 'b':
		health = 4;
		break;
	case 's':
		health = 3;
		break;
	case 'c':
		health = 3;
		break;
	case 'd':
		health = 2;
		break;
	default:
		return 0;
	}
	pShip->health = health;
	pShip->sizeShip = health;
	pShip->type = type;
	for (i = 0; i < 5; i++) {
		pShip->positions[i][0] = positions[i][0];
		pShip->positions[i][1] = positions[i][1];
		pShip->positions[i][2] = 0;
	}
	return 1;
}
/* Do an hit */
int ship_hit(struct ship *pShip, int x, int y) {
	int i, status;
	for (i = 0; i < pShip->sizeShip; i++) {
		if (pShip->positions[i][0] == x && pShip->positions[i][1] == y) {
			if (pShip->positions[i][2])
				return -1;
			pShip->positions[i][2] = 1;
			pShip->health--;
			/* Determine the type of return code */
			if (pShip->health == 0)
				status = 20;
			else
				status = 10;
			/* Determine type */
			switch (pShip->type) {
			case 'a':
				status += 1;
				break;
			case 'b':
				status += 2;
				break;
			case 's':
				status += 3;
				break;
			case 'c':
				status += 4;
				break;
			case 'd':
				status += 5;
				break;
			default:
				return 0;
			}
			return status;
		}
	}
	return 0;
}
/* Check if is destroyed */
int ship_destroyed(struct ship *pShip) {
	return !pShip->health;
}

void ship_print(struct ship *pShip) {
	int i;
	printf("\n");
	if (!pShip->health)
		printf("destroyed");
	else {
		for (i = 0; i < pShip->sizeShip; i++)
			if (pShip->positions[i][0] != -1)
				printf("%d,%d\t", pShip->positions[i][0],
						pShip->positions[i][1]);
	}
}

/*********************************************************
 ____ ____ _  _ ____
 | __ |__| |\/| |___
 |__] |  | |  | |___

 **********************************************************/
int game_build(struct game *pGame) {
	int i, n, vert, shipSize, x, y, reset, positions[5][5][3], cont;
	char type;
	srand((unsigned int) time(NULL));
	/* Fill the map with dots */
	for (i = 0; i < 10; i++)
		for (n = 0; n < 10; n++) {
			pGame->map[i][n] = '.';
			pGame->mapOpponent[i][n] = '.';
		}
	/* Generate the 5 ships with random orientation */
	for (i = 0; i < 5; i++) {
		reset = 0;
		switch (i) {
		case 0:
			shipSize = 5;
			type = 'a';
			break;
		case 1:
			shipSize = 4;
			type = 'b';
			break;
		case 2:
			shipSize = 3;
			type = 's';
			break;
		case 3:
			shipSize = 3;
			type = 'c';
			break;
		case 4:
			shipSize = 2;
			type = 'd';
			break;
		}
		vert = rand() % 2;
		if (vert) {
			x = rand() % 10;
			y = rand() % (10 - shipSize);
			cont = 0;
			for (n = y; n < y + shipSize; n++) {
				if (pGame->map[x][n] != '.') {
					i--;
					reset = 1;
					while (cont) {
						pGame->map[x][n - cont] = '.';
						cont--;
					}
					break;
				} else {
					pGame->map[x][n] = type;
					positions[i][cont][0] = x;
					positions[i][cont][1] = n;
					cont++;
				}
			}
		} else {
			x = rand() % (10 - shipSize);
			y = rand() % 10;
			cont = 0;
			for (n = x; n < x + shipSize; n++) {
				if (pGame->map[n][y] != '.') {
					i--;
					reset = 1;
					while (cont) {
						pGame->map[n - cont][y] = '.';
						cont--;
					}
					break;
				} else {
					pGame->map[n][y] = type;
					positions[i][cont][0] = n;
					positions[i][cont][1] = y;
					cont++;
				}
			}
		}
		if (!reset) {
			ship_build(&pGame->ships[i], type, positions[i]);
		}
	}
	return 1;
}

void game_printMap(struct game *pGame) {
	int i, n;
	printf("\n  0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < 10; i++) {
		printf("%d ", i);
		for (n = 0; n < 10; n++) {
			printf("%c ", pGame->map[n][i]);
		}
		printf("\n");
	}
}

void game_printOpMap(struct game *pGame) {
	int i, n;
	printf("\n  0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < 10; i++) {
		printf("%d ", i);
		for (n = 0; n < 10; n++) {
			printf("%c ", pGame->mapOpponent[n][i]);
		}
		printf("\n");
	}
}

void game_hit(struct game *pGame, int x, int y) {
	int i, hit_code = 0, boxX=654, boxY;
	for (i = 0; i < 5; i++) {
		hit_code = ship_hit(&pGame->ships[i], x, y);
		if (hit_code > 10 || hit_code==-1) {
			break;
		}
	}
	if (hit_code == -1)
		return;
	attempts++;
	if (hit_code == 0) {
		printf("WATER!\n");
		pGame->mapOpponent[x][y] = 'o';
		i_drawHit(x, y, 0);
		write_xpm(water_xpm, 625, 125);
		return;
	}
	pGame->map[x][y] = 'x';
	pGame->mapOpponent[x][y] = 'x';
	i_drawHit(x, y, 1);
	switch (hit_code) {
	case 11:
		printf("SHOT AIRCRAFT CARRIER!\n");
		boxY=239;
		break;
	case 12:
		printf("SHOT BATTLESHIP!\n");
		boxY=319;
		break;
	case 13:
		printf("SHOT SUBMARINE!\n");
		boxY=379;
		break;
	case 14:
		printf("SHOT CRUISER!\n");
		boxY=439;
		break;
	case 15:
		printf("SHOT DESTROYER!\n");
		boxY=519;
		break;
	case 21:
		printf("SUNK AIRCRAFT CARRIER!\n");
		boxY=239;
		break;
	case 22:
		printf("SUNK BATTLESHIP!\n");
		boxY=319;
		break;
	case 23:
		printf("SUNK SUBMARINE!\n");
		boxY=379;
		break;
	case 24:
		printf("SUNK CRUISER!\n");
		boxY=439;
		break;
	case 25:
		printf("SUNK DESTROYER!\n");
		boxY=519;
		break;
	}
	if(hit_code>20)
		write_xpm(sunk_xpm, 625, 125);
	else
		write_xpm(hit_xpm, 625, 125);
	i_printNumber(pGame->ships[i].health, boxX, boxY);

}

int game_isOver(struct game *pGame) {
	int retCode = 1, i;
	for (i = 0; i < 5; i++) {
		if (!ship_destroyed(&pGame->ships[i])) {
			retCode = 0;
			break;
		}
	}
	return retCode;
}
