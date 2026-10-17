#ifndef _TYPE_H_
#define _TYPE_H_

/**
 * @file Type.h
 * @brief Defines the different types of items available in the game.
 *
 * Declares an enumeration for item types.
 */

/**
 * @enum ItemType
 * @brief Represents the type of an item.
 *
 * Identifies different materials or objects the player can collect or interact with.
 */

typedef enum {
    ITEM_NONE,
    ITEM_WOOD,
    ITEM_EARTH,
    ITEM_SAPPLING,
    NUM_ITEM_TYPES
} ItemType;


#endif
