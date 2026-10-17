#include "clock.h"

int (clock_start_timer) () {
    if (rtc_get_time(&start_hours, &start_minutes, &start_seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (game_start_hours == 0 && game_start_minutes == 0 && game_start_seconds == 0) {
        game_start_hours = start_hours;
        game_start_minutes = start_minutes;
        game_start_seconds = start_seconds;
    }

    if (rtc_set_alarm(remaining_hours, remaining_minutes, remaining_seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (clock_stop_timer) () {
    uint8_t current_hours, current_minutes, current_seconds;
    if (rtc_get_time(&current_hours, &current_minutes, &current_seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_disable_alarm() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    uint32_t elapsed_seconds = (current_hours * 3600 + current_minutes * 60 + current_seconds) - (start_hours * 3600 + start_minutes * 60 + start_seconds);
    uint32_t remaining_time = (remaining_hours * 3600 + remaining_minutes * 60 + remaining_seconds) - elapsed_seconds;

    remaining_hours = remaining_time / 3600;
    remaining_time -= remaining_hours * 3600;
    remaining_minutes = remaining_time / 60;
    remaining_time -= remaining_minutes * 60;
    remaining_seconds = remaining_time;

    return 0;
}

int (clock_reset_timer) () {
    remaining_hours = 0;
    remaining_minutes = 10;
    remaining_seconds = 0;
    game_start_hours = 0;
    game_start_minutes = 0;
    game_start_seconds = 0;

    return 0;
}

int (clock_get_elapsed_time) (uint8_t* hours, uint8_t* minutes, uint8_t* seconds) {
    uint8_t curr_minutes, curr_seconds, curr_hours;
    if (rtc_get_time(&curr_hours, &curr_minutes, &curr_seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    uint32_t elapsed_seconds = (to_binary(curr_hours) * 3600 + to_binary(curr_minutes) * 60 + to_binary(curr_seconds))
            - (to_binary(game_start_hours) * 3600 + to_binary(game_start_minutes) * 60 + to_binary(game_start_seconds));

    *hours = elapsed_seconds / 3600;
    elapsed_seconds -= *hours * 3600;
    *minutes = elapsed_seconds / 60;
    elapsed_seconds -= *minutes * 60;
    *seconds = elapsed_seconds;

    return 0;
}
