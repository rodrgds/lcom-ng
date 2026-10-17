#include "views.h"

const xpm_map_t keys[] = {
    (xpm_map_t)key_q_xpm, (xpm_map_t)key_w_xpm, (xpm_map_t)key_e_xpm,
    (xpm_map_t)key_r_xpm, (xpm_map_t)key_t_xpm, (xpm_map_t)key_y_xpm,
    (xpm_map_t)key_u_xpm, (xpm_map_t)key_i_xpm, (xpm_map_t)key_o_xpm,
    (xpm_map_t)key_p_xpm, (xpm_map_t)key_a_xpm, (xpm_map_t)key_s_xpm,
    (xpm_map_t)key_d_xpm, (xpm_map_t)key_f_xpm, (xpm_map_t)key_g_xpm,
    (xpm_map_t)key_h_xpm, (xpm_map_t)key_j_xpm, (xpm_map_t)key_k_xpm,
    (xpm_map_t)key_l_xpm, (xpm_map_t)key_cedilla_xpm, (xpm_map_t)key_z_xpm,
    (xpm_map_t)key_x_xpm, (xpm_map_t)key_c_xpm, (xpm_map_t)key_v_xpm,
    (xpm_map_t)key_b_xpm, (xpm_map_t)key_n_xpm, (xpm_map_t)key_m_xpm,
    (xpm_map_t)key_comma_xpm, (xpm_map_t)key_period_xpm
};

const char keys_chars[] = {
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '*',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', '%', '.'
};

static xpm_map_t get_colored_key_xpm(KeyPerformance perf) {
    switch (perf) {
        case KEY_EXCELLENT: return (xpm_map_t)dark_green_key_xpm;
        case KEY_GOOD: return (xpm_map_t)light_green_key_xpm;
        case KEY_AVERAGE: return (xpm_map_t)orange_key_xpm;
        case KEY_BAD: return (xpm_map_t)red_key_xpm;
        case KEY_NO_DATA: return (xpm_map_t)transparent_key_xpm;
        default: return (xpm_map_t)transparent_key_xpm;
    }
}

void draw_keyboard() {
    // Layout
    int x = 240;
    int y = 450;
    int n = 10;
    int key_id = 0;
    bool* key_pressed = get_pressed_keys();

    for (int i = 0; i < 3; i++) {
        if (i == 2) n = 9;
        for (int j = 0; j < n; j++, key_id++) {
            if (key_pressed[key_id]) {
                draw_image_to_both((xpm_map_t) orange_key_xpm, x, y);
            } else {
                draw_image_to_both((xpm_map_t) transparent_key_xpm, x, y);
            }
            draw_image_to_both(keys[i*10+j], x, y); // Key letter
            x += 55;
        }
        x = 200 + (i+1)*27;
        y += 55;
    }

    if (key_pressed[29]) {
        draw_image_to_both((xpm_map_t) orange_space_xpm, 364, 615);
    } else {
        draw_image_to_both((xpm_map_t) transparent_space_xpm, 364, 615);
    }
}

// Only redraw key that changed
void draw_keyboard_on_change(char c, bool make) {
    // Layout
    int x = 240;
    int y = 450;
    int n = 10;
    int key_id = 0;

    if (c == '_') {
        if (make) {
            draw_image((xpm_map_t) orange_space_xpm, 364, 615);
        } else {
            draw_image((xpm_map_t) transparent_space_xpm, 364, 615);
        }
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (i == 2) n = 9;
        for (int j = 0; j < n; j++, key_id++) {
            if (keys_chars[key_id] == c) {
                if (make) {
                    draw_image((xpm_map_t) orange_key_xpm, x, y);
                } else {
                    draw_image((xpm_map_t) transparent_key_xpm, x, y);
                }
                draw_image(keys[i*10+j], x, y); // Key letter
                return;
            }
            x += 55;
        }
        x = 200 + (i+1)*27;
        y += 55;
    }
}

void draw_stats_keyboard(const KeyPerformanceMap key_perf) {
    int x = 240;
    int y = 475;
    int n = 10;
    int key_index = 0;

    for (int row = 0; row < 3; row++) {
        if (row == 2) n = 9;

        for (int col = 0; col < n; col++, key_index++) {
            draw_image_to_both(get_colored_key_xpm(key_perf[key_index]), x, y); // Colored base
            draw_image_to_both(keys[key_index], x, y); // Letter on top
            x += 55;
        }

        x = 200 + (row + 1) * 27;
        y += 55;
    }

    // Draw space
    draw_image_to_both((xpm_map_t) transparent_space_xpm, 364, 640);
}
