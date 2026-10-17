#ifndef _SNAKE_H
#define _SNAKE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "snake.h"
#include "menu.h"
#include "pause.h"
#include "kbcKeys.h"
#include "devices/graphics.h"
#include "leaderboard.h"


enum DEVICE{
  KBC,
  TIMER,
  MOUSE,
  RTC,
  SERIAL_PORT
};


/*
 * @brief Function responsible for handling all the interrupts, where the driver_receiver loop takes place. Basically the loop of the game
 */
void gameLoop();

/*
 * @brief
 * @param device, the device of the interrupt
 */
void InterruptRouter(enum DEVICE device);

/*
 * @brief Responsible for handling all the interrupts of the Menu State
 * @param device, the device of the interrupt
 */
void MenuIH(enum DEVICE device);

/*
 * @brief Responsible for handling all the interrupts of the PauseMenu State
 * @param device, the device of the interrupt
 */
void PauseIH(enum DEVICE device);

/*
 * @brief Responsible for handling all the interrupts of the Play_Solo State
 * @param device, the device of the interrupt
 */
void PlaySoloIH(enum DEVICE device);


/*
 * @brief Responsible for handling all the interrupts of the Multiplayer State
 * @param device, the device of the interrupt
 */
void PlayMultiplayerIH(enum DEVICE device);

/*
 * @brief Responsible for handling all the interrupts of the Dead State
 * @param device, the device of the interrupt
 */
void DeadIH(enum DEVICE device);


/*
 * @brief Responsible for handling all the interrupts of the Play_Solo State
 * @param device, the device of the interrupt
 */
void changePosition();

/*
 * @brief Responsible for input key of the user, and passing it to the GameState
 */
void updateKBC();

/*
 * @brief Responsible for loading all the XPM's (optimize performance)
 */
void loadAll();

/*
 * @brief Inits all the letters of the alphabet and stores them in an array. (From the XPM font)
 */
void initLetters();

/*
 * @brief Responsible for handling all the interrupts of the LeaderBoard State
 * @param device, the device of the interrupt
 */
void BoardIH(enum DEVICE device);

#endif
