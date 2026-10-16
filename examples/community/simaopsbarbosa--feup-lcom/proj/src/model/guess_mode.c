#include <lcom/lcf.h>
#include "guess_mode.h"

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
int max_int_guess = 20; // 20 seconds

bool count_down; // Flag to indicate if the countdown is active
bool p1_pressed;
bool p2_pressed;

void guess_mode_init(void) {
    p1_pressed = false;
    p2_pressed = false;
    winner = SOMEONE; // reset winner
    timeDiff.player1 = 0;
    timeDiff.player2 = 0;
    elapsed_int = 0;
    max_int_guess = 20 * timer_frequency;
    target_int = random_number(4 * timer_frequency, 15 * timer_frequency); // Random timer interrupt between 4 and 15 seconds
    count_down = true;

    draw_guess_mode_frame(elapsed_int / timer_frequency, target_int / timer_frequency);
}

void guess_mode_timer_ih() {
    if(gameMode != GUESS_MODE) return;

    if(elapsed_int < 3*timer_frequency && elapsed_int % timer_frequency == 0 && count_down) {
        draw_guess_mode_frame(elapsed_int / timer_frequency, target_int / timer_frequency);
    } else if(elapsed_int == 3*timer_frequency && count_down) {
        count_down = false;
        elapsed_int = 0;
        draw_guess_mode_frame(timer_frequency*3 + 1, target_int / timer_frequency);
        return;
    } // Reset after 3 seconds to start guess time

    if(elapsed_int >= max_int_guess) {
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
}

void guess_mode_keyboard_ih(void) {
    if(gameMode != GUESS_MODE) return;
    if(count_down) return; // Ignore key presses during countdown
  
    if (scancode == SPACE_KEY && !p1_pressed) {
        p1_pressed = true;
        winner = SOMEONE;
        timeDiff.player1 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        if(playerMode == ONE_PLAYER || p2_pressed) {
            change_mode(GAME_OVER);
        }
    }
}

void guess_mode_mouse_ih(void) {
    update_mouse_state_menus();
    if(gameMode != GUESS_MODE) return;
    if(playerMode == ONE_PLAYER) return;
    if(count_down) return; // Ignore mouse clicks during countdown

    if (isLeftButtonPressed() && !p2_pressed) {
        p2_pressed = true;
        winner = SOMEONE;
        timeDiff.player2 = (int)((elapsed_int - target_int) * 1000 / timer_frequency); // in milliseconds
        if (p1_pressed) {
            change_mode(GAME_OVER);
        }
    }
}
