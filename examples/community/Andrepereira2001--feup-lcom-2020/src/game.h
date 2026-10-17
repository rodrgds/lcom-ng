#pragma once

#include "rtc.h"


/**
 * \mainpage 
 * \section Description
 * This game was developed for the course unit Laboratório de Computadores in Faculty of Engineering of University of Porto, year 2020/2021.\n\n
 * Small game description: The game objective is to catch and get as many golds as they can in order to reach a record 
 * within a time interval: 100 seconds, for single player. The player is a miner in a car and the golds appear 
 * below him. With the help of the claw, controlled by the keyboard, and the pickaxe, controlled by the mouse, 
 * the player is supposed to transform the stones in golds by pressing the left button in the stone and then catch
 * them by moving the claw over the golds. The player should also be aware that some stones are not golds, 
 * but obsidians, and if he catches them his time decreases. In the meanwhile, during the game, there may be 
 * displayed some clocks that if catched, will give the player more time.\n\n
 * \section Controls
 *  - MOUSE movement and click to move/click.\n
 *  - A,W,S,D to move car of player 1.\n
 *  - LEFT,UP,DOWN,RIGHT to move car of player 2 (if multiplayer in one pc).\n
 *  - ESC to leave actual screen.\n
 *  - ENTER, while in loading screen, to start multiplayer in one pc.\n\n
 * \section License
 * Authors André Pereira and Maltilde Oliveira.\n
 * All rights reserved. 
 * 
 */

 /** @defgroup game
 * @{
 *
 * Functions related to the game unwind and handling interruptions. 
 */

/**
 * @brief  load the video mode that will be used, subscribe all interrupts, load all sprites
 * 
 */
void game_init();

/**
 * @brief  check all th interrupts while the game is running and make changes
 * 
 */
void game_run();

/**
 * @brief unscribe all the interrupts , unload sprites, and return to text mode
 * 
 */
void game_end();

/**
 * @brief  subscibre interrupts of the periferics
 * 
 * @return int  0 up on sucess 
 */
int subs_interrupts();

/**
 * @brief  unscribe interrupts of the periferics
 * 
 * @return int  0 up on sucess 
 */
int unsubs_interrupts();

/**
 * @brief  handle the keyboard interrupt and make all the ncesseary changes
 * 
 * @param bytes  bytes received in the interrupt
 * @param size  size of the bytes array
 * @return int    0 up on sucess
 */
int keyboard_handler(uint8_t bytes[], unsigned * size);

/**
 * @brief handle the mouse iterrupts
 * 
 * @param mouse_bytes - bytes received in the interrupt
 * @param counter - current reading byte
 * @return int 
 */
int mouse_handler(uint8_t mouse_bytes[], int * counter);

/**
 * @brief handle timer interrupts
 * 
 * @param date  date to be displayed
 * @param time_elapsed  elapsed time added 1 for each interrupt
 * @return int  0 up on sucess
 */
int timer_handler(Date_t * date,int *time_elapsed);

/**
 * @brief handles rtc interrupts
 * 
 * @param date  date read from rtc
 * @return int  0 up on sucess
 */
int rtc_handler(Date_t * date);

/**
 * @brief handles uart interrupts
 * 
 * @return int 
 */
int uart_handler();

/**
 * @brief reset game variavels all game variavels
 * 
 * @param seed_value - value of the seed
 * @param useSeed - if it should use the given seed or not
 * @return int 
 */
int reset_game(bool useSeed);

/**
 * @brief add a new score to the points array that contais the 5 biggest scores made in that run
 * 
 */
void add_score();

/**
 * @brief function used to send keys byte to the other VM
 * 
 * @param val value of the keys byte
 */
void send_keys_byte(uint8_t val);

/**
 * @brief - used to make the same function as rand() and srand() but works fine
 * This code module as imported from the internet being the link the following one
 * <https://stackoverflow.com/questions/7953612/using-a-random-number-to-generate-a-series-of-repeatable-numbers-in-c>
 * 
 * @return uint32_t - returning value;
 */
uint32_t myRand (void);

/**
 * @brief Set the seed value
 * 
 * @param seed_val- value of the seed
 */
void set_seed(uint8_t seed_val);

/**
 * @brief Get the seed value
 * 
 * @return uint32_t - value of the seed
 */
uint32_t get_seed();
