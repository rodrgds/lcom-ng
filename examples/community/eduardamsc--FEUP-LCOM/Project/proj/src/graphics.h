#pragma once
#include "obstacleCourse.h"
#include "rectangle.h"
#include "gameState.h"

typedef struct {
	int x;
	int y;
	int leftButton;
	int rightButton;
	int midButton;
	unsigned long packet[3];
	int packetReady;
	int packetIndex;
	int mouseUpdated;
} Mouse;

typedef struct {
	int x;
	int y;
	int width;
	int height;
} Car;

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int destroyed;
	int lane;
	int ready;
} Obstacle;

Mouse mouse;
Car car;
Obstacle rock;
Obstacle cone;
Obstacle hole;
Obstacle coin1;
Obstacle coin2;
Obstacle coin3;

/**
 * @brief Initializes the position of the mouse and registers all buttons as 0
 */
void initMouse();

/**
 * @brief Initializes game
 * @param state The struct with the current game state
 */
void initGame(GameState* state);

/**
 * @brief Checks for collisions among the car and the game elements (rocks, cones, holes and coins)
 * @return 0 if there are no collisions, 1 for collisions which amount to the loss of points and 2 for coins
 */
int checkCollision();

/**
 * @brief Updates all mouse components
 */
void updateMouse(ObstacleCourse* course);

/**
 * @brief Checks if button is inside a rectangular space (button area)
 * @param rect The struct with 4 coordinates correspondent to the corners of a rectangle
 * @return 0 if mouse is inside button, non-zero otherwise
 */
int mouseInButton(Rectangle* rect);

/**
 * @brief Updates car position according to mouse movement
 * @param state The struct with elements regarding the game state
 */
void mouseMove(GameState* state);

/**
 * @brief Draws score
 * @param state The struct with the current game state
 */
void drawScore(GameState* state);

/**
 * @brief Draws RTC date
 * @param state The struct with the current game state
 */
void drawDate(GameState* state);

/**
 * @brief Draws single digits as bitmaps
 * @param state The struct with the current game state
 * @param digit number to be drawn
 * @param deviation x coordinate
 * @param y y coordinate
 */
void drawDigit(GameState* state, int digit, int deviation, int y);
