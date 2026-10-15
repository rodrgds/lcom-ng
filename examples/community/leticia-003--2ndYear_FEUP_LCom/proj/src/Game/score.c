/**
 * @file score.c
 * @brief Implements the score logic and drawing for the game.
 */

#include "score.h"
#include "Drivers/VBE/vbe.h"
#include "Sprites/start_sprite.h"
#include <stdio.h>

/**
 * @def MAX_DIGITS
 * @brief Maximum number of digits to display for the score.
 */
#define MAX_DIGITS 3

/**
 * @def SCORE_X
 * @brief Default X position for drawing the score.
 */
#define SCORE_X 50  

/**
 * @def SCORE_Y
 * @brief Default Y position for drawing the score.
 */
#define SCORE_Y 50 

static int current_score = 0;

/**
 * @var left_column_width
 * @brief Width of the left column for score alignment.
 */
int left_column_width = 250;

static xpm_image_t digit_imgs[10];
static uint8_t* digit_maps[10];

/**
 * @brief Initializes the digit sprites for score display.
 */
void score_init(void) {
    xpm_map_t xpm_digits[] = {
        digit_0_xpm, digit_1_xpm, digit_2_xpm, digit_3_xpm, digit_4_xpm,
        digit_5_xpm, digit_6_xpm, digit_7_xpm, digit_8_xpm, digit_9_xpm
    };

    for (int i = 0; i < 10; i++) {
        digit_maps[i] = xpm_load(xpm_digits[i], XPM_8_8_8_8, &digit_imgs[i]);
        if (!digit_maps[i]) {
            printf("❌ Failed to load digit %d\n", i);
        }
    }
}

/**
 * @brief Resets the current score to zero.
 */
void score_reset(void) {
    current_score = 0;
}

/**
 * @brief Adds points to the current score.
 * @param points Number of points to add.
 */
void score_add(int points) {
    current_score += points;
}

/**
 * @brief Gets the current score.
 * @return The current score.
 */
int score_get(void) {
    return current_score;
}

/**
 * @brief Draws the current score at the specified position.
 * @param x X position.
 * @param y Y position.
 */
void score_draw_at(int x, int y) {
    char buf[MAX_DIGITS + 1];
    snprintf(buf, sizeof(buf), "%d", current_score);

    int len = strlen(buf);
    int total_width = 0;

    for (int i = 0; i < len; i++) {
        int digit = buf[i] - '0';
        total_width += digit_imgs[digit].width;
    }

    int draw_x = x + (left_column_width - total_width) / 2;
    
    vg_draw_rectangle(draw_x, y, 100, digit_imgs[0].height, 0x0077B6);

    for (int i = 0; i < len; i++) {
        int digit = buf[i] - '0';
        vg_draw_xpm(draw_x, y, digit_imgs[digit].width, digit_imgs[digit].height, digit_maps[digit], false);
        draw_x += digit_imgs[digit].width;
    }
}


