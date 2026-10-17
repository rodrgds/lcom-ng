#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "images.h"
#include "track.h"
#include "mouse.h"

/** @defgroup game game
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Enumerated type to evaluate Game State
 */
enum gameState{
  MAIN_MENU, /*!< Currently in Main Menu State*/
  GAME, /*!< Currently in Game State*/
  END_GAME, /*!< Currently in End Game state*/
  HIGHSCORE_MENU, /*!< Currently in End Game state*/
  LEVEL_SELECT, /*!< Level to be played */ 
  EXIT /*!< Currently in Exit Game State*/
};

/**
 * @brief Struc with information about a Date
 */
typedef struct {
  int sec; /*!< Date's second */
  int min; /*!< Date's minute */
  int hour; /*!< Date's hour */
  int day; /*!< Date's day */
  int month; /*!< Date's month */
  int year; /*!< Date's year */
} Date;

/**
 * @brief Struct with infrormation about mouse status
 */
typedef struct {
  int x; /*!< Mouse x axis position */
  int y; /*!< Mouse y axis position */
  int deltax; /*!< Mouse x axis movement */
  int deltay; /*!< Mouse y axis movement */
  bool press; /*!< Mouse button pressed */
  struct mouse_ev *ev; /*!< struct with Mouse's current event */
} MouseInfo;

/**
 * @brief Struct to store Car's information
 */
typedef struct {
  int maxSpeed; /*!< Car max speed allowed*/
  xpm_image_t img; /*!< Car image*/
} CarInfo;

/**
 * @brief Struct with the Game's information
 */
typedef struct {
	CarInfo car; /*!< Player Car  */
	RaceInfo race; /*!<Actual Race */
	int racenum;  /*!< Number of actual Race */ 
  int targetTime[8]; /*!< Array with target time for each race */ 
	int highscore[8]; /*!< Array with highcores */ 
  int highestLevel; /*!< Game's highest level */
  Date highscoreDate[8]; /*!< Array with highscore's date */
  MouseInfo * mousePlayer; /*!< Game's information about the mouse */
	ImageLibrary *image; /*!< All images */
  enum gameState state;/*!< Actual State of game */ 
} GameInfo;

//Car
/**
 * @brief Creates a car "CarInfo" with an image
 * @param img Image of the car tha will be created
 */
CarInfo (createCar)(xpm_image_t img);

//Game
/**
 * @brief Initializes the Game by loading the necessary images
 */
GameInfo (initGame)();


/**
 * @brief Plays the Race the PLayer has chosen
 * @param self GameInfo struct to change
 * @param num Number od the race to be run
 */
void (numberRace)(GameInfo * self, int num);

/**
 * @brief Changes the race to the following one
 * @param self Pointer to Game's information
 */
void nextRace(GameInfo * self);

/**
 * @brief Repeats the Race
 * @param self Pointer to Game's information
 */
void repeatRace(GameInfo * self);

/**
 * @brief Sets Race highscore
 * @param self Pointer to Game's information
 * @param racenum Number of the Race of the highscore to be set
 */
void setHighscore(GameInfo * self, int racenum);

/**
 * @brief Exits the Game
 * @param self Pointer to Game's information
 */
void exitGame(GameInfo * self);

#endif  /* INFO_H */
