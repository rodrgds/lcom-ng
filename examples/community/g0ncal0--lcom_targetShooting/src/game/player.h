#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <lcom/lcf.h>
#include "position.h"

/**
 * @struct Player
 * @brief Represents a player in the game.
 * 
 * The Player struct contains information about the player's position, score and whether the player can shoot.
 */
typedef struct {
  Position pos;     /** The player's position */
  uint16_t score;   /** The player's score */
  bool canShoot;    /** Indicates whether the player can shoot */
} Player;

/**
 * @brief Creates a new Player object.
 * @return A pointer to the newly created Player object.
 */
Player* createPlayer();

/**
 * @brief Gets the x-coordinate of the player's position.
 * @param player The Player object.
 * @return The x-coordinate of the player's position.
 */
int16_t getPlayerX(Player* player);

/**
 * @brief Gets the y-coordinate of the player's position.
 * @param player The Player object.
 * @return The y-coordinate of the player's position.
 */
int16_t getPlayerY(Player* player);

/**
 * @brief Gets the player's score.
 * @param player The Player object.
 * @return The player's score.
 */
uint16_t getPlayerScore(Player* player);

/**
 * @brief Checks if the player can shoot.
 * @param player The Player object.
 * @return True if the player can shoot, false otherwise.
 */
bool getPlayerCanShoot(Player* player);

/**
 * @brief Sets the x-coordinate of the player's position.
 * @param player The Player object.
 * @param x The new x-coordinate.
 */
void setPlayerX(Player* player, uint16_t x);

/**
 * @brief Sets the y-coordinate of the player's position.
 * @param player The Player object.
 * @param y The new y-coordinate.
 */
void setPlayerY(Player* player, uint16_t y);

/**
 * @brief Sets the player's score.
 * @param player The Player object.
 * @param score The new score.
 */
void setPlayerScore(Player* player, uint16_t score);

/**
 * @brief Sets whether the player can shoot.
 * @param player The Player object.
 * @param canShoot True if the player can shoot, false otherwise.
 */
void setPlayerCanShoot(Player* player, bool canShoot);

/**
 * @brief Destroys a Player object.
 * @param player The Player object to destroy.
 */
void destroyPlayer(Player* player);

#endif /* _PLAYER_H_ */
