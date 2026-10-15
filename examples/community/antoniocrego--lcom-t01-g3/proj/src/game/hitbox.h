#ifndef LCOM_HITBOX_H
#define LCOM_HITBOX_H

#define TOP 8
#define BOTTOM 4
#define RIGHT 2
#define LEFT 1
#define NO 0

#include <lcom/lcf.h>
#include "../controllers/video/video_gr.h"

/// @brief Stores the information needed to transport the player between levels
typedef struct gate{
    char origin_level;           /**< Level of wich the player is transporting from */
    char destination_level;      /**< Level of wich the player is tranporting to*/
    int destination_x;          /**< X coordinate of the initial position for the player in the destination level*/
    int destination_y;          /**< Y coordinate of the initial position for the player in the destination level*/
} gate;

/// @brief Stores the information needed about the hitboxes of the game elements
typedef struct invis_hitbox{
    int x_pos;                  /**< X coordinate of the top left corner of the hitbox*/
    int y_pos;                  /**< Y coordinate of the top left corner of the hitbox*/
    uint16_t width;                  /**< Width of the hitbox*/
    uint16_t length;                 /**< Length of the hitbox*/
    char type[16];              /**< Inditication of the type of the hitbox*/
    gate destination;           /**< Gate information if the hitbox is for a gate*/
} invis_hitbox;

/// @brief Draws the hitbox. This function was used to test and debug the placement of hitboxes and the interaction between them
/// @param hitbox Hitbox to draw
/// @return Returns 0 on sucess and 1 otherwise
int draw_hitbox(invis_hitbox hitbox);

/// @brief Constructs a hitbox
/// @param x X coordinate of the top left corner of the hitbox
/// @param y Y coordinate of the top left corner of the hitbox
/// @param width Width of the hitbox
/// @param length Length of the hitbox
/// @param type Inditication of the type of the hitbox
/// @return Returns a hitbox
invis_hitbox init_hitbox(int x, int y, uint16_t width, uint16_t length, char type[]);

/// @brief Verifies collision between two hitboxes
/// @param hitbox_1 First hitbox to compare
/// @param hitbox_2 Second hitbox to compare
/// @return Returns the directions in which the collision occurred relative to the first hitbox
char verifyCollision(invis_hitbox hitbox_1, invis_hitbox hitbox_2);

/// @brief Constructs a gate
/// @param orig Origin level from which the player will be tranported
/// @param dest Destination level to which the player will be transported
/// @param x X coordinate of the initial position for the player in the destination level
/// @param y Y coordinate of the initial position for the player in the destination level
/// @return Returns a gate
gate init_gate(char orig, char dest, int x, int y);

#endif
