#include "core.h"
#include <stdio.h>
#include <stdlib.h>

#define HRES 800
#define VRES 600

char *read_xpm(char *map[], int *wd, int *ht) {

	int width, height, colors;
	char sym[256];
	int col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors*/
	if (sscanf(map[0], "%d %d %d", &width, &height, &colors) != 3) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}
#ifdef DEBUG
	printf("%d %d %d\n", width, height, colors);
#endif
	if (width > HRES || height > VRES || colors > 256) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i = 0; i < 256; i++)
		sym[i] = 0;

	/* read symbols <-> colors*/
	for (i = 0; i < colors; i++) {
		if (sscanf(map[i + 1], "%c %d", &symbol, &col) != 2) {
			printf("read_xpm: incorrect symbol, color at line %d\n", i + 1);
			return NULL;
		}
#ifdef DEBUG
		printf("%c %d\n", symbol, col);
#endif
		if (col > 256) {
			printf("read_xpm: incorrect color at line %d\n", i + 1);
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width * height);

	/* parse each pixmap symbol line */
	for (i = 0; i < height; i++) {
		line = map[colors + 1 + i];
#ifdef DEBUG
		printf("\nparsing %s\n", line);
#endif
		for (j = 0; j < width; j++) {
			tmp = memchr(sym, line[j], 256); /* find color of each symbol*/
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n",
						colors + i + 1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-)*/
#ifdef DEBUG
			printf("%c:%d ", line[j], tmp-sym);
#endif
		}
	}

	return pix;
}

void write_xpm(char *map[], int x, int y) {
	int w, h, i, n;
	char* pic;
	pic = read_xpm(map, &w, &h);
	for (i = 0; i < h; i++) {
		for (n = 0; n < w; n++) {
			vg_set_pixel(n + x, i + y, pic[w * i + n]);
		}
	}
}

int aloc_xpm() {
	int i;
	FILE *file;
	file = fopen("/usr/src/drivers/batalha_naval/res/newgame.xpm", "r");
	for (i = 0; i < 605; i++) {
		battleship1_xpm[i] = malloc(802 * sizeof(char));
		fgets(battleship1_xpm[i], 802, file);
		if (battleship1_xpm[i][strlen(battleship1_xpm[i]) - 1] == '\n') {
			battleship1_xpm[i][strlen(battleship1_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);
	file = fopen("/usr/src/drivers/batalha_naval/res/exit.xpm", "r");
	for (i = 0; i < 605; i++) {
		battleship2_xpm[i] = malloc(802 * sizeof(char));
		fgets(battleship2_xpm[i], 802, file);
		if (battleship2_xpm[i][strlen(battleship2_xpm[i]) - 1] == '\n') {
			battleship2_xpm[i][strlen(battleship2_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/mapa.xpm", "r");
	for (i = 0; i < 603; i++) {
		map_xpm[i] = malloc(802 * sizeof(char));
		fgets(map_xpm[i], 802, file);
		if (map_xpm[i][strlen(map_xpm[i]) - 1] == '\n') {
			map_xpm[i][strlen(map_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/won.xpm", "r");
	for (i = 0; i < 603; i++) {
		won_xpm[i] = malloc(802 * sizeof(char));
		fgets(won_xpm[i], 802, file);
		if (won_xpm[i][strlen(won_xpm[i]) - 1] == '\n') {
			won_xpm[i][strlen(won_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/0.xpm", "r");
	for (i = 0; i < 31; i++) {
		zero_xpm[i] = malloc(29 * sizeof(char));
		fgets(zero_xpm[i], 29, file);
		if (zero_xpm[i][strlen(zero_xpm[i]) - 1] == '\n') {
			zero_xpm[i][strlen(zero_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/1.xpm", "r");
	for (i = 0; i < 31; i++) {
		one_xpm[i] = malloc(29 * sizeof(char));
		fgets(one_xpm[i], 29, file);
		if (one_xpm[i][strlen(one_xpm[i]) - 1] == '\n') {
			one_xpm[i][strlen(one_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/2.xpm", "r");
	for (i = 0; i < 31; i++) {
		two_xpm[i] = malloc(29 * sizeof(char));
		fgets(two_xpm[i], 29, file);
		if (two_xpm[i][strlen(two_xpm[i]) - 1] == '\n') {
			two_xpm[i][strlen(two_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/3.xpm", "r");
	for (i = 0; i < 31; i++) {
		three_xpm[i] = malloc(29 * sizeof(char));
		fgets(three_xpm[i], 29, file);
		if (three_xpm[i][strlen(three_xpm[i]) - 1] == '\n') {
			three_xpm[i][strlen(three_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/4.xpm", "r");
	for (i = 0; i < 31; i++) {
		four_xpm[i] = malloc(29 * sizeof(char));
		fgets(four_xpm[i], 29, file);
		if (four_xpm[i][strlen(four_xpm[i]) - 1] == '\n') {
			four_xpm[i][strlen(four_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/5.xpm", "r");
	for (i = 0; i < 31; i++) {
		five_xpm[i] = malloc(29 * sizeof(char));
		fgets(five_xpm[i], 29, file);
		if (five_xpm[i][strlen(five_xpm[i]) - 1] == '\n') {
			five_xpm[i][strlen(five_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/6.xpm", "r");
	for (i = 0; i < 31; i++) {
		six_xpm[i] = malloc(29 * sizeof(char));
		fgets(six_xpm[i], 29, file);
		if (six_xpm[i][strlen(six_xpm[i]) - 1] == '\n') {
			six_xpm[i][strlen(six_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/7.xpm", "r");
	for (i = 0; i < 31; i++) {
		seven_xpm[i] = malloc(29 * sizeof(char));
		fgets(seven_xpm[i], 29, file);
		if (seven_xpm[i][strlen(seven_xpm[i]) - 1] == '\n') {
			seven_xpm[i][strlen(seven_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/8.xpm", "r");
	for (i = 0; i < 31; i++) {
		eight_xpm[i] = malloc(29 * sizeof(char));
		fgets(eight_xpm[i], 29, file);
		if (eight_xpm[i][strlen(eight_xpm[i]) - 1] == '\n') {
			eight_xpm[i][strlen(eight_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/9.xpm", "r");
	for (i = 0; i < 31; i++) {
		nine_xpm[i] = malloc(29 * sizeof(char));
		fgets(nine_xpm[i], 29, file);
		if (nine_xpm[i][strlen(nine_xpm[i]) - 1] == '\n') {
			nine_xpm[i][strlen(nine_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/sideboats.xpm", "r");
	for (i = 0; i < 403; i++) {
		sidebar_xpm[i] = malloc(182 * sizeof(char));
		fgets(sidebar_xpm[i], 182, file);
		if (sidebar_xpm[i][strlen(sidebar_xpm[i]) - 1] == '\n') {
			sidebar_xpm[i][strlen(sidebar_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/x.xpm", "r");
	for (i = 0; i < 33; i++) {
		x_xpm[i] = malloc(32 * sizeof(char));
		fgets(x_xpm[i], 32, file);
		if (x_xpm[i][strlen(x_xpm[i]) - 1] == '\n') {
			x_xpm[i][strlen(x_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/y.xpm", "r");
	for (i = 0; i < 33; i++) {
		y_xpm[i] = malloc(32 * sizeof(char));
		fgets(y_xpm[i], 32, file);
		if (y_xpm[i][strlen(y_xpm[i]) - 1] == '\n') {
			y_xpm[i][strlen(y_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/water.xpm", "r");
	for (i = 0; i < 89; i++) {
		water_xpm[i] = malloc(152 * sizeof(char));
		fgets(water_xpm[i], 152, file);
		if (water_xpm[i][strlen(water_xpm[i]) - 1] == '\n') {
			water_xpm[i][strlen(water_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/hit.xpm", "r");
	for (i = 0; i < 88; i++) {
		hit_xpm[i] = malloc(152 * sizeof(char));
		fgets(hit_xpm[i], 152, file);
		if (hit_xpm[i][strlen(hit_xpm[i]) - 1] == '\n') {
			hit_xpm[i][strlen(hit_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	file = fopen("/usr/src/drivers/batalha_naval/res/sunk.xpm", "r");
	for (i = 0; i < 89; i++) {
		sunk_xpm[i] = malloc(152 * sizeof(char));
		fgets(sunk_xpm[i], 152, file);
		if (sunk_xpm[i][strlen(sunk_xpm[i]) - 1] == '\n') {
			sunk_xpm[i][strlen(sunk_xpm[i]) - 1] = '\0';
		}
	}
	fclose(file);

	return 0;
}
