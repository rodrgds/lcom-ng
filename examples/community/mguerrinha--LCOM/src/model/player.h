#ifndef _LCOM_PLAYER_H_
#define _LCOM_PLAYER_H_

#include <lcom/lcf.h>

/**
 * @brief Struct that represents the direction of the player.
*/
enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP,
} dir;

/**
 * @brief Struct that represents the position of the player.
*/
typedef struct {
    uint16_t x;
    uint16_t y;
} Position;

/**
 * @brief Struct that represents the player and its information.
*/
typedef struct{
    unsigned int id;
    uint16_t x;
    uint16_t y;
    enum direction dir;
    bool out_of_territory;
} PlayerInfo;

/**
 * @brief Function that moves the player up.
 * @param player_info Struct with the player information.
 * @return 0 if the player successfully moved up, 1 otherwise.
*/
int (move_player_up)(PlayerInfo *playerInfo);

/**
 * @brief Function that moves the player down.
 * @param player_info Struct with the player information.
 * @return 0 if the player successfully moved down, 1 otherwise.
*/
int (move_player_down)(PlayerInfo *playerInfo);

/**
 * @brief Function that moves the player left.
 * @param player_info Struct with the player information.
 * @return 0 if the player successfully moved left, 1 otherwise.
*/
int (move_player_left)(PlayerInfo *playerInfo);

/**
 * @brief Function that moves the player right.
 * @param player_info Struct with the player information.
 * @return 0 if the player successfully moved right, 1 otherwise.
*/
int (move_player_right)(PlayerInfo *playerInfo);

#endif
