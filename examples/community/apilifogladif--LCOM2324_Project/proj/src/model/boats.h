#ifndef _LCOM_BOATS_H_
#define _LCOM_BOATS_H_

/**
 * @file boats.h
 * 
 * @brief This file contains the boats functions.
*/

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"
#include "view/placingDraw.h"
#include "game.h"

/**
 * @brief Function that constructs and returns the boat created.
 * 
 * It constructs the Boat, allocates the boat object in a memory block and returns the boat that the construction fuction creates.
 * 
 * @param sprite : Horizontal boat's sprite.
 * @param s_sprite : Horizontal selected boat's sprite.
 * @param v_sprite : Vertical boat's sprite.
 * @param sv_sprite : Vertical selected boat's sprite.
 * @param small_sprite : Horizontal small boat's sprite.
 * @param small_v_sprite : Vertical small boat's sprite.
 * @param h : Boat's desired height.
 * @param w : Boat's desired width.
 * @param id : Boat's id.
 * @return Boat* : Boat created.
 */
Boat *create_boat(Sprite *sprite, Sprite *s_sprite, Sprite *v_sprite, Sprite *sv_sprite, Sprite *small_sprite, Sprite *small_v_sprite, int h, int w, int id);

/**
 * @brief Function that destroys a given boat.
 * 
 * It destroys the boat and frees the block that was allocated for the boat.
 * 
 * @param boat : Boat to be destroyed.
 */
void destroy_boat(Boat *boat);

/**
 * @brief Function that updates the given boat's position.
 * 
 * Moves the boat to a given (x,y) coordinates.
 * 
 * @param boat : Boat to be positioned.
 * @param x : Horizontal position.
 * @param y : Vertical position.
 */
void updateBoatPos(Boat * boat, int x, int y);

/**
 * @brief Function that loads and creates the different boats sprites and attributes.
 * 
 * Creates all the different boats and loads them.
 * 
 */
void load_boats();

/**
 * @brief Function that draws the selected boat with id b.
 * 
 * Draws selected boat.
 * 
 * @param b : Boat's id.
 */
void drawSelected(int b);

/**
 * @brief Function that draws that draws all the boats.
 * 
 * Draws all the boats' sprites.
 */
void drawBoats();

/**
 * @brief Function that fixes boats positions when overlapping.
 * 
 * Bypasses overlap of Boats and adjusts their positions.
 */
void fixTheSelected();

/**
 * @brief Function that draws the boat in its new updated position.
 * 
 * Updates the boat position and redraws it.
 * 
 * @param y : Boat's vertical position.
 * @param x : Boat's horizontal position.
 */
void moveSelected(int y, int x);

/**
 * @brief Function that draws the boat sprite by receiving its id.
 * 
 * Receives boat's id, checks its attributes and draws its sprite.
 * 
 * @param b : Boat's id.
 */
void drawBoat(int b);

/**
 * @brief Function that rotates the selected boat.
 * 
 * Rotates boat so player can change from vertical to horizontal position or vice versa.
 */
void rotateSelected();

/**
 * @brief Function that checks overlapping between selected boat and already placed boat.
 * 
 * Checks if spot occuped by boat is already taken or not and returns true or false.
 * 
 * @param boat : Boat to be checked if overlaps with another one.
 * 
 * @return bool : true if overlapps and false otherwise.
 */
bool overlapBoats(Boat* boat);

/**
 * @brief Function that sets the spots occupied by the boat as taken by it.
 * 
 * Sets the spots occupied by the boat as taken by it by setting the .taken attribute of the spot to the boat's id.
 * 
 * @param boat : Spots will be set as taken by the id of the given boat.
 * 
 */
void setSpots(Boat* boat);

/**
 * @brief Function that sets the spots occupied by the given boat of the board set as untaken.
 * 
 * Sets the spots occupied by the boat as untaken by it by setting the taken attribute to 0.
 * 
 * @param boat : Spots occupied by the given boat will be set as untaken.
 * 
 */
void clearSpots(Boat* boat);

/**
 * @brief Function that returns true if all the boats are fixed and false otherwise.
 * 
 * Evaluates if all boats are fixed.
 * 
 * @return bool : true if all fixed and false otherwise.
 */
bool allBoatsFixed();

#endif
