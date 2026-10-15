#include <lcom/lcf.h>
#include "random.h"

int random_number(int min, int max) {
    return min + rand() % (max - min + 1);
}
