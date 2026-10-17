#ifndef CLOCK_H
#define CLOCK_H

#include <lcom/lcf.h>
#include "../sprites/sprite.h"

/** @defgroup clock clock
 *  @brief Functions for using the game clock
*/

extern Sprite* clock_background;
extern Sprite *numbers[10];

/**
 * @brief Struct for clock time
 */
typedef struct {
    int minutes;
    int seconds;
} Time;

/**
 * @brief Struct for the game clock
 */
typedef struct {
    Time time;
    bool running;
} Clock;

static Clock white_clock;
static Clock black_clock;

/**
 * @brief Initializes the clock with 5 minutes
 */
void clock_init();

/**
 * @brief Sets the clock to the specified turn
 * @param white_turn True if it's white's turn, false otherwise
 */
void clock_set(bool white_turn);

/**
 * @brief Updates the clock time
 */
void clock_update();  

/**
 * @brief Stops the clock
 */
void clock_stop();

/**
 * @brief Draws the clocks for both players
 * @return 0 upon success, 1 otherwise
 */
int draw_clock();

/**
 * @brief Checks if the clock has timed out (reached 0:00)
 * @return True if the clock has timed out, false otherwise
 */
bool clock_timeout();

#endif
