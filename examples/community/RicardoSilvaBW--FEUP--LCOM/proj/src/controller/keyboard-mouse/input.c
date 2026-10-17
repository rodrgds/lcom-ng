#include "input.h"

typedef struct s_input_profile{
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;
} t_input_profile;

static t_input_profile profiles[] = {
    [INPUT_ARROWS] = {
        UP_ARROW_MAKE,
        DOWN_ARROW_MAKE,
        LEFT_ARROW_MAKE,
        RIGHT_ARROW_MAKE
    },
    [INPUT_WASD] = {
        W_MAKE,
        S_MAKE,
        A_MAKE,
        D_MAKE
    },
    [INPUT_DFJK] = {
        D_MAKE,
        F_MAKE,
        J_MAKE,
        K_MAKE
    }
};

static t_input_scheme current_scheme = INPUT_ARROWS;

void set_input_scheme(t_input_scheme scheme) {
    current_scheme = scheme;
}

note_lane get_pressed_lane(uint8_t scancode) {
    t_input_profile *profile = &profiles[current_scheme];

    if (scancode == profile->up)
        return LANE_UP;
    if (scancode == profile->down)
        return LANE_DOWN;
    if (scancode == profile->left)
        return LANE_LEFT;
    if (scancode == profile->right)
        return LANE_RIGHT;
    return LANE_NONE;
}
