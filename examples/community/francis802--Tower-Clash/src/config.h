/**
 * @file config.h
 * 
 * @brief File containing the game configuration variables, such as the game
 * frequency, the video mode and the double buffer mode enabler.
*/

/**
 * @brief This macro defines the game frequency, i.e, frames per second.
*/
#define GAME_FREQUENCY      30

/**
 * @brief This macro defines the video mode to be used by the game.
*/
#define VIDEO_MODE          0x115

/**
 * @brief This macro defines whether the double buffer mode should be enabled or not. If it is enabled, the game will use a double buffer to draw the sprites, which will be copied to the video memory at the end of each frame. If it is disabled, the game will draw the sprites directly to the video memory.
*/
#define DOUBLE_BUFFER       1

/**
 * @brief This macro defines the gravity value, which is used to calculate the vertical speed of the projectile.
*/
#define GRAVITY             2.2    // 3, made up gravity

