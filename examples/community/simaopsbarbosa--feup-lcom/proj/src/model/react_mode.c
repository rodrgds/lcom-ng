#include "react_mode.h"

// GAME externs
extern GameMode gameMode;
extern GameMode previousGameMode;
extern Winner winner;
extern PlayerMode playerMode;
extern TimeDiff timeDiff;

// VIDEO externs
extern uint8_t *drawing_frame_buffer;
extern vbe_mode_info_t mode_info;

// KEYBOARD externs
extern uint8_t scancode;

// TIMER externs
extern int timer_frequency;

int elapsed_int;
int target_int;
int max_int_react = 13; // 13 seconds

bool p1_pressed;
bool p2_pressed;

void react_mode_init(void) {
    p1_pressed = false;
    p2_pressed = false;
    winner = SOMEONE; // reset winner
    timeDiff.player1 = 0;
    timeDiff.player2 = 0;
    elapsed_int = 0;
    max_int_react = 13 * timer_frequency; // 13 seconds in timer ticks
    target_int = random_number(3 * timer_frequency, 10 * timer_frequency); // Random timer interrupt between 3 and 10 seconds

    draw_react_mode_frame(elapsed_int / timer_frequency, target_int / timer_frequency);
}

void react_mode_timer_ih(void) {
    if(gameMode != REACT_MODE) return;

    if(elapsed_int >= max_int_react) {
        if (playerMode == TWO_PLAYERS && !p1_pressed && !p2_pressed) {
            winner = NO_ONE;
        } else if (playerMode == TWO_PLAYERS && p1_pressed && !p2_pressed) {
            winner = SOMEONE;
            timeDiff.player2 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        } else if (playerMode == TWO_PLAYERS && !p1_pressed && p2_pressed) {
            winner = SOMEONE;
            timeDiff.player1 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        } else if (playerMode == ONE_PLAYER && !p1_pressed) {
            winner = NO_ONE;
        }
        change_mode(GAME_OVER);
        return;
    }

    if (p1_pressed && p2_pressed) {
        change_mode(GAME_OVER);
    } else {
        elapsed_int += 1;
    }

    if( elapsed_int == target_int) {draw_react_mode_frame(elapsed_int / timer_frequency, target_int / timer_frequency);}
}

void react_mode_keyboard_ih(void) {
    if(gameMode != REACT_MODE) return;

    if (scancode == SPACE_KEY && !p1_pressed) {
        p1_pressed = true;
        winner = SOMEONE;
        timeDiff.player1 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        if(playerMode == ONE_PLAYER || p2_pressed) {
            change_mode(GAME_OVER);
        }
    }
}

void react_mode_mouse_ih(void) {
   if(gameMode != REACT_MODE) return;
   if(playerMode == ONE_PLAYER) return;

   if (isLeftButtonPressed() && !p2_pressed) {
        p2_pressed = true;
        winner = SOMEONE;
        timeDiff.player2 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        if (p1_pressed) {
            change_mode(GAME_OVER);
        }
   }
}
