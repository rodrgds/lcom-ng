/**
 * @file model.h
 * @brief file that is part of and MVC system and responsible by the modeling of the game
 * 
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/video/video.h"
#include "devices/timer/timer.h"
#include "configuration.h"
#include "model/sprites.h"
#include "view/view.h"

#include "images/dinossaurs/back1.xpm"
#include "images/dinossaurs/back2.xpm"
#include "images/dinossaurs/back3.xpm"
#include "images/dinossaurs/dino1.xpm"
#include "images/dinossaurs/dino2.xpm"
#include "images/dinossaurs/dino3.xpm"
#include "images/dinossaurs/dino4.xpm"
#include "images/dinossaurs/dino5.xpm"
#include "images/dinossaurs/dino6.xpm"
#include "images/dinossaurs/dino7.xpm"
#include "images/dinossaurs/dino8.xpm"
#include "images/menus/mainmenu.xpm"
#include "images/menus/menufinal.xpm"
#include "images/menus/background.xpm"
#include "images/mice/dino_mouse.xpm"
#include "images/dinossaurs/frame.xpm"
#include "images/mice/dino_cursor.xpm"
#include "images/numbers/colon.xpm"
#include "images/numbers/digit0.xpm"
#include "images/numbers/digit1.xpm"
#include "images/numbers/digit2.xpm"
#include "images/numbers/digit3.xpm"
#include "images/numbers/digit4.xpm"
#include "images/numbers/digit5.xpm"
#include "images/numbers/digit6.xpm"
#include "images/numbers/digit7.xpm"
#include "images/numbers/digit8.xpm"
#include "images/numbers/digit9.xpm"

typedef enum { MENU_START, MENU_INGAME, MENU_END } Menu;
typedef enum { RUNNING, EXIT } System;
typedef enum { EASY, MEDIUM, HARD } Difficulty;
typedef enum { NEW, KEEP } Frame;
typedef enum { DO, DONE } Setup;
typedef enum { FIRST_CHOICE, SECOND_CHOICE, PAUSE, AFTER_PAUSE } GamePhase;

/**
 * @brief creates every sprite used in the game using the method createSprite from sprite.h file
 * 
 */
void setupSprites();

/**
 * @brief frees the memory of every sprite used in the game using the method destroySprite from sprite.h file
 * 
 */
void destroySprites();

/**
 * @brief this function is responsible for calling the method flipBuffers at each timer interruption and that allows to have double buffering into the game and for having a counter so we can use it to do time stuff too
 * 
 * @return int: 0 if success otherwise 1
 */
int updateTimerState();

/**
 * @brief this function uses "state machines" to detect where the game is at the moment and uses the keyboard to check the pressed keys in order to change the states of the game
 * 
 */
void updateKeyboardState();

/**
 * @brief this fuction is responsable for interpreting the mouse movement and clicks, and update the mouse parameters
 * 
 */
void updateMouseState();

/**
 * @brief this function interprets the mouse clicks in the screen in order to change the "state machines" in some way
 * 
 */
void updateStates();


#endif /* _MODEL_H_ */
