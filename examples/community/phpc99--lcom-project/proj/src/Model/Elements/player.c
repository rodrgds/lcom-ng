#include "player.h"

/**
 * @brief Allocates and initializes a new Player.
 *
 * Sets up a Player object with default dimensions, zero vertical speed,
 * a new backpack with 9 slots, and an initial state of `PLAYER_IDLE`.
 *
 * @param position The starting position of the player.
 * @param on_ground Whether the player starts on the ground.
 * @return Pointer to the newly created Player, or NULL if allocation fails.
 */

Player *new_Player(Position position, bool on_ground){

    Player *p = malloc(sizeof(*p));

    if( p == NULL){
        return NULL;
    }
    p->position = position;
    p->width = 1;
    p->height = 1;
    p->on_ground = on_ground;
    p->vy = 0;
    p->backpack = new_Backpack(9);
    p->state = PLAYER_IDLE;
    return p;
}

/**
 * @brief Frees memory associated with a Player.
 *
 * Also deallocates the player's backpack.
 *
 * @param p Pointer to the Player to delete.
 */

void delete_Player(Player *p){
    delete_Backpack(p->backpack);
    free(p);
}

/**
 * @brief Retrieves the current position of the Player.
 *
 * @param p Pointer to the Player.
 * @return The player's position as a `Position` struct.
 */

Position get_Player_Position(Player *p){
    return p->position;
}
