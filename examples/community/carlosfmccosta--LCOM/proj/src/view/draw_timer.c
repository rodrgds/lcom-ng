/**
 * @file draw_timer.c
 * @brief Implementation of timer sprite drawing and management.
 *
 * This file contains the implementation of the functions declared in draw_timer.h.
 */
#include "draw_timer.h"

/**
 * @struct draw_timer
 * @brief Holds the sprites used for drawing a timer (digits and colon).
 *
 * Contains pointers to the digit sprite (for 0-9) and the colon sprite,
 * used to render a digital timer (HH:MM:SS) on the screen.
 */
struct draw_timer {
    sprite_t* digits;  /**< Sprite containing digits 0-9 */
    sprite_t* colon;   /**< Sprite containing colon symbol */
};

draw_timer_t* create_timer(uint16_t x, uint16_t y) {
    draw_timer_t* ts = malloc(sizeof(draw_timer_t));
    if (!ts) {
        return NULL;
    }

    //Create digits sprite
    xpm_map_t digits_map[10] = {zero, um, dois, tres, quatro, cinco, seis, sete, oito, nove};
    ts->digits = create_sprite(digits_map, 10, x, y);
    if (!ts->digits) {
        free(ts);
        return NULL;
    }

    //Create colon sprite
    xpm_map_t pontos_arr[1] = {pontos};
    ts->colon = create_sprite(pontos_arr, 1, x, y);
    if (!ts->colon) {
        destroy_sprite(ts->digits);
        free(ts);
        return NULL;
    }

    return ts;
}

void destroy_timer(draw_timer_t* ts) {
    if (!ts) {
        return;
    }

    destroy_sprite(ts->digits);
    destroy_sprite(ts->colon);
    free(ts);
    ts = NULL;
}

int draw_timer(draw_timer_t* ts, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t x, uint16_t y) {
    if (!ts || !ts->digits || !ts->colon){ 
        return 1;
    }

    const uint16_t  digit_width = get_sprite_width(ts->digits);
    const uint16_t  colon_width = get_sprite_width(ts->colon);
    const uint16_t spacing = 2;

    int ret;

    // Draw hours
    ret = draw_digit(ts, hours / 10, x, y);
    ret = ret || draw_digit(ts, hours % 10, x + digit_width + spacing, y);

    // Draw colon
    ret = ret || draw_colon(ts, x + 2 * digit_width + spacing * 2, y);

    // Draw minutes
    ret = ret || draw_digit(ts, minutes / 10, x + 2 * digit_width + colon_width + spacing * 3, y);
    ret = ret || draw_digit(ts, minutes % 10, x + 3 * digit_width + colon_width + spacing * 4, y);

    // Draw colon
    ret = ret || draw_colon(ts, x + 4 * digit_width + colon_width + spacing * 5, y);

    // Draw seconds
    ret = ret || draw_digit(ts, seconds / 10, x + 4 * digit_width + 2 * colon_width + spacing * 6, y);
    ret = ret || draw_digit(ts, seconds % 10, x + 5 * digit_width + 2 * colon_width + spacing * 7, y);

    return ret;
}

int draw_digit(draw_timer_t* ts, uint8_t digit, uint16_t px, uint16_t py) {
    set_sprite_curr(ts->digits, digit);
    position_t* pos = get_sprite_pos(ts->digits);
    pos->y = py;
    pos->x = px;
    return draw_sprite(ts->digits);
}

int draw_colon(draw_timer_t* ts, uint16_t px, uint16_t py) {
    position_t* pos = get_sprite_pos(ts->colon);
    pos->y = py;
    pos->x = px;
    return draw_sprite(ts->colon);
}
