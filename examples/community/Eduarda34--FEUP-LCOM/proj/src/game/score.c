#include "score.h"

int survival_score = 0;

void score_reset() {
    survival_score = 0;
}

void score_increment() {
    survival_score++;
}

int get_score() {
    return survival_score;
}
