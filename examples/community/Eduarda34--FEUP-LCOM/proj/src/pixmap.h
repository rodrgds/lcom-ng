#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

// Only declare here
extern const char *const solid_snake_segment[];
extern const char *const apple[];

extern const char *const snake_tongue_down[];
extern const char *const snake_tongue_up[];
extern const char *const snake_tongue_left[];
extern const char *const snake_tongue_right[];

extern const char *const digit_0[];
extern const char *const digit_1[];
extern const char *const digit_2[];
extern const char *const digit_3[];
extern const char *const digit_4[];
extern const char *const digit_5[];
extern const char *const digit_6[];
extern const char *const digit_7[];
extern const char *const digit_8[];
extern const char *const digit_9[];

xpm_map_t get_digit_xpm(int digit);
