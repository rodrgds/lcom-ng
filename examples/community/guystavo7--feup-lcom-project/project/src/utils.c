#include "../include/utils/utils.h"
/** @file utils.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

float floatModule(float n) {
	if (n < 0) n = -n;
	return n;
}

// Cut and paste this to file - only way to work
void print_double(double a, int decimals) {
    int factor = 1;

    // build 10^decimals
    for (int i = 0; i < decimals; i++) {
        factor *= 10;
    }

    // handle negative numbers
    if (a < 0) {
        printf("-");
        a = -a;
    }

    // shift decimal
    int int_part = (int)a;
    int frac_part = (int)((a - int_part) * factor);

    // print integer part
    printf("%d", int_part);

    // print decimal part
    if (decimals > 0) {
        printf(".");

        // handle leading zeros in fractional part
        int tmp = frac_part;
        int div = factor / 10;

        while (div > 0 && tmp < div) {
            printf("0");
            div /= 10;
        }

        printf("%d", frac_part);
    }
}


