#pragma once
#include <lcom/lcf.h>
#include "vector.h"
#include "../labs/graphics.h"

/**
 * @brief Enum that represents the type of player ball
*/

typedef enum playerBall{
  PLAYERSOLID,
  PLAYERSTRIPED,
  PLAYERBALLNONE,
} playerBall;

/**
 * @brief Struct that represents the player
*/
typedef struct Player{
  char* name;
  playerBall ballType;
  bool isPlaying;
} Player;

/**
 * @brief Creates a new Player object.
 *
 * This function allocates memory for a new Player object and initializes it with the given name, ball type, and playing status.
 *
 * @param name The name of the player.
 * @param ballType The type of ball assigned to the player.
 * @param isPlaying Boolean indicating if the player is currently playing.
 * @return Player* Pointer to the newly created Player object.
 */

Player* newPlayer(char* name, playerBall ballType , bool isPlaying);

/**
 * @brief Destroys a Player object.
 *
 * This function frees the memory allocated for the Player object.
 *
 * @param player Pointer to the Player object to be destroyed.
 */
void destroyPlayer(Player* player);

/**
 * @brief Draws the in-game player's name on the screen.
 *
 * This function draws the player's name on the screen using the specified font and coordinates.
 *
 * @param player Pointer to the Player object.
 * @param font Pointer to the xpm_image_t object representing the font.
 * @param x The x-coordinate where the name should be drawn.
 * @param y The y-coordinate where the name should be drawn.
 * @param spacing The spacing between characters.
 * @return int Returns 0 on success, 1 on failure.
 */
int drawInGamePlayerName(Player* player, xpm_image_t* font, int x, int y, int spacing);
