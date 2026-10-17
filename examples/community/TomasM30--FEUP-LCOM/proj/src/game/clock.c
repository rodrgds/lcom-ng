#include "clock.h"

void clock_init() {
    white_clock.time.minutes = 5;
    white_clock.time.seconds = 0;
    white_clock.running = false;

    black_clock.time.minutes = 5;
    black_clock.time.seconds = 0;
    black_clock.running = false;
}

void clock_set(bool white_turn) {
    white_clock.running = white_turn;
    black_clock.running = !white_turn;
}

void clock_update() {
    if (white_clock.running) {
        if (white_clock.time.seconds == 0) {
            if (white_clock.time.minutes == 0) {
                white_clock.running = false;
                return;
            }
            white_clock.time.minutes--;
            white_clock.time.seconds = 59;
        }
        else white_clock.time.seconds--;
    }

    if (black_clock.running) {
        if (black_clock.time.seconds == 0) {
            if (black_clock.time.minutes == 0) {
                black_clock.running = false;
                return;
            }
            black_clock.time.minutes--;
            black_clock.time.seconds = 59;
        }
        else black_clock.time.seconds--;
    }
}

void clock_stop() {
    white_clock.running = false;
    black_clock.running = false;
}

int draw_clock() {
    if (draw_sprite(clock_background, 940, 34)) return 1;

    if (draw_sprite(numbers[black_clock.time.minutes], 990, 44)) return 1;

    if (draw_sprite(numbers[black_clock.time.seconds / 10], 1020, 44)) return 1;

    if (draw_sprite(numbers[black_clock.time.seconds % 10], 1040, 44)) return 1;


    if (draw_sprite(clock_background, 940, 790)) return 1;

    if (draw_sprite(numbers[white_clock.time.minutes], 990, 800)) return 1;

    if (draw_sprite(numbers[white_clock.time.seconds / 10], 1020, 800)) return 1;

    if (draw_sprite(numbers[white_clock.time.seconds % 10], 1040, 800)) return 1;

    return 0;
}

bool clock_timeout() {
    if (white_clock.time.minutes == 0 && white_clock.time.seconds == 0) return true;
    if (black_clock.time.minutes == 0 && black_clock.time.seconds == 0) return true;
    
    return false;
}
