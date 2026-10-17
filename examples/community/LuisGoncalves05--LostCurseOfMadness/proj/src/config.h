#define GAME_FREQUENCY 30   /**< Game frame rate */
#define VIDEO_MODE 0x107    /**< VBE mode the game runs in. 0x107 for higher res */
#define CLOSE_RADIUS 80     /**< Radius of the player's local field of view */
#define FOV_RADIUS 400      /**< Radius of the player's field of view */
#define FOV_ANGLE 60        /**< Angle (deg) of the player's field of view */
#define IN_FOV_COLOR 56     /**< Color of the player's field of view */
#define OUT_OF_FOV_COLOR 64 /**< Black color for things outside of the player's field of view */
#define CELL_SIZE 64        /**< Total size of each cell including wall width. */
