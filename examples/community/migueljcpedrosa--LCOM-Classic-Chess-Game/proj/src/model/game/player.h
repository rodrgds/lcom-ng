/**
 * @file player.h
 * @brief This file contains the definition of the player module.
 * 
 * This includes the definition of the Player, PlayerType and Timer structs.
 * 
 * @see Player
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "pieces.h"

/**
 * @brief Represents a player type.
 * 
 * This enum represents a player type. It can be either a user or an AI.
 * 
 * @see Player
 */
typedef enum {
    USER,
    AI
} PlayerType;


/**
 * @brief Represents a timer.
 * 
 * This struct represents a timer. It is composed of a wait time and a start time.
 * This is used so that an AI player plays can be be delayed.
 * 
 * @see Player
 */
typedef struct {

    unsigned int wait_time;
    unsigned int start_time;

} Timer;

/**
 * @brief Represents a player.
 * 
 * This struct represents a player. It is composed of a name, a color, an array of pieces, a timer and a type.
 * 
 * The array of pieces contains all the pieces that the player controls. These have the same color as the player.
 * 
 * @see Color
 * @see Piece
 * @see Timer
 * @see PlayerType
 */
typedef struct {

    char* name;
    Color color;
    Piece* pieces[16];

    unsigned int time_counter;
    Timer timer;
    PlayerType type;

} Player;

/**
 * @brief Creates a player.
 * 
 * This function creates a player with the given name, color and type.
 * 
 * @param name the name of the player.
 * @param color the color of the player.
 * @param type the type of the player.
 * 
 * @return Returns the created player.
 */
Player* (create_player)(char name[], Color color, PlayerType type);

/**
 * @brief Destroys a player.
 * 
 * This function destroys a player and frees all the memory allocated to it.
 * 
 * @param player pointer to the player to be destroyed.
 */
void (destroy_player)(Player* player);

#endif
