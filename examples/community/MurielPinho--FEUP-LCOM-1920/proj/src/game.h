#include <lcom/lcf.h>

#ifndef GAME_H
#define GAME_H

/** @defgroup Game Game
 * @{
 * Functions to control the game 
 */

/** @brief Enumeration that represents the states of the game*/
enum gameState
{
    /** @brief Game is currently being played*/
    Play,
    /** @brief User completed a mission*/
    Complete,
    /** @brief User completed the game*/
    End
};

/**
 * @brief Struct type for specifying game variables
 */
typedef struct
{
    enum gameState gState; /**< @brief Enumeration for the states of the game */
    int iterator;          /**< @brief int to represent the game iterator */
    int selectedPlanet;    /**< @brief int to represent the selected planet */
    int currentPlanet;     /**< @brief int to represent the current planet */
    int nVisitedEdges;     /**< @brief int to represent the number of visited edges */

} Game;

/**
 * @brief Function to create the menu object
 * @return Returns the created menu object
 */
Game *createGame();
/**
 * @brief Function to update game screen
 */
void updateScreen();
/**
 * @brief Function to draw the mission sprites
 */
void drawMission();
/**
 * @brief Function to draw the mission complete screen
 */
void drawComplete();
/**
 * @brief Function to draw the mission game complete screen
 */
void drawEnd();
/**
 * @brief Function to check and update game state
 */
void checkGameState();
/**
 * @brief Function to draw the current planet sprites
 */
void drawPlanets();
/**
 * @brief Function to draw the graph edges
 */
void drawEdges();
/**
 * @brief Function to reset the mission variables and state
 */
void resetMission();
/**
 * @brief Function to move the game to the next mission
 */
void missionComplete();
/**
 * @brief Function to loop in the game
 * @return Returns an int that represents the project state
 */
int gameLoop();
/**
 * @brief Function to initialize the game variables and objects
 * @return Returns 0 if successfull
 */
int initGame();
/**
 * @brief Function to handle the game input
 * @return Returns 0 if successfull
 */
int gameInput();
/**
 * @brief Function to subscribre the interrupts
 * @return Returns 0 if successfull
 */
int subscribeInterrupts();
/**
 * @brief Function to subscribre the interrupts
 * @return Returns 0 if successfull
 */
int unsubscribeInterrupts();
/**
 * @}
 */
#endif /*GAME_H*/
