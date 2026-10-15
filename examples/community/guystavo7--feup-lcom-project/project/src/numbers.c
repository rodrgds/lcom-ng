#include "../include/ui/numbers.h"
/** @file numbers.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/data.h"
#include "../resources/numbers/numbers.h"
#include <stdlib.h>

struct Number {
    SpriteXPM** sprites;
    int digits;
    int number;
};

static void generateNumbers(Number* n, int number) {
    int digits = 0;
	if (number > 100) number = 99;
    while (number > 0) {
        digits++;
        number /= 10;
    }

    n->digits = digits;
    number = n->number;
    for (int i = 0; i < n->digits; i++) {
        int value = number % 10;
        number /= 10;
        n->sprites[i] = newSpriteXPM((xpm_map_t)numbersXPM[value], XPM_8_8_8, 0, 0);
    }

	if (n->number == 0) n->sprites[0] = newSpriteXPM((xpm_map_t)numbersXPM[n->number], XPM_8_8_8, 0, 0);
}

static void freeNumbers(Number* number) {
    for (int i = 0; i < number->digits; i++) freeSpriteXPM(number->sprites[i]);
}

Number* numberNew(int number) {
    Number* n = malloc(sizeof(Number));
	if (number > 100) number = 99;
    n->number = number;
    n->sprites = malloc(sizeof(SpriteXPM*) * 2);
	generateNumbers(n, number);	

    return n;
}

void numberFree(Number* number) {
    for (int i = 0; i < number->digits; i++) freeSpriteXPM(number->sprites[i]);
    free(number->sprites);
    free(number);
}

void numberUpdate(Number* number, Vec2d position, Terminal* terminal) {
    int x = position.x;
    int y = position.y;

    for (int i = number->digits - 1; i >= 0; i--) {
        x = position.x + NUMBER_WIDTH * (number->digits - 1 - i);
        drawSpriteXPM(number->sprites[i], terminal, x, y, false);
    }

    if (number->number == 0) {
        drawSpriteXPM(number->sprites[0], terminal, x, y, false);
    }
}


int numberGetNumber(Number* number) {
	return number->number;
}

void numberSetNumber(Number* number, int n) {
	number->number = n;
	freeNumbers(number);
	generateNumbers(number, n);
}
