#include "countdown_timer_view.h"

int remaining_time = 0;

const xpm_map_t countdown_keys[10] = {
    (xpm_map_t)count_down_0_xpm,
    (xpm_map_t)count_down_1_xpm,
    (xpm_map_t)count_down_2_xpm,
    (xpm_map_t)count_down_3_xpm,
    (xpm_map_t)count_down_4_xpm,
    (xpm_map_t)count_down_5_xpm,
    (xpm_map_t)count_down_6_xpm,
    (xpm_map_t)count_down_7_xpm,
    (xpm_map_t)count_down_8_xpm,
    (xpm_map_t)count_down_9_xpm,
};

void draw_countdown_timer(int remaining_time) {
    draw_rectangle_background(50, 50, 150, 50);

    if (remaining_time <= -1) return;

    int digits[3];
    int num_digits = 0;

    // Split digits
    if (remaining_time == 0) {
        digits[0] = 0;
        num_digits = 1;
    } else {
        while (remaining_time > 0 && num_digits < 3) {
            digits[num_digits++] = remaining_time % 10;
            remaining_time /= 10;
        }
    }

    int x = 50;
    int y = 50;

    // Draw digits from left to right
    for (int i = num_digits - 1; i >= 0; i--) {
        draw_image_to_both(countdown_keys[digits[i]], x, y);
        x += 35;
    }

    draw_image_to_both((xpm_map_t)count_down_s_xpm, x, 73);
}
