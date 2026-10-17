#ifndef _LEADER_H
#define _LEADER_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "kbcKeys.h"
#include <xpms/leaderMenu.xpm>
#include "devices/graphics.h"
#include <stdio.h>
#include <string.h>
#include "pause.h"


/**
 * @brief function used to read and update the leaderboard text file
 * 
 */
void leaderboard();
/**
 * @brief 
 * 
 */
void BoardTimerHandler();
/**
 * @brief return to menu if user presses escape
 * 
 * @param k - key that user pressed
 */
void BoardHandlerKBC(enum KEY k);
/**
 * @brief draws leaderboard
 * 
 */
void drawBoard();
/**
 * @brief draws background of leaderboard
 * 
 */
void drawDefBoard();

#endif
