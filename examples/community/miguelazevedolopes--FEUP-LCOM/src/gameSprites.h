
#include <lcom/lcf.h>
#include <stdint.h>
/** @defgroup gameSprites gameSprites.h
 * @{
 *
 * This file contains the main game-sprites related functions.
 * 
 */

typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t img;
    xpm_image_t img_clean;
    uint8_t *pixmap;
    uint8_t *pixmap_clean;
} bar;

typedef struct{
    uint16_t x;
    uint16_t y;
    int16_t vx;
    int16_t vy;
    xpm_image_t img;
    xpm_image_t img_clean;
    uint8_t *pixmap;
    uint8_t *pixmap_clean;
} ball;

typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t point[10];
    uint8_t* pixmap[10];
} points;

/**
 * @brief Loads bars sprites, and sets their position.
 * 
 */
void init_bars();

/**
 * @brief Dealocates memory that was previously alocated to draw the sprites.
 * 
 */
void free_all();

/**
 * @brief Initializes the sprite of the colon;
 * 
 */
void init_timer();

/**
 * @brief Moves the right bar sprite up;
 * 
 */
void go_up_rbar();
/**
 * @brief Moves the right bar sprite down;
 * 
 */
void go_down_rbar();

/**
 * @brief Moves the left bar sprite up;
 * 
 */
void go_up_lbar();

/**
 * @brief Moves the left bar sprite down;
 * 
 */
void go_down_lbar();

/**
 * @brief Initializes the sprite of the ball, sets its position and speed;
 * 
 */
void init_ball();
/**
 * @brief Handles the movement of the ball.
 * 
 * @return 0 if it hits one of the bars or the top and bottom of the screen, or if no colision occurs. Returns 1 or 2 respectively if a player scores a point
 */
int move_ball();

/**
 * @brief Detects if the ball hits something.
 * 
 * @return 1 for colision with right bar,2 with left bar, 3 for top or bottom colision and 0 if no colision
 */
int detect_colision(); 

/**
 * @brief Initializes the numbers used for the points, as well as the highscore and the timer.
 * 
 */
void init_points();

/**
 * @brief Draws the current points, based on the value of the global variables PlayerOnePoints and PlayerTwoPoints
 * 
 */
void draw_points();

/**
 * @brief Resets the bars to their original position at the center of the y axis.
 * 
 */
void reset_bars();

/**
 * @brief Resets the ball to its original position at the center of the screen.
 * 
 */
void reset_ball();

/**
 * @brief Draws the game when it starts.
 * 
 */
void drawStartGame();

/**
 * @brief Draws the game after it was paused.
 * 
 */
void drawResumeGame();

/**
 * @brief Auxilary function to draw the bars.
 * 
 */
void draw_bars();

/**
 * @brief Auxilary function to draw the ball.
 * 
 */
void draw_ball();

/**
 * @brief Auxilary function to draw the time.
 * 
 */
void draw_time(unsigned int minute,unsigned int second);

/**@}*/
