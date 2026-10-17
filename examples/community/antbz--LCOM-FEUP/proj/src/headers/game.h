#pragma once

#include <lcom/lcf.h>
#include <stdio.h>

#include "screen.h"
#include "images.h"
#include "fruit.h"
#include "kbd_defs.h"
#include "gestures.h"
#include "mouse.h"
#include "serialport.h"


extern uint32_t timer_counter;

extern int rand_seed;

extern uint8_t data[2];

extern double cut_slope;

extern struct packet pp;
extern unsigned mouse_counter;
extern enum mouse_state_t cutting_state;
extern uint16_t cut_xi, cut_xf, cut_yi, cut_yf;
extern Screen* current_screen, *main_screen, *singleplayer_screen, *endsp_game_screen, *help_screen, *multiplayer_screen;
extern Fruit* mouse_pointer;
extern uint32_t score, combo;
bool multiplayer_mode;

extern xpm_image_t digits[10];
extern xpm_image_t game_images[NUM_IMAGES];

extern bool spectator;

extern uint8_t ser_in;
extern bool ser_connected;


/**
 * @brief Device types.
 * 
 * Used as parameters for the specific Screen handlers.
 */
enum device_t {
  KEYBOARD,
  MOUSE,
  TIMER,
  RTC,
  SERIAL
};


/**
 * @brief Interrupt handler for main Menu.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void mainMenuHandler(int dev);


/**
 * @brief Interrupt handler for single player mode.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void singlePlayerMenuHandler(int dev);


/**
 * @brief Interrupt handler for single player end screen.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void singlePlayerEndHandler(int dev);


/**
 * @brief Interrupt handler for help screen.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void helpHandler(int dev);


/**
 * @brief Interrupt handler for multi player.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void multiPlayerHandler(int dev);


/**
 * @brief Interrupt handler for spectator.
 * 
 * @param dev the device (device_t) that called the interrupt.
 */
void spectatorHandler(int dev);


/**
 * @brief Screen constructor for main menu.
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadMainScreen();


/**
 * @brief Screen constructor for single palyer game.
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadSinglePlayerScreen();


/**
 * @brief Screen constructor for end screen.
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadSinglePlayerEndScreen();


/**
 * @brief Screen constructor for help screen.
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadHelpScreen();


/**
 * @brief Screen constructor for multi player game (Player side).
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadMultiPlayerScreen();


/**
 * @brief Screen constructor for multi player game (Spectator side).
 * 
 * @return a pointer to the new screen created.
 */
Screen* loadSpectatorScreen();


/**
 * @brief Displays the current score/combo.
 * 
 * Displays the player's score and the last combo, if there
 * was one. For use during game run.
 */
void show_score();
