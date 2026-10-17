#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "GameState.h"
#include "Score.h"
#include "video.h"
#include <lcom/lcf.h>

#define POINTS_PER_KILL    10

/** @defgroup Game Game
 * @{
 */

typedef enum states_t{
  MENU,
  HIGHSCORES,
  PLAYING,
  GAMEOVER,
  NEWHIGHSCORE,
  EXIT,
} State;

typedef struct game_t{
  xpm_image_t background;
  State state;
  Entity mouse;
  GameState gamestate;
  Score fs;
} Game;

/*
@brief load the font xpm's
*/

void loadFont();

/*
@brief loads some xpm's for the menus and initializes Game struct
*/

Game game();

/*
@brief draws the main menu
*/

void drawMenu();

/*
@brief draws the highscores menu
*/

void drawHighScores();

/*
@brief draws the gameover menu
*/

void drawGameOver();

/*
@brief draws the menus

@param g struct Game to check the state
*/

void drawGame(Game * g);

/*
@brief handles mouse interrupts

@param g Game struct
*/

void reactToMouseEvent(Game * g);

/*
@brief handles keybord interrupts

@param g Game struct
*/

void reactToKBDEvent(Game * g);

/*
@brief handles timer interrupts

@param g Game struct
*/
void reactToTimerEvent(Game * g);


#endif
