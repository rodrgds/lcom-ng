#include <lcom/lcf.h>

#include "difficulty_controller.h"

extern int score;
extern int freq;

extern int a_speed;
extern int a_diagspeed;

bool change_difficulty = false;
bool first = false;

void update_difficulty() {
    switch (score) {
        case 5:
            if (!first) {
                a_speed += 2;
                a_diagspeed += 2;
                first = true;
            }
            break;
        case 10:
            if (!change_difficulty) {
                change_difficulty = true;
                freq -= 5;
            }
            break;
        case 20:
            if (change_difficulty) {
                change_difficulty = false;
                freq -= 5;
            }
            break;
        case 30:
            if (!change_difficulty) {
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 40:
            if (change_difficulty) {
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 50:
            if (!change_difficulty) {
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 60:
            if (change_difficulty) {
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 70:
            if (!change_difficulty) {
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 80:
            if (change_difficulty) {
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
    }
}
