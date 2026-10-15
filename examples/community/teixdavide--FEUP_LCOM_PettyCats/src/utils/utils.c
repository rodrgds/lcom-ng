#include <stdlib.h>
#include "utils.h"
#include <time.h>
#include <math.h>
#include "../drivers/rtclock.h"

int randomNumer(int lower_bound, int upper_bound) {
    int random_number;
    
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand(time(NULL));
        seed_initialized = 1;
    }

    random_number = lower_bound + rand() % (upper_bound - lower_bound + 1);
    
    return random_number;
}

double degToRad(double angleInDegrees) {
    return angleInDegrees * (M_PI / 180.0);
}

double radToDeg(double angleInRad) {
    return angleInRad / (M_PI / 180);
}

