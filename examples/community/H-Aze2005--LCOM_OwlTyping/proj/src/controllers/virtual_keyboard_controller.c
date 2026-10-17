#include "virtual_keyboard_controller.h"

static bool key_pressed[NUM_KEYS] = { false };

static const char key_map_chars[NUM_KEYS] = {
    'q','w','e','r','t','y','u','i','o','p',
    'a','s','d','f','g','h','j','k','l','*',
    'z','x','c','v','b','n','m','%','.', '_'
    // '%' -> ',' and '*' -> 'ç'
};

bool* get_pressed_keys() {
    return key_pressed;
}

int get_key_index(char c) {
    for (int i = 0; i < NUM_KEYS; ++i) {
        if (key_map_chars[i] == c) return i;
    }
    return -1; // Not found
}
