#include "leaderboard_state.h"
#include "../font.h"
#include "../ui.h"

static Test lb_best_tests[B_T_SIZE];
static int lb_count;

int leaderboard_state_init() {
    load_best_tests(lb_best_tests, &lb_count);
    return 0;
}

void leaderboard_state_draw() {
    video_clear(0x1C1C1C);

    uint16_t char_h = char_height;

    font_draw_centered("leaderboard", 40, 0xE2A100);

    video_draw_rect(80, 40 + char_h + 8, screen_width - 160, 2, 0x444444);

    uint16_t table_x = 80;
    uint16_t table_y = 40 + char_h + 30;
    display_best_tests(lb_best_tests, lb_count, table_x, table_y);

    ui_draw_esc_hint();
}

void leaderboard_state_reload() {
    load_best_tests(lb_best_tests, &lb_count);
}

void leaderboard_state_destroy() {}
