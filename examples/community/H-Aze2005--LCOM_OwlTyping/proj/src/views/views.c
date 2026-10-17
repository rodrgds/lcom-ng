#include "views.h"
#include "model/model.h"
#include "texts/phrases.h"
#include "utils/utils.h"

extern State current_state;
extern TimeSelector *timeSelector;

void draw_screen() {
    switch (current_state) {
        case HOME:
            draw_home();
            break;
        case TYPING:
            draw_typing();
            break;
        case PAUSE:
            draw_pause();
            break;
        case STATS:
            draw_stats();
            break;
        default:
            break;
    }
}

void draw_home() {
    load_background_xpm((xpm_map_t) home_screen_xpm);
    draw_TimeSelector(timeSelector);
    draw_background();
}

void draw_typing() {
    load_background_xpm((xpm_map_t) typing_screen_xpm);
    draw_background();
    draw_keyboard();
}

void draw_pause() {
    load_background_xpm((xpm_map_t) pause_screen_xpm);
    draw_background();
}

void draw_stats() {
    load_background_xpm((xpm_map_t) stats_screen_xpm);
    draw_background();

    draw_stats_keyboard(perf_map);
    draw_stats_result(final_wpm, final_accuracy);
}


// Stats page components
static xpm_map_t get_digit_xpm(int digit) {
    switch (digit) {
        case 0: return (xpm_map_t)stats_0_xpm;
        case 1: return (xpm_map_t)stats_1_xpm;
        case 2: return (xpm_map_t)stats_2_xpm;
        case 3: return (xpm_map_t)stats_3_xpm;
        case 4: return (xpm_map_t)stats_4_xpm;
        case 5: return (xpm_map_t)stats_5_xpm;
        case 6: return (xpm_map_t)stats_6_xpm;
        case 7: return (xpm_map_t)stats_7_xpm;
        case 8: return (xpm_map_t)stats_8_xpm;
        case 9: return (xpm_map_t)stats_9_xpm;
        default: return NULL;
    }
}

static void draw_stats_number(int value, int x_start, int y) {
    int digits[3];
    int num_digits = 0;

    // Split digits
    if (value == 0) {
        digits[0] = 0;
        num_digits = 1;
    } else {
        while (value > 0 && num_digits < 3) {
            digits[num_digits++] = value % 10;
            value /= 10;
        }
    }

    int x = x_start;

    // Draw digits from left to right
    for (int i = num_digits - 1; i >= 0; i--) {
        draw_image_to_both(get_digit_xpm(digits[i]), x, y);
        x += 65;
    }
}

void draw_stats_result(int speed, int accuracy) {
    // Speed section
    draw_stats_number(speed, 260, 240);
    draw_image_to_both((xpm_map_t)stats_wpm_xpm, 260 + 65 * ((speed >= 100) ? 3 : (speed >= 10) ? 2 : 1), 290);

    // Accuracy section
    draw_stats_number(accuracy, 570, 240);
    draw_image_to_both((xpm_map_t)stats_percentage_xpm, 570 + 65 * ((accuracy >= 100) ? 3 : (accuracy >= 10) ? 2 : 1), 290);
}
