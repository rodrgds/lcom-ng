/**
 * @file Game.h
 * @brief This file contains functions to control the movement of every game entity
 * @date 2024-06-02
 */
#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "../models/Room.h"
#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../drivers/utils_drivers.h"
#include "../drivers/rtclock.h"

#include "Cloud.h"
#include "Datetime.h"


typedef enum {
    MENU_STATE,
    GAME_STATE,
    INSTRUCTIONS_STATE,
    LEAVE_STATE,
} GameState;

typedef struct {
    Room *room;
    GameState state;
    int counter;
} Game;
/**
 * @brief Creates a new game
 * 
 * @return pointer to game 
 */
Game* createNewGame();
/**
 * @brief Deletes allocated memory space for previously created game
 * 
 * @param game pointer to game to be deleted
 */
void deleteGame(Game* game);
/**
 * @brief Manages game states and calls entity control functions
 * 
 * @param game current game
 * @return always returns 0 
 */
int control_game(Game *game);
/**
 * @brief Controls player's movement direction 
 * 
 * @param game current game
 * @return always returns 0 
 */
int control_player(Game* game);
/**
 * @brief 
 * 
 * @param game 
 * @param cat 
 * @return int 
 */
int control_cat(Game* game, Entity* cat);

int petting_helper_on_mouse_int(Game* game);
int petting_helper_on_timer_int(Game* game);

Position futurePos(Entity* entity, Room* room);
/**
 * @brief Moves entity to previously defined (future)position
 * 
 * @param entity entity to move
 * @param room current room
 */
void moveEntity(Entity* entity, Room* room);
/**
 * @brief updates game time with RTC's time
 * 
 */
void updateGameTime();

#endif /* GAME_H */
