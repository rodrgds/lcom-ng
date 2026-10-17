#include "fruit.h"

unsigned int time_until_spawn = 100; // Time between new fruit spawns
struct Fruit fruit_map[20]; // Array of fruit coordinates

void spawnFruit(void* background_game, uint8_t* xpm_game_background_adr, uint8_t* apple_adr, xpm_image_t img_game_background, xpm_image_t img_apple)
{
	struct Fruit f1;

	clear_buffer();

	draw_xpm(xpm_game_background_adr, img_game_background, 0, 0);

	// Fruit spawn setup

	for (int i = 0; i < 20; i++)
	{
		f1.x_coordinate = (rand() % (50 - 1 - 1)) + 1;
		f1.y_coordinate = (rand() % (40 - 1 - 1)) + 1;

		fruit_map[i] = f1;
	}

	// Fruit internal colision

	for (int j = 0; j < 20; j++) {
		for (int k = j + 1; k < 20; k++) {
			if (((fruit_map[k].x_coordinate >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;

			}
			if (((fruit_map[k].x_coordinate + 2 >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate + 2 <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate + 2 >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate + 2 <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
			}
			if ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y)) {
				do {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				} while ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y));
			}

			if ((getNewSnake().snake_dir == LEFT) || getNewSnake().snake_dir == RIGHT) {
				if ((fruit_map[k].x_coordinate == getNewSnake().x + 1) || (fruit_map[k].x_coordinate == getNewSnake().x + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
			else if ((getNewSnake().snake_dir == UP) || getNewSnake().snake_dir == DOWN) {
				if ((fruit_map[k].y_coordinate == getNewSnake().y + 1) || (fruit_map[k].y_coordinate == getNewSnake().y + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
		}
	}

	// Doing it twice to reduce colision probability

	for (int j = 0; j < 20; j++) {
		for (int k = j + 1; k < 20; k++) {
			if (((fruit_map[k].x_coordinate >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;

			}
			if (((fruit_map[k].x_coordinate + 2 >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate + 2 <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate + 2 >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate + 2 <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
			}
			if ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y)) {
				do {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				} while ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y));
			}

			if ((getNewSnake().snake_dir == LEFT) || getNewSnake().snake_dir == RIGHT) {
				if ((fruit_map[k].x_coordinate == getNewSnake().x + 1) || (fruit_map[k].x_coordinate == getNewSnake().x + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
			else if ((getNewSnake().snake_dir == UP) || getNewSnake().snake_dir == DOWN) {
				if ((fruit_map[k].y_coordinate == getNewSnake().y + 1) || (fruit_map[k].y_coordinate == getNewSnake().y + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
		}
	}

	// Doing it three times to make it impossible


	for (int j = 0; j < 20; j++) {
		for (int k = j + 1; k < 20; k++) {
			if (((fruit_map[k].x_coordinate >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;

			}
			if (((fruit_map[k].x_coordinate + 2 >= fruit_map[j].x_coordinate) && (fruit_map[k].x_coordinate + 2 <= fruit_map[j].x_coordinate + 2)) && (((fruit_map[k].y_coordinate + 2 >= fruit_map[j].y_coordinate) && (fruit_map[k].y_coordinate + 2 <= fruit_map[j].y_coordinate + 2)))) {
				fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
			}
			if ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y)) {
				do {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				} while ((fruit_map[k].x_coordinate == getNewSnake().x) && (fruit_map[k].y_coordinate == getNewSnake().y));
			}

			if ((getNewSnake().snake_dir == LEFT) || getNewSnake().snake_dir == RIGHT) {
				if ((fruit_map[k].x_coordinate == getNewSnake().x + 1) || (fruit_map[k].x_coordinate == getNewSnake().x + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
			else if ((getNewSnake().snake_dir == UP) || getNewSnake().snake_dir == DOWN) {
				if ((fruit_map[k].y_coordinate == getNewSnake().y + 1) || (fruit_map[k].y_coordinate == getNewSnake().y + 2)) {
					fruit_map[k].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
					fruit_map[k].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				}
			}
		}
	}

	// Fruit drawing

	for (int z = 0; z < 20; z++)
	{
		draw_xpm(apple_adr, img_apple, fruit_map[z].x_coordinate * 25, fruit_map[z].y_coordinate * 25);
		memcpy(background_game, getBuffer(), getSize());
	}
}

bool checkColision() {
	for (int i = 0; i < 20; i++)
	{
		if ((fruit_map[i].x_coordinate == getNewSnake().x) && (fruit_map[i].y_coordinate == getNewSnake().y)) {
			return true;
		}
	}

	return false;
}
